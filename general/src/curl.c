#define Uses_INIT_BREAKABLE_FXN
#define Uses_BRIF_M_BREAK_IF
#define Uses_MEMSET_ZERO_O
#define Uses_TOSTRING
#define Uses_Curl_handle
#include <general.dep>

status curlh_init( Curl_h_t * curlh )
{
	MEMSET_ZERO_O( curlh );
	return errOK;
}

_PRIVATE_FXN IMMORTAL_LPCSTR pr_convert_curl_err_2_str( CURLcode result )
{
	#define SALAM_BOBO( code ) case code: return TOSTRING(code);

	switch ( result )
	{
			SALAM_BOBO( CURLE_UNSUPPORTED_PROTOCOL )
			SALAM_BOBO( CURLE_FAILED_INIT )
			SALAM_BOBO( CURLE_URL_MALFORMAT )
			SALAM_BOBO( CURLE_NOT_BUILT_IN )
			SALAM_BOBO( CURLE_COULDNT_RESOLVE_PROXY )
			SALAM_BOBO( CURLE_COULDNT_RESOLVE_HOST )
			SALAM_BOBO( CURLE_COULDNT_CONNECT )
			SALAM_BOBO( CURLE_WEIRD_SERVER_REPLY )
			SALAM_BOBO( CURLE_REMOTE_ACCESS_DENIED )
			SALAM_BOBO( CURLE_FTP_ACCEPT_FAILED )
			SALAM_BOBO( CURLE_FTP_WEIRD_PASS_REPLY )
			SALAM_BOBO( CURLE_FTP_ACCEPT_TIMEOUT )
			SALAM_BOBO( CURLE_FTP_WEIRD_PASV_REPLY )
			SALAM_BOBO( CURLE_FTP_WEIRD_227_FORMAT )
			SALAM_BOBO( CURLE_FTP_CANT_GET_HOST )
			SALAM_BOBO( CURLE_HTTP2 )
			SALAM_BOBO( CURLE_FTP_COULDNT_SET_TYPE )
			SALAM_BOBO( CURLE_PARTIAL_FILE )
			SALAM_BOBO( CURLE_FTP_COULDNT_RETR_FILE )
			SALAM_BOBO( CURLE_OBSOLETE20 )
			SALAM_BOBO( CURLE_QUOTE_ERROR )
			SALAM_BOBO( CURLE_HTTP_RETURNED_ERROR )
			SALAM_BOBO( CURLE_WRITE_ERROR )
			SALAM_BOBO( CURLE_OBSOLETE24 )
			SALAM_BOBO( CURLE_UPLOAD_FAILED )
			SALAM_BOBO( CURLE_READ_ERROR )
			SALAM_BOBO( CURLE_OUT_OF_MEMORY )
			SALAM_BOBO( CURLE_OPERATION_TIMEDOUT )
			SALAM_BOBO( CURLE_OBSOLETE29 )
			SALAM_BOBO( CURLE_FTP_PORT_FAILED )
			SALAM_BOBO( CURLE_FTP_COULDNT_USE_REST )
			SALAM_BOBO( CURLE_OBSOLETE32 )
			SALAM_BOBO( CURLE_RANGE_ERROR )
			SALAM_BOBO( CURLE_OBSOLETE34 )
			SALAM_BOBO( CURLE_SSL_CONNECT_ERROR )
			SALAM_BOBO( CURLE_BAD_DOWNLOAD_RESUME )
			SALAM_BOBO( CURLE_FILE_COULDNT_READ_FILE )
			SALAM_BOBO( CURLE_LDAP_CANNOT_BIND )
			SALAM_BOBO( CURLE_LDAP_SEARCH_FAILED )
			SALAM_BOBO( CURLE_OBSOLETE40 )
			SALAM_BOBO( CURLE_OBSOLETE41 )
			SALAM_BOBO( CURLE_ABORTED_BY_CALLBACK )
			SALAM_BOBO( CURLE_BAD_FUNCTION_ARGUMENT )
			SALAM_BOBO( CURLE_OBSOLETE44 )
			SALAM_BOBO( CURLE_INTERFACE_FAILED )
			SALAM_BOBO( CURLE_OBSOLETE46 )
			SALAM_BOBO( CURLE_TOO_MANY_REDIRECTS )
			SALAM_BOBO( CURLE_UNKNOWN_OPTION )
			SALAM_BOBO( CURLE_SETOPT_OPTION_SYNTAX )
			SALAM_BOBO( CURLE_OBSOLETE50 )
			SALAM_BOBO( CURLE_OBSOLETE51 )
			SALAM_BOBO( CURLE_GOT_NOTHING )
			SALAM_BOBO( CURLE_SSL_ENGINE_NOTFOUND )
			SALAM_BOBO( CURLE_SSL_ENGINE_SETFAILED )
			SALAM_BOBO( CURLE_SEND_ERROR )
			SALAM_BOBO( CURLE_RECV_ERROR )
			SALAM_BOBO( CURLE_OBSOLETE57 )
			SALAM_BOBO( CURLE_SSL_CERTPROBLEM )
			SALAM_BOBO( CURLE_SSL_CIPHER )
			SALAM_BOBO( CURLE_PEER_FAILED_VERIFICATION )
			SALAM_BOBO( CURLE_BAD_CONTENT_ENCODING )
			SALAM_BOBO( CURLE_OBSOLETE62 )
			SALAM_BOBO( CURLE_FILESIZE_EXCEEDED )
			SALAM_BOBO( CURLE_USE_SSL_FAILED )
			SALAM_BOBO( CURLE_SEND_FAIL_REWIND )
			SALAM_BOBO( CURLE_SSL_ENGINE_INITFAILED )
			SALAM_BOBO( CURLE_LOGIN_DENIED )
			SALAM_BOBO( CURLE_TFTP_NOTFOUND )
			SALAM_BOBO( CURLE_TFTP_PERM )
			SALAM_BOBO( CURLE_REMOTE_DISK_FULL )
			SALAM_BOBO( CURLE_TFTP_ILLEGAL )
			SALAM_BOBO( CURLE_TFTP_UNKNOWNID )
			SALAM_BOBO( CURLE_REMOTE_FILE_EXISTS )
			SALAM_BOBO( CURLE_TFTP_NOSUCHUSER )
			SALAM_BOBO( CURLE_OBSOLETE75 )
			SALAM_BOBO( CURLE_OBSOLETE76 )
			SALAM_BOBO( CURLE_SSL_CACERT_BADFILE )
			SALAM_BOBO( CURLE_REMOTE_FILE_NOT_FOUND )
			SALAM_BOBO( CURLE_SSH )
			SALAM_BOBO( CURLE_SSL_SHUTDOWN_FAILED )
			SALAM_BOBO( CURLE_AGAIN )
			SALAM_BOBO( CURLE_SSL_CRL_BADFILE )
			SALAM_BOBO( CURLE_SSL_ISSUER_ERROR )
			SALAM_BOBO( CURLE_FTP_PRET_FAILED )
			SALAM_BOBO( CURLE_RTSP_CSEQ_ERROR )
			SALAM_BOBO( CURLE_RTSP_SESSION_ERROR )
			SALAM_BOBO( CURLE_FTP_BAD_FILE_LIST )
			SALAM_BOBO( CURLE_CHUNK_FAILED )
			SALAM_BOBO( CURLE_NO_CONNECTION_AVAILABLE )
			SALAM_BOBO( CURLE_SSL_PINNEDPUBKEYNOTMATCH )
			SALAM_BOBO( CURLE_SSL_INVALIDCERTSTATUS )
			SALAM_BOBO( CURLE_HTTP2_STREAM )
			SALAM_BOBO( CURLE_RECURSIVE_API_CALL )
			SALAM_BOBO( CURLE_AUTH_ERROR )
			SALAM_BOBO( CURLE_HTTP3 )
			SALAM_BOBO( CURLE_QUIC_CONNECT_ERROR )
			SALAM_BOBO( CURLE_PROXY )
			SALAM_BOBO( CURLE_SSL_CLIENTCERT )
			SALAM_BOBO( CURLE_UNRECOVERABLE_POLL )
			SALAM_BOBO( CURLE_TOO_LARGE )
			SALAM_BOBO( CURLE_ECH_REQUIRED )
			default:;
	}

	#undef SALAM_BOBO
	return "bad curl situation";
}

_PRIVATE_FXN int is_http_success( long code )
{
	return ( code >= 200 && code < 300 );
}

_PRIVATE_FXN int is_http_redirect( long code )
{
	return ( code >= 300 && code < 400 );
}

_PRIVATE_FXN int is_http_client_error( long code )
{
	return ( code >= 400 && code < 500 );
}

_PRIVATE_FXN int is_http_server_error( long code )
{
	return ( code >= 500 );
}

_PRIVATE_FXN status http_result_err( long http_code , Brief_Err * imortalErrStr )
{
	status d_error;
	/* 3) Handle HTTP code */
	if ( is_http_success( http_code ) )
	{
		/* 2xx */
		/* Request succeeded */
		d_error = errOK;
	}
	else if ( is_http_redirect( http_code ) )
	{
		/* 3xx */
		/* Redirect response */
		/* Usually auto-handled if FOLLOWLOCATION is set */
		d_error = errFault;
		STORE_INDIR_BRIEF_ERR( imortalErrStr , "" , 0 );
	}
	else if ( is_http_client_error( http_code ) )
	{
		/* 4xx */
		/* Client mistake */
		d_error = errFault;
		if ( http_code == 401 || http_code == 403 )
		{
			STORE_INDIR_BRIEF_ERR( imortalErrStr , "auth_error" , 0 );
		}
		else if ( http_code == 404 )
		{
			STORE_INDIR_BRIEF_ERR( imortalErrStr , "no handler" , 0 );
		}
		else
		{
			STORE_INDIR_BRIEF_ERR( imortalErrStr , "client_error" , 0 );
		}
	}
	else if ( is_http_server_error( http_code ) )
	{
		/* 5xx */
		/* Server problem */
		d_error = errFault;
		STORE_INDIR_BRIEF_ERR( imortalErrStr , "server error" , 0 );
	}
	else
	{
		/* Very rare or invalid */
		d_error = errFault;
		STORE_INDIR_BRIEF_ERR( imortalErrStr , "Unknown HTTP error" , 0 );
	}
	return d_error;
}

status curlh_connect_curl( Curl_h_t * curlh , Brief_Err * imortalErrStr )
{
	status d_error = errOK;  /*c does not have class and data member*/
	int _ErrLvl = 0;
	CURLcode result;

	BRIF_M_BREAK_IF( !( curlh->curl = curl_easy_init() ) , errMemoryLow , 0 , "curl_easy_init failed\n" , imortalErrStr );

	/* Headers */
	BRIF_M_BREAK_IF( !( curlh->curl_hdrs = curl_slist_append( curlh->curl_hdrs , "Content-Type: application/json" ) ) , errMemoryLow , 0 , "curl_slist_append failed\n" , imortalErrStr );

	/* optional but good practice */
	BRIF_M_BREAK_IF( !( curlh->curl_hdrs = curl_slist_append( curlh->curl_hdrs , "Accept: application/json" ) ) , errMemoryLow , 0 , "curl_slist_append failed\n" , imortalErrStr );

	BRIF_M_BREAK_IF( ( result = curl_easy_setopt( curlh->curl , CURLOPT_HTTPHEADER , curlh->curl_hdrs ) ) != CURLE_OK , errFault , 1 , pr_convert_curl_err_2_str( result ) , imortalErrStr );

	// use certificate to secure connection
	BRIF_M_BREAK_IF( ( result = curl_easy_setopt( curlh->curl , CURLOPT_CAINFO , "http_ca.crt" ) ) != CURLE_OK , errFault , 1 , pr_convert_curl_err_2_str( result ) , imortalErrStr );

	/* Authentication */
	BRIF_M_BREAK_IF( ( result = curl_easy_setopt( curlh->curl , CURLOPT_HTTPAUTH , CURLAUTH_BASIC ) ) != CURLE_OK , errFault , 1 , pr_convert_curl_err_2_str( result ) , imortalErrStr );
	BRIF_M_BREAK_IF( ( result = curl_easy_setopt( curlh->curl , CURLOPT_USERNAME , "logtoel" ) ) != CURLE_OK , errFault , 1 , pr_convert_curl_err_2_str( result ) , imortalErrStr );
	BRIF_M_BREAK_IF( ( result = curl_easy_setopt( curlh->curl , CURLOPT_PASSWORD , "razhmanlogtoel" ) ) != CURLE_OK , errFault , 1 , pr_convert_curl_err_2_str( result ) , imortalErrStr );

	BRIF_M_BREAK_IF( ( result = curl_easy_setopt( curlh->curl , CURLOPT_SSL_VERIFYPEER , 1L ) ) != CURLE_OK , errFault , 1 , pr_convert_curl_err_2_str( result ) , imortalErrStr );
	BRIF_M_BREAK_IF( ( result = curl_easy_setopt( curlh->curl , CURLOPT_SSL_VERIFYHOST , 2L ) ) != CURLE_OK , errFault , 1 , pr_convert_curl_err_2_str( result ) , imortalErrStr );

	/* Timeouts (important in production) */
	BRIF_M_BREAK_IF( ( result = curl_easy_setopt( curlh->curl , CURLOPT_CONNECTTIMEOUT , 5L ) ) != CURLE_OK , errFault , 1 , pr_convert_curl_err_2_str( result ) , imortalErrStr );
	BRIF_M_BREAK_IF( ( result = curl_easy_setopt( curlh->curl , CURLOPT_TIMEOUT , 10L ) ) != CURLE_OK , errFault , 1 , pr_convert_curl_err_2_str( result ) , imortalErrStr );

	// use post instead of 
	BRIF_M_BREAK_IF( ( result = curl_easy_setopt( curlh->curl , CURLOPT_POST , 1L ) ) != CURLE_OK , errFault , 1 , pr_convert_curl_err_2_str( result ) , imortalErrStr );

	// donot close connection after usage is complete and keep alive for furute usage
	BRIF_M_BREAK_IF( ( result = curl_easy_setopt( curlh->curl , CURLOPT_FORBID_REUSE , 0L ) ) != CURLE_OK , errFault , 1 , pr_convert_curl_err_2_str( result ) , imortalErrStr ); /*make tcp conn open*/

	//BRIF_M_BREAK_IF( ( result = curl_easy_setopt( curlh->curl , CURLOPT_WRITEFUNCTION , NULL ) ) != CURLE_OK , errFault , 1 , pr_convert_curl_err_2_str( result ) , imortalErrStr );

	// donot echo send on screen and just let me decide result by error code
	curlh->devnull = fopen( "/dev/null" , "w" );
	BRIF_M_BREAK_IF( ( result = curl_easy_setopt( curlh->curl , CURLOPT_WRITEDATA , curlh->devnull ) ) != CURLE_OK , errFault , 1 , pr_convert_curl_err_2_str( result ) , imortalErrStr );

	// Verbose output goes to stderr, not stdout
	BRIF_M_BREAK_IF( ( result = curl_easy_setopt( curlh->curl , CURLOPT_VERBOSE , 0L ) ) != CURLE_OK , errFault , 1 , pr_convert_curl_err_2_str( result ) , imortalErrStr );

	// If you want curl_easy_perform() to fail on 4xx/5xx then curl_easy_perform() returns CURLE_HTTP_RETURNED_ERROR
	BRIF_M_BREAK_IF( ( result = curl_easy_setopt( curlh->curl , CURLOPT_FAILONERROR , 1L ) ) != CURLE_OK , errFault , 1 , pr_convert_curl_err_2_str( result ) , imortalErrStr );

	/* Elasticsearch endpoint */
	BRIF_M_BREAK_IF( ( result = curl_easy_setopt( curlh->curl , CURLOPT_URL , "https://10.202.18.129:9200/binesh_sensors_fqdn_stats-app1-2026.02.08/_doc" ) ) != CURLE_OK , errFault , 1 , pr_convert_curl_err_2_str( result ) , imortalErrStr );

	BEGIN_RET
	N_END_RET
}

status curlh_send_http_request( Curl_h_t * curlh , buffer msg , long msg_sz , Brief_Err * imortalErrStr , bool bretry )
{
	INIT_BREAKABLE_FXN();
	CURLcode perform_iolvlres;

	BRIF_M_BREAK_IF( ( perform_iolvlres = curl_easy_setopt( curlh->curl , CURLOPT_POSTFIELDS , msg ) ) != CURLE_OK , errFault , 0 , pr_convert_curl_err_2_str( perform_iolvlres ) , imortalErrStr );
	BRIF_M_BREAK_IF( ( perform_iolvlres = curl_easy_setopt( curlh->curl , CURLOPT_POSTFIELDSIZE , msg_sz ) ) != CURLE_OK , errFault , 1 , pr_convert_curl_err_2_str( perform_iolvlres ) , imortalErrStr );
	perform_iolvlres = curl_easy_perform( curlh->curl );
	switch( perform_iolvlres )
	{
		case CURLE_OK:
		{
			long http_code = 0;
			CURLcode getinfo_res = curl_easy_getinfo( curlh->curl , CURLINFO_RESPONSE_CODE , &http_code );
			BRIF_M_BREAK_IF( getinfo_res != CURLE_OK , errFault , 1 , pr_convert_curl_err_2_str( getinfo_res ) , imortalErrStr );
			BREAK_STAT( http_result_err( http_code , imortalErrStr ) , 0 );
			break;
		}
		case CURLE_COULDNT_RESOLVE_HOST:
		case CURLE_COULDNT_CONNECT:
		case CURLE_OPERATION_TIMEDOUT:
		{
			switch ( perform_iolvlres )
			{
				case CURLE_COULDNT_RESOLVE_HOST:
				{
					STORE_INDIR_BRIEF_ERR( imortalErrStr , "wrong hostname, DNS problem" , 0 );
					break;
				}
				case CURLE_COULDNT_CONNECT:
				{
					STORE_INDIR_BRIEF_ERR( imortalErrStr , "server down, wrong port, firewall" , 0 );
					break;
				}
				case CURLE_OPERATION_TIMEDOUT:
				{
					STORE_INDIR_BRIEF_ERR( imortalErrStr , "slow network or server" , 0 );
					break;
				}
				default:;
			}
			
			// retry
			if ( bretry )
			{
				curl_easy_setopt( curlh->curl , CURLOPT_FRESH_CONNECT , 1L );
				d_error = curlh_send_http_request( curlh , msg , msg_sz , imortalErrStr , false );
				curl_easy_setopt( curlh->curl , CURLOPT_FRESH_CONNECT , 0L );
				return d_error;
			}
			break;
		}
		case CURLE_SEND_ERROR:
		case CURLE_RECV_ERROR:
		{
			switch ( perform_iolvlres )
			{
				case CURLE_SEND_ERROR:
				{
					STORE_INDIR_BRIEF_ERR( imortalErrStr , "connection closed, network error" , 0 );
					break;
				}
				case CURLE_RECV_ERROR:
				{
					STORE_INDIR_BRIEF_ERR( imortalErrStr , "server closed connection" , 0 );
					break;
				}
				default:;
			}
	
			curlh_disconnect( curlh->curl , imortalErrStr );
			//curlh->restart_needed = true; // Reconnect
			BREAK( errPeerClosed , 0 ); // most cause reconnection
			break;
		}
		case CURLE_SSL_CONNECT_ERROR:
		{
			STORE_INDIR_BRIEF_ERR( imortalErrStr , "bad certificate, TLS version mismatch" , 0 );
			break;
		}
		case CURLE_PEER_FAILED_VERIFICATION:
		{
			STORE_INDIR_BRIEF_ERR( imortalErrStr , "self-signed or invalid cert" , 0 );
			break;
		}
		case CURLE_HTTP_RETURNED_ERROR:
		{
			STORE_INDIR_BRIEF_ERR( imortalErrStr , "HTTP error code (>= 400)" , 0 );
			break;
		}
		case CURLE_OUT_OF_MEMORY:
		{
			STORE_INDIR_BRIEF_ERR( imortalErrStr , "system out of memory" , 0 );
			BREAK( errMemoryLow , 0 );
			break;
		}
		default:;
	}
	BRIF_M_BREAK_IF( perform_iolvlres != CURLE_OK , errFault , 1 , pr_convert_curl_err_2_str( perform_iolvlres ) , imortalErrStr );

	BEGIN_RET
	N_END_RET
}

status curlh_disconnect( Curl_h_t * curlh , Brief_Err * imortalErrStr )
{
	if ( !curlh ) return errArg;
	if ( curlh->devnull )
	{
		if ( fclose( curlh->devnull ) ) return errFault;
		curlh->devnull = NULL;
	}
	if ( curlh->curl_hdrs )
	{
		curl_slist_free_all( curlh->curl_hdrs );
		curlh->curl_hdrs = NULL;
	}
	if ( curlh->curl )
	{
		curl_easy_cleanup( curlh->curl );
		curlh->curl = NULL;
	}
	return errOK;
}
