#define Uses_IPPROTO_TCP
#define Uses_fcntl
#define Uses_tcp_send_all
#define Uses_recv
#define Uses_DETAILED_IMMORTAL_ERR_STR
#define Uses_MEMSET_ZERO_O
#define Uses_socket_handle
#include <general.dep>

status tcps_init_tcp( tcp_h_t * tcp ) // first of all init every object
{
	MEMSET_ZERO_O( tcp );
	tcp->tcp_fd = INVALID_FD;
	return errOK;
}

status tcps_init_ssl_tcp( tcp_h_t * tcp , SSL_h_t * ssl )
{
	status d_error = tcps_init_tcp( tcp );
	if ( !d_error )
	{
		if ( !( d_error = sslh_init( ssl ) ) )
		{
			tcp->ssl_over_tcp = ssl;
		}
	}
	return d_error;
}

void tcps_init_msg_ext_buf( tcp_h_t * tcp , ExternalErrBuf * buf , size_t max_buf_sz ) // if not used error message just ignored
{
	// TODO . check buffer size
	tcp->dts_buf = ( Detail_ErrBuf * )buf;
	if ( tcp->ssl_over_tcp )
	{
		sslh_init_msg_ext_buf( tcp->ssl_over_tcp , buf , max_buf_sz );
	}
}

void tcps_init_msg_dts_buf( tcp_h_t * tcp , DtsErrBuf buf )
{
	tcp->dts_buf = buf;
	if ( tcp->ssl_over_tcp )
	{
		sslh_init_msg_dts_buf( tcp->ssl_over_tcp , buf );
	}
}

#ifndef server_side
status tcps_accept_tcp_connection( tcp_h_t * tcp , LPCSTR ip , int port , int timeout_sec , Brief_Err * imortalErrStr )
{
	Detail_ErrBuf detailErrBuf = { 0 };
	status d_error = create_server_socket_with_timeout( ip , port , timeout_sec , &tcp->tcp_fd , imortalErrStr , &detailErrBuf );
	if ( d_error )
	{
		if ( tcp->dts_buf && strlen( detailErrBuf ) )
		{
			strncpy( *tcp->dts_buf , detailErrBuf , sizeof( *tcp->dts_buf ) );
		}
	}
	else
	{
		int prevflags = fcntl( tcp->tcp_fd , F_GETFL , 0 );
		if ( prevflags == -1 )
		{
			_close_socket( &tcp->tcp_fd , imortalErrStr );
			STORE_BRIEF_ERR( imortalErrStr , "fcntl(F_GETFL) failed.\n" , false );
			return errsockopt;
		}
		if ( fcntl( tcp->tcp_fd , F_SETFL , prevflags | O_NONBLOCK ) == -1 )
		{
			_close_socket( &tcp->tcp_fd , imortalErrStr );
			STORE_BRIEF_ERR( imortalErrStr , "fcntl(O_NONBLOCK) failed.\n" , false );
			return errsockopt;
		}
		int opt = 1;
		if ( setsockopt( tcp->tcp_fd , IPPROTO_TCP , TCP_QUICKACK , &opt , sizeof( opt ) ) < 0 ) // Force immediate ACK. Decreases feedback latency. Disable delayed ACK at receiver
		{
			_close_socket( &tcp->tcp_fd , imortalErrStr );
			STORE_BRIEF_ERR( imortalErrStr , "setsockopt(TCP_QUICKACK)\n" , false );
			return errsockopt;
		}
		if ( setsockopt( tcp->tcp_fd , IPPROTO_TCP , TCP_NODELAY , &opt , sizeof( opt ) ) < 0 ) // Force immediate ACK. Decreases feedback latency. Disable delayed ACK at receiver
		{
			_close_socket( &tcp->tcp_fd , imortalErrStr );
			STORE_BRIEF_ERR( imortalErrStr , "setsockopt(TCP_NODELAY)\n" , false );
			return errsockopt;
		}

		tcp->tcp_conn_established = 1;
	}
	return d_error;
}

status tcps_accept_ssl_tcp_connection( tcp_h_t * tcp , LPCSTR ip , int port , int timeout_sec , LPCSTR server_certificate_path , LPCSTR server_key_path
	, LPCSTR ca_crt_path , Brief_Err * imortalErrStr )
{
	status d_error;

	// init ssl
	if ( tcp->ssl_over_tcp )
	{
		if ( ( d_error = sslh_serverside_pre_tcp_initialize_ctx( tcp->ssl_over_tcp , server_certificate_path , server_key_path , ca_crt_path , imortalErrStr ) ) )
		{
			STORE_BRIEF_ERR( imortalErrStr , "sslh_serverside_pre_tcp_initialize_ctx() failed.\n" , false );
			//if ( tcp->dts_buf && tcp->ssl_over_tcp->dts_buf )
			//{
			//	strncpy( tcp->dts_buf , tcp->ssl_over_tcp->dts_buf );
			//}
			return d_error;
		}
	}

	// try connect tcp
	if ( ( d_error = tcps_accept_tcp_connection( tcp , ip , port , timeout_sec , imortalErrStr ) ) )
	{
		sslh_cleanup_ctx( tcp->ssl_over_tcp );
		return d_error;
	}

	// try stablish ssl
	if ( ( d_error = sslh_serverside_post_tcp_ssl_handshake( tcp->ssl_over_tcp , tcp->tcp_fd , imortalErrStr ) ) )
	{
		STORE_BRIEF_ERR( imortalErrStr , "sslh_serverside_post_tcp_ssl_handshake() failed.\n" , false );
		//if ( tcp->dts_buf && tcp->ssl_over_tcp->dts_buf )
		//{
		//	strncpy( tcp->dts_buf , tcp->ssl_over_tcp->dts_buf );
		//}

		_close_socket( &tcp->tcp_fd , imortalErrStr );
		tcp->tcp_conn_established = 0;
		sslh_cleanup_ctx( tcp->ssl_over_tcp );
		return d_error;
	}

	return d_error;
}

#endif

#ifndef client_side
status tcps_connect_2_server( tcp_h_t * tcp , LPCSTR ip , int port , int timeout_sec , Brief_Err * imortalErrStr )
{
	Detail_ErrBuf detailErrBuf = { 0 };
	status d_error = connect_with_timeout( ip , port , timeout_sec , &tcp->tcp_fd , imortalErrStr , &detailErrBuf );
	if ( d_error )
	{
		if ( tcp->dts_buf && strlen( detailErrBuf ) )
		{
			strncpy( *tcp->dts_buf , detailErrBuf , sizeof( *tcp->dts_buf ) );
		}
	}
	else
	{
		tcp->tcp_conn_established = 1;
	}
	return d_error;
}

status tcps_connect_2_ssl_server( tcp_h_t * tcp , LPCSTR ip , int port , int timeout_sec , LPCSTR server_certificate_path , LPCSTR server_key_path
	, LPCSTR ca_crt_path , Brief_Err * imortalErrStr )
{
	status d_error;

	// init ssl
	if ( tcp->ssl_over_tcp )
	{
		if ( ( d_error = sslh_clientside_pre_tcp_initialize_ctx( tcp->ssl_over_tcp , server_certificate_path , server_key_path , ca_crt_path , imortalErrStr ) ) )
		{
			STORE_BRIEF_ERR( imortalErrStr , "sslh_clientside_pre_tcp_initialize_ctx() failed.\n" , false );
			//if ( tcp->dts_buf && tcp->ssl_over_tcp->dts_buf )
			//{
			//	strncpy( tcp->dts_buf , tcp->ssl_over_tcp->dts_buf );
			//}
			return d_error;
		}
	}

	// try connect tcp
	if ( ( d_error = tcps_connect_2_server( tcp , ip , port , timeout_sec , imortalErrStr ) ) )
	{
		sslh_cleanup_ctx( tcp->ssl_over_tcp );
		return d_error;
	}

	// try stablish ssl
	if ( ( d_error = sslh_clientside_post_tcp_ssl_handshake( tcp->ssl_over_tcp , tcp->tcp_fd , imortalErrStr ) ) )
	{
		STORE_BRIEF_ERR( imortalErrStr , "sslh_clientside_post_tcp_ssl_handshake() failed.\n" , false );
		//if ( tcp->dts_buf && tcp->ssl_over_tcp->dts_buf )
		//{
		//	strncpy( tcp->dts_buf , tcp->ssl_over_tcp->dts_buf );
		//}

		_close_socket( &tcp->tcp_fd , imortalErrStr );
		tcp->tcp_conn_established = 0;
		sslh_cleanup_ctx( tcp->ssl_over_tcp );
		return d_error;
	}

	return d_error;
}
#endif

//status tcps_read( tcp_h_t * tcp , buffer buf , size_t buf_sz , int * num_reads , Brief_Err * imortalErrStr )
//{
//	if ( !tcp ) return errArg;
//
//	if ( tcp->ssl_over_tcp )
//	{
//		status ret_err = sslh_read( tcp->ssl_over_tcp , buf , (int)buf_sz , num_reads , imortalErrStr );
//		switch ( ret_err )
//		{
//			case errPeerClosed:
//			{
//				tcps_close( tcp , imortalErrStr );
//				break;
//			}
//		}
//		return ret_err;
//	}
//	else
//	{
//		if ( ( *num_reads = ( int )recv( tcp->tcp_fd , buf , buf_sz , 0 ) ) <= 0 )
//		{
//			STORE_BRIEF_ERR( imortalErrStr , "recv() failed.\n" , true );
//			return errInvalidData;
//		}
//	}
//	return errOK;
//}

status tcps_ssl_read( tcp_h_t * tcp , buffer buf , size_t buf_sz , int * num_reads , Brief_Err * imortalErrStr )
{
	if ( !tcp ) return errArg;
	if ( !tcp->ssl_over_tcp ) return errFailure;
	status ret_err = sslh_read( tcp->ssl_over_tcp , buf , ( int )buf_sz , num_reads , imortalErrStr );
	switch ( ret_err )
	{
		case errPeerClosed:
		{
			tcps_close( tcp , imortalErrStr );
			break;
		}
	}
	return ret_err;
}

status tcps_write( tcp_h_t * tcp , buffer buf , size_t buf_sz , int * num_writes/*nullable*/ , Brief_Err * imortalErrStr )
{
	if ( !tcp ) return errArg;

	if ( tcp->ssl_over_tcp )
	{
		status ret_err = sslh_write( tcp->ssl_over_tcp , buf , ( int )buf_sz , num_writes , imortalErrStr );
		switch ( ret_err )
		{
			case errPeerClosed:
			{
				tcps_close( tcp , imortalErrStr );
				break;
			}
		}
		return ret_err;
	}
	else
	{
		if ( num_writes ) *num_writes = ( int )buf_sz;
		return tcp_send_all( tcp->tcp_fd , buf , buf_sz , 0 , SEND_TIMEOUT_ms , ACK_TIMEOUT_ms , RETRY_MECHANISM , imortalErrStr , tcp->dts_buf );
	}
	return errState;
}

// to keep alive and try to probe peer in semi normal sitribution time and does not have line up in connction
void tcps_enable_keepalive( tcp_h_t * tcp , Brief_Err * imortalErrStr )
{
	if ( !tcp || tcp->tcp_fd == INVALID_FD ) return;
	enable_keepalive_chaotic( tcp->tcp_fd , imortalErrStr );
}

void tcps_close( tcp_h_t * tcp , Brief_Err * imortalErrStr )
{
	if ( !tcp ) return;

	if ( tcp->ssl_over_tcp && sslh_is_handshaked( tcp->ssl_over_tcp ) )
	{
		sslh_cleanup_handshaked_ssl( tcp->ssl_over_tcp );
	}
	if ( tcp->tcp_conn_established )
	{
		_close_socket( &tcp->tcp_fd , imortalErrStr );
		tcp->tcp_conn_established = 0;
	}
	if ( tcp->ssl_over_tcp && sslh_has_context( tcp->ssl_over_tcp ) )
	{
		sslh_cleanup_ctx( tcp->ssl_over_tcp );
	}
}

bool tcps_socket_is_accessible( tcp_h_t * tcp )
{
	if ( tcp && tcp->tcp_conn_established )
	{
		if ( tcp->ssl_over_tcp )
		{
			return sslh_is_handshaked( tcp->ssl_over_tcp );
		}
		else
		{
			return true;
		}
	}
	return false;
}
