#define Uses_poll
#define Uses_DETAILED_IMMORTAL_ERR_STR
#define Uses_MEMSET_ZERO_O
#define Uses_SSL_handle
#include <general.dep>

status sslh_init( SSL_h_t * ssl )
{
	MEMSET_ZERO_O( ssl );
	return errOK;
}

void sslh_init_msg_ext_buf( SSL_h_t * ssl , ExternalErrBuf * buf , size_t max_buf_sz )
{
	ssl->dts_buf = ( Detail_ErrBuf * )buf;
}

void sslh_init_msg_dts_buf( SSL_h_t * ssl , DtsErrBuf buf )
{
	ssl->dts_buf = buf;
}

#ifndef private_fxn
_PRIVATE_FXN void sslh_drain_err_msg( DtsErrBuf buf )
{
	ulong err;
	size_t tmp_buf_sz = sizeof( *buf );
	char tmp_buf[ sizeof( *buf ) ];
	int snprintf_n , snprintf_sumn = 0;
	while ( ( err = ERR_get_error() ) != 0 )
	{
		tmp_buf[0] = EOS;
		ERR_error_string_n( err , tmp_buf , sizeof( tmp_buf ) );
		snprintf_sumn += ( snprintf_n = snprintf( ( ( LPSTR )( *buf ) ) + snprintf_sumn , tmp_buf_sz , "er:%s\n" , tmp_buf ) );
		if ( tmp_buf_sz > snprintf_n )
		{
			tmp_buf_sz -= ( size_t )snprintf_n;
		}
		else
		{
			break;
		}
	}
}

/// <summary> </summary> <returns>
/// errOK , errPeerClosed , errUsrLvl , errOSLvl , errNotImplemented
/// </returns>
_PRIVATE_FXN status sslh_handle_ssl_read_write_error( SSL_h_t * ssl , int ssl_fxn_ret , int * ssl_err , Brief_Err * imortalErrStr , bool * bretry )
{
	if ( bretry ) *bretry = false;
	switch( ( *ssl_err = SSL_get_error( ssl->ssl_obj , ssl_fxn_ret ) ) )
	{
		case SSL_ERROR_NONE:  /* should not happen here */
		{
			/* Not an error (rarely returned here) */
			return errOK;
		}
		case SSL_ERROR_WANT_READ:
		{
			if ( bretry ) *bretry = true;
			/*
			* SSL needs more data from the underlying socket.
			* Common with non-blocking sockets.
			*/
			return errOK;
		}
		case SSL_ERROR_WANT_WRITE:
		{
			if ( bretry ) *bretry = true;
			/*
			* SSL wants to write data before it can proceed.
			* Usually during handshake or renegotiation.
			*/
			return errOK;
		}
		case SSL_ERROR_WANT_CONNECT:
		{
			if ( bretry ) *bretry = true;
			STORE_BRIEF_ERR( imortalErrStr , "Non-blocking connect in progress (client-side).\n" , true );
			return errOK;
		}
		case SSL_ERROR_WANT_ACCEPT:
		{
			if ( bretry ) *bretry = true;
			STORE_BRIEF_ERR( imortalErrStr , "Non-blocking accept in progress (server-side).\n" , true );
			return errOK;
		}
		case SSL_ERROR_WANT_X509_LOOKUP: /*OpenSSL is waiting for your application to finish a certificate lookup*/
		{
			if ( bretry ) *bretry = true;
			STORE_BRIEF_ERR( imortalErrStr , "Certificate callback requested more data.\n" , true );
			/*
			* Certificate callback requested more data.
			* Very rare unless you use client cert callbacks.
			*/
			return errOK;
		}
		case SSL_ERROR_ZERO_RETURN: /*Peer is done, cleanly*/
		{
			/*
			* Clean TLS shutdown.
			* The peer sent "close_notify".
			* This is NOT an error.
			*/
			return errPeerClosed; /* connection closed cleanly */
		}
		case SSL_ERROR_SYSCALL:
		{
			/*
			* Fatal I/O error or unexpected EOF.
			* Look at errno AND OpenSSL error queue.
			*/
			if ( ssl_fxn_ret == 0 )
			{
				STORE_BRIEF_ERR( imortalErrStr , "SSL: unexpected EOF\n" , true );
				/* EOF without close_notify (protocol violation) */
			}
			else
			{
				STORE_BRIEF_ERR( imortalErrStr , "SSL syscall error\n" , true );
			}
			return errUsrLvl;
		}
		case SSL_ERROR_SSL:
		{
			/*
			* Real TLS protocol error.
			* Details are in OpenSSL error stack.
			*/
			STORE_BRIEF_ERR( imortalErrStr , "TLS protocol error. detail are provided.\n" , true );
			if ( ssl->dts_buf )
			{
				sslh_drain_err_msg( ssl->dts_buf );
			}
			return errOSLvl;
		}
		default:
		{
			/*
			* Should never happen.
			*/
			STORE_BRIEF_ERR( imortalErrStr , "SSL: unknown error.\n" , true );
			return errNotImplemented;
		}
	}
	return errNotImplemented;
}

_PRIVATE_FXN status wait_for_ssl( SSL_h_t * ssl , int ssl_err , int timeout_sec /*-1 wait forever*/ , Brief_Err * imortalErrStr )
{
	int fd = SSL_get_fd( ssl->ssl_obj );
	if ( fd < 0 )
	{
		STORE_BRIEF_ERR( imortalErrStr , "could not get file descriptor.\n" , true );
		return errNotFound;
	}
	struct pollfd pfd = { 0 };
	pfd.fd = fd;
	switch ( ssl_err )
	{
		case SSL_ERROR_WANT_READ:
		case SSL_ERROR_WANT_ACCEPT:
			pfd.events = POLLIN;
			break;
		case SSL_ERROR_WANT_WRITE:
		case SSL_ERROR_WANT_CONNECT:
			pfd.events = POLLOUT;
			break;
		default:
		{
			STORE_BRIEF_ERR( imortalErrStr , "is not SSL state. so wait does not applied in this state.\n" , true );
			return errState; /* not retriable */
		}
	}
	int poll_ret = poll( &pfd , 1 , timeout_sec );

	if ( poll_ret < 0 )
	{
		if ( errno == EINTR )
		{
			return wait_for_ssl( ssl , ssl_err , timeout_sec , imortalErrStr );   /* interrupted → poll again */
		}
		STORE_BRIEF_ERR( imortalErrStr , "is not SSL state. so wait does not applied in this state.\n" , true );
		return errFailure;       /* real failure */
	}

	if ( poll_ret == 0 )
	{
		return errTimeout;     /* timeout expired */
	}

	/* ---- poll_ret > 0: inspect revents ---- */

	if ( pfd.revents & POLLNVAL )
		return errFailure;

	if ( pfd.revents & POLLERR )
		return errOSLvl;

	if ( pfd.revents & POLLHUP )
		return errPeerClosed;

	/* readable + writable can happen together */
	if ( ( pfd.revents & POLLIN ) && ( pfd.revents & POLLOUT ) )
		return errOK;

	if ( pfd.revents & POLLIN )
		return errOK;

	if ( pfd.revents & POLLOUT )
		return errOK;

	/* Should not happen */
	return errFault;
}
#endif

#ifndef server_side
status sslh_serverside_pre_tcp_initialize_ctx( SSL_h_t * ssl , LPCSTR server_certificate_path , LPCSTR server_key_path , LPCSTR ca_crt_path , Brief_Err * imortalErrStr )
{
	const SSL_METHOD * method;
	OpenSSL_add_all_algorithms(); // Load all algorithms
	SSL_load_error_strings(); // Load error strings
	//OpenSSL_add_ssl_module(); // Add SSL module . in earlier release this job done automatically

	if ( !( method = TLS_server_method() ) ) // Use TLS method
	{
		STORE_BRIEF_ERR( imortalErrStr , "TLS_server_method() failed.\n" , true );
		if ( ssl->dts_buf )
		{
			sslh_drain_err_msg( ssl->dts_buf );
		}
		return errOSLvl;
	}

	if ( !( ssl->ssl_ctx = SSL_CTX_new( method ) ) ) // Create SSL context
	{
		STORE_BRIEF_ERR( imortalErrStr , "SSL_CTX_new() failed.\n" , true );
		if ( ssl->dts_buf )
		{
			sslh_drain_err_msg( ssl->dts_buf );
		}
		return errCreation;
	}

	// Load server certificate
	if ( SSL_CTX_use_certificate_file( ssl->ssl_ctx , server_certificate_path /*"server.crt"*/ , SSL_FILETYPE_PEM ) <= 0 )
	{
		STORE_BRIEF_ERR( imortalErrStr , "SSL_CTX_use_certificate_file(server.crt) failed.\n" , true );
		if ( ssl->dts_buf )
		{
			sslh_drain_err_msg( ssl->dts_buf );
		}
		sslh_cleanup_ctx( ssl );
		return errNotFound;
	}

	// Load server private key
	if ( SSL_CTX_use_PrivateKey_file( ssl->ssl_ctx , server_key_path/*"server.key"*/ , SSL_FILETYPE_PEM ) <= 0 )
	{
		STORE_BRIEF_ERR( imortalErrStr , "SSL_CTX_use_certificate_file(server.key) failed.\n" , true );
		if ( ssl->dts_buf )
		{
			sslh_drain_err_msg( ssl->dts_buf );
		}
		sslh_cleanup_ctx( ssl );
		return errNotFound;
	}

	// Verify the private key
	if ( !SSL_CTX_check_private_key( ssl->ssl_ctx ) )
	{
		STORE_BRIEF_ERR( imortalErrStr , "SSL_CTX_check_private_key() failed.\n" , true );
		if ( ssl->dts_buf )
		{
			snprintf( *ssl->dts_buf , sizeof( *ssl->dts_buf ) , "Private key does not match the certificate public key\n" );
		}
		sslh_cleanup_ctx( ssl );
		return errInvalidData;
	}

	//// Request and verify client certificate
	//SSL_CTX_set_verify( ssl->ssl_ctx , SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT , NULL );

	///* Load CA certificate to verify client certificates */
	//if ( !SSL_CTX_load_verify_locations( ssl->ssl_ctx , ca_crt_path /*"ca.crt"*/ , NULL ) )
	//{
	//	if ( imortalErrStr ) *imortalErrStr = ( "SSL_CTX_load_verify_locations(ca.crt) failed.\n" );
	//	if ( ssl->dts_buf )
	//	{
	//		snprintf( *ssl->dts_buf , sizeof( *ssl->dts_buf ) , "Failed to load CA certificate\n" );
	//	}
	//	sslh_cleanup_ctx( ssl );
	//	return errInvalidData;
	//}

	//// Set verification depth
	//SSL_CTX_set_verify_depth( ssl->ssl_ctx , 1 );

	return errOK;
}

status sslh_serverside_post_tcp_ssl_handshake( SSL_h_t * ssl , sockfd client_sock_id , Brief_Err * imortalErrStr )
{
	if ( !ssl ) return errArg;
	if ( ssl->ssl_obj ) return errFailure;
	
	if ( !( ssl->ssl_obj = SSL_new( ssl->ssl_ctx ) ) )
	{
		STORE_BRIEF_ERR( imortalErrStr , "SSL_new() failed.\n" , true );
		if ( ssl->dts_buf )
		{
			sslh_drain_err_msg( ssl->dts_buf );
		}
		return errCreation;
	}
	if ( !SSL_set_fd( ssl->ssl_obj , client_sock_id ) )
	{
		STORE_BRIEF_ERR( imortalErrStr , "SSL_set_fd() failed.\n" , true );
		if ( ssl->dts_buf )
		{
			sslh_drain_err_msg( ssl->dts_buf );
		}
		sslh_cleanup_ssl_obj( ssl );
		return errOSLvl;
	}

	// Perform SSL handshake
	int ssl_fxn_ret = 0;
	int ssl_err = 0;
	bool bretry = false;
	for ( ; /*TODO . circuit breaker*/ ; )
	{
		bretry = false;
		if ( ( ssl_fxn_ret = SSL_accept( ssl->ssl_obj ) ) <= 0 )
		{
			status stat_ret = sslh_handle_ssl_read_write_error( ssl , ssl_fxn_ret , &ssl_err , imortalErrStr , &bretry );
			switch( stat_ret )
			{
				case errPeerClosed:
				{
					sslh_cleanup_ssl_obj( ssl );
					// outer should clean up server side connection
					return stat_ret; // outer should cleanup whole tcp, ssl 
				}
			}

			if ( bretry )
			{
				switch( ( stat_ret = wait_for_ssl( ssl , ssl_err , DEFAULT_BAD_NETWORK_HANDSHAKE_TIMEOUT , imortalErrStr ) ) )
				{
					case errPeerClosed:
					{
						sslh_cleanup_ssl_obj( ssl );
						return stat_ret; // outer should cleanup whole tcp, ssl 
					}
					case errTimeout: // timeout on handshake different with read or write situation
					{
						sslh_cleanup_ssl_obj( ssl );
						return stat_ret;
					}
					default:
					{
						break;
					}
					case errOK:
					{
						continue;
					}
				}
			}

			sslh_cleanup_ssl_obj( ssl );
			return errAccept;
		}
		else
		{
			ssl->is_handshaked = true;
			return errOK; /* handshake done */
		}
	}
	
	return errFailure;
}
#endif

#ifndef client_side
status sslh_clientside_pre_tcp_initialize_ctx( SSL_h_t * ssl , LPCSTR server_certificate_path , LPCSTR server_key_path , LPCSTR ca_crt_path , Brief_Err * imortalErrStr )
{
	const SSL_METHOD * method;
	OpenSSL_add_all_algorithms(); // Load all algorithms
	SSL_load_error_strings(); // Load error strings
	//OpenSSL_add_ssl_module(); // Add SSL module . in earlier release this job done automatically

	if ( !( method = TLS_client_method() ) ) // Use TLS method
	{
		STORE_BRIEF_ERR( imortalErrStr , "TLS_client_method() failed.\n" , true );
		if ( ssl->dts_buf )
		{
			sslh_drain_err_msg( ssl->dts_buf );
		}
		return errNotFound;
	}

	if ( !( ssl->ssl_ctx = SSL_CTX_new( method ) ) ) // Create SSL context
	{
		STORE_BRIEF_ERR( imortalErrStr , "SSL_CTX_new() failed.\n" , true );
		if ( ssl->dts_buf )
		{
			sslh_drain_err_msg( ssl->dts_buf );
		}
		return errCreation;
	}

	//// Load client certificate and private key 
	//if ( SSL_CTX_use_certificate_file( ssl->ssl_ctx , server_certificate_path /*"client.crt"*/ , SSL_FILETYPE_PEM ) <= 0 )
	//{
	//	if ( imortalErrStr ) *imortalErrStr = ( "SSL_CTX_use_certificate_file(client.crt) failed.\n" );
	//	if ( ssl->dts_buf )
	//	{
	//		sslh_drain_err_msg( ssl->dts_buf );
	//	}
	//	sslh_cleanup_ctx( ssl );
	//	return errNotFound;
	//}

	//// Load server private key 
	//if ( SSL_CTX_use_PrivateKey_file( ssl->ssl_ctx , server_key_path/*"client.key"*/ , SSL_FILETYPE_PEM ) <= 0 )
	//{
	//	if ( imortalErrStr ) *imortalErrStr = ( "SSL_CTX_use_certificate_file(server.key) failed.\n" );
	//	if ( ssl->dts_buf )
	//	{
	//		sslh_drain_err_msg( ssl->dts_buf );
	//	}
	//	sslh_cleanup_ctx( ssl );
	//	return errNotFound;
	//}

	//// Verify the private key
	//if ( !SSL_CTX_check_private_key( ssl->ssl_ctx ) )
	//{
	//	if ( imortalErrStr ) *imortalErrStr = ( "SSL_CTX_check_private_key() failed.\n" );
	//	if ( ssl->dts_buf )
	//	{
	//		snprintf( *ssl->dts_buf , sizeof( *ssl->dts_buf ) , "Private key does not match the certificate public key\n" );
	//	}
	//	sslh_cleanup_ctx( ssl );
	//	return errInvalidData;
	//}

	///* Request and verify client certificate */
	//SSL_CTX_set_verify( ssl->ssl_ctx , SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT , NULL );

	///* Load CA certificate to verify client certificates */
	//SSL_CTX_load_verify_locations( ssl->ssl_ctx , ca_crt_path /*"ca.crt"*/ , NULL );

	return errOK;
}

status sslh_clientside_post_tcp_ssl_handshake( SSL_h_t * ssl , sockfd client_sock_id , Brief_Err * imortalErrStr )
{
	if ( !ssl ) return errArg;
	if ( ssl->ssl_obj ) return errFailure;

	if ( !( ssl->ssl_obj = SSL_new( ssl->ssl_ctx ) ) )
	{
		STORE_BRIEF_ERR( imortalErrStr , "SSL_new() failed.\n" , true );
		if ( ssl->dts_buf )
		{
			sslh_drain_err_msg( ssl->dts_buf );
		}
		return errCreation;
	}
	if ( !SSL_set_fd( ssl->ssl_obj , client_sock_id ) )
	{
		STORE_BRIEF_ERR( imortalErrStr , "SSL_set_fd() failed.\n" , true );
		if ( ssl->dts_buf )
		{
			sslh_drain_err_msg( ssl->dts_buf );
		}
		sslh_cleanup_ssl_obj( ssl );
		return errOSLvl;
	}

	int ssl_fxn_ret = 0;
	int ssl_err = 0;
	bool bretry = false;
	for ( ; /*TODO . circuit breaker*/ ; )
	{
		bretry = false;
		if ( ( ssl_fxn_ret = SSL_connect( ssl->ssl_obj ) ) <= 0 )
		{
			status stat_ret = sslh_handle_ssl_read_write_error( ssl , ssl_fxn_ret , &ssl_err , imortalErrStr , &bretry );
			switch ( stat_ret )
			{
				case errPeerClosed:
				{
					sslh_cleanup_ssl_obj( ssl );
					// outer should clean up server side connection
					return stat_ret; // outer should cleanup whole tcp, ssl 
				}
			}

			if ( bretry )
			{
				switch ( ( stat_ret = wait_for_ssl( ssl , ssl_err , DEFAULT_BAD_NETWORK_HANDSHAKE_TIMEOUT , imortalErrStr ) ) )
				{
					case errPeerClosed:
					{
						sslh_cleanup_ssl_obj( ssl );
						return stat_ret; // outer should cleanup whole tcp, ssl 
					}
					case errTimeout:
					{
						sslh_cleanup_ssl_obj( ssl );
						return stat_ret;
					}
					default:
					{
						break;
					}
					case errOK:
					{
						continue;
					}
				}
			}

			sslh_cleanup_ssl_obj( ssl );
			return errFault;
		}
		else
		{
			return errOK;
		}
	}

	return errFailure; // until now in this fxn it should determind that how to act with fault
}
#endif

status sslh_read( SSL_h_t * ssl , buffer buf , int buf_sz , int * num_reads , Brief_Err * imortalErrStr )
{
	if ( !ssl ) return errArg;
	if ( !ssl->ssl_obj ) return errFailure;
	
	bool bretry = false;
	int ssl_fxn_ret = 0;
	int ssl_err = 0;
	for ( ; /*TODO . circuit breaker*/ ; )
	{
		bretry = false;
		if ( !ssl->ssl_obj ) return errFailure;
		if ( ( ssl_fxn_ret = *num_reads = SSL_read( ssl->ssl_obj , buf , buf_sz ) ) <= 0 ) // when it exception because it is test app and i should shutdown on other side this problem is ignorable
		{
			status stat_ret = sslh_handle_ssl_read_write_error( ssl , *num_reads , &ssl_err , imortalErrStr , &bretry );
			switch ( stat_ret )
			{
				case errPeerClosed:
				{
					// outer should clean up server side connection
					return stat_ret; // outer should cleanup whole tcp, ssl 
				}
			}

			if ( bretry )
			{
				switch ( ( stat_ret = wait_for_ssl( ssl , ssl_err , DEFAULT_BAD_NETWORK_HANDSHAKE_TIMEOUT /*TODO*/ , imortalErrStr ) ) )
				{
					case errPeerClosed:
					{
						return stat_ret; // outer should cleanup whole tcp, ssl 
					}
					case errTimeout:
					{
						return stat_ret;
					}
					default: // TODO . remove this default and spec for each case what should i do
					{
						return stat_ret;
					}
					case errOK:
					{
						continue; // read again
					}
				}
			}

			//sslh_cleanup_ssl_obj( ssl ); // why clean up . in this fxn read failed doen not mean ssl connection is curropted
			return errFault;
		}
		else
		{
			return errOK; /* read complete */
		}
	}

	return errFailure;
}

status sslh_write( SSL_h_t * ssl , buffer buf , int buf_sz , int * byte_writes /*nullable*/ , Brief_Err * imortalErrStr)
{
	if ( !ssl ) return errArg;
	if ( !ssl->ssl_obj ) return errFailure;

	bool bretry = false;
	int ssl_fxn_ret = 0;
	int ssl_err = 0;
	for ( ; /*TODO . circuit breaker*/ ; )
	{
		bretry = false;
		if ( !ssl->ssl_obj ) return errFailure;
		if ( ( ssl_fxn_ret = SSL_write( ssl->ssl_obj , buf , buf_sz ) ) <= 0 )
		{
			status stat_ret = sslh_handle_ssl_read_write_error( ssl , ssl_fxn_ret , &ssl_err , imortalErrStr , &bretry );
			switch ( stat_ret )
			{
				case errPeerClosed:
				{
					// outer should clean up server side connection
					return stat_ret; // outer should cleanup whole tcp, ssl 
				}
			}

			if ( bretry )
			{
				switch ( ( stat_ret = wait_for_ssl( ssl , ssl_err , DEFAULT_BAD_NETWORK_HANDSHAKE_TIMEOUT /*TODO*/ , imortalErrStr ) ) )
				{
					case errPeerClosed:
					{
						return stat_ret; // outer should cleanup whole tcp, ssl 
					}
					case errTimeout:
					{
						return stat_ret;
					}
					default: // TODO . remove this default and spec for each case what should i do
					{
						break;
					}
					case errOK:
					{
						continue;
					}
				}
			}

			//sslh_cleanup_ssl_obj( ssl ); // write could not happened. why clean ssl obj
			return errFault; // i donot know that this fault cause failure
		}
		else
		{
			if ( byte_writes ) *byte_writes = ssl_fxn_ret;
			return errOK;
		}
	}

	return errFailure;
}

bool sslh_is_handshaked( SSL_h_t * ssl )
{
	if ( ssl && ssl->ssl_obj && ssl->is_handshaked )
	{
		return true;
	}
	return false;
}

bool sslh_has_context( SSL_h_t * ssl )
{
	if ( ssl && ssl->ssl_ctx )
	{
		return true;
	}
	return false;
}

void sslh_cleanup_handshaked_ssl( SSL_h_t * ssl )
{
	if ( ssl && ssl->ssl_obj )
	{
		//struct timeval tv = { 5, 0 }; // 5 second timeout
		//setsockopt( sockfd , SOL_SOCKET , SO_RCVTIMEO , &tv , sizeof( tv ) );
		SSL_shutdown( ssl->ssl_obj );
		ssl->is_handshaked = false;
	}
	sslh_cleanup_ssl_obj( ssl );
}

void sslh_cleanup_ssl_obj( SSL_h_t * ssl )
{
	if ( ssl && ssl->ssl_obj )
	{
		SSL_free( ssl->ssl_obj );
		ssl->ssl_obj = NULL;
	}
}

void sslh_cleanup_ctx( SSL_h_t * ssl )
{
	if ( ssl && ssl->ssl_ctx )
	{
		SSL_CTX_free( ssl->ssl_ctx );
		ssl->ssl_ctx = NULL;
	}

	ERR_free_strings();
	EVP_cleanup();
}
