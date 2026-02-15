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
	tcp->type = tcph_tcp;
	return errOK;
}

//status tcps_init_ssl_tcp( tcp_h_t * tcp , SSL_h_t * ssl )
//{
//	status d_error = tcps_init_tcp( tcp );
//	if ( !d_error )
//	{
//		if ( !( d_error = sslh_init( ssl ) ) )
//		{
//			tcp->ssl_over_tcp = ssl;
//			tcp->type = tcph_ssl;
//		}
//	}
//	return d_error;
//}

status tcps_init_curl( tcp_h_t * tcp , Curl_h_t * curl )
{
	status d_error = tcps_init_tcp( tcp );
	if ( !d_error )
	{
		if ( !( d_error = curlh_init( curl ) ) )
		{
			tcp->curl = curl;
			tcp->type = tcph_curl;
		}
	}
	return d_error;
}

void tcps_init_msg_ext_buf( tcp_h_t * tcp , ExternalErrBuf * buf , size_t max_buf_sz ) // if not used error message just ignored
{
	// TODO . check buffer size
	tcp->dts_buf = ( Detail_ErrBuf * )buf;
	//if ( tcp->ssl_over_tcp )
	//{
	//	sslh_init_msg_ext_buf( tcp->ssl_over_tcp , buf , max_buf_sz );
	//}
}

void tcps_init_msg_dts_buf( tcp_h_t * tcp , DtsErrBuf buf )
{
	tcp->dts_buf = buf;
	//if ( tcp->ssl_over_tcp )
	//{
	//	sslh_init_msg_dts_buf( tcp->ssl_over_tcp , buf );
	//}
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

//status tcps_accept_ssl_tcp_connection( tcp_h_t * tcp , LPCSTR ip , int port , int timeout_sec , LPCSTR server_certificate_path , LPCSTR server_key_path
//	, LPCSTR ca_crt_path , Brief_Err * imortalErrStr )
//{
//	status d_error;
//
//	// init ssl
//	//switch ( tcp->type )
//	//{
//	//	case tcph_ssl:
//	//	{
//	//		if ( ( d_error = sslh_serverside_pre_tcp_initialize_ctx( tcp->ssl_over_tcp , server_certificate_path , server_key_path , ca_crt_path , imortalErrStr ) ) )
//	//		{
//	//			STORE_BRIEF_ERR( imortalErrStr , "sslh_serverside_pre_tcp_initialize_ctx() failed.\n" , false );
//	//			//if ( tcp->dts_buf && tcp->ssl_over_tcp->dts_buf )
//	//			//{
//	//			//	strncpy( tcp->dts_buf , tcp->ssl_over_tcp->dts_buf );
//	//			//}
//	//			return d_error;
//	//		}
//	//		break;
//	//	}
//	//	default:;
//	//}
//
//	// try connect tcp
//	if ( ( d_error = tcps_accept_tcp_connection( tcp , ip , port , timeout_sec , imortalErrStr ) ) )
//	{
//		if ( tcp->ssl_over_tcp )
//		{
//			sslh_cleanup_ctx( tcp->ssl_over_tcp );
//		}
//		return d_error;
//	}
//
//	// try stablish ssl
//	//switch ( tcp->type )
//	//{
//	//	case tcph_ssl:
//	//	{
//	//		if ( ( d_error = sslh_serverside_post_tcp_ssl_handshake( tcp->ssl_over_tcp , tcp->tcp_fd , imortalErrStr ) ) )
//	//		{
//	//			STORE_BRIEF_ERR( imortalErrStr , "sslh_serverside_post_tcp_ssl_handshake() failed.\n" , false );
//	//			//if ( tcp->dts_buf && tcp->ssl_over_tcp->dts_buf )
//	//			//{
//	//			//	strncpy( tcp->dts_buf , tcp->ssl_over_tcp->dts_buf );
//	//			//}
//
//	//			_close_socket( &tcp->tcp_fd , imortalErrStr );
//	//			tcp->tcp_conn_established = 0;
//	//			sslh_cleanup_ctx( tcp->ssl_over_tcp );
//	//			return d_error;
//	//		}
//	//		break;
//	//	}
//	//	default:;
//	//}
//
//	return d_error;
//}

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

//status tcps_connect_2_ssl_server( tcp_h_t * tcp , LPCSTR ip , int port , int timeout_sec , LPCSTR server_certificate_path , LPCSTR server_key_path
//	, LPCSTR ca_crt_path , Brief_Err * imortalErrStr )
//{
//	status d_error;
//
//	// init ssl
//	//switch ( tcp->type )
//	//{
//	//	case tcph_ssl:
//	//	{
//	//		if ( ( d_error = sslh_clientside_pre_tcp_initialize_ctx( tcp->ssl_over_tcp , server_certificate_path , server_key_path , ca_crt_path , imortalErrStr ) ) )
//	//		{
//	//			STORE_BRIEF_ERR( imortalErrStr , "sslh_clientside_pre_tcp_initialize_ctx() failed.\n" , false );
//	//			//if ( tcp->dts_buf && tcp->ssl_over_tcp->dts_buf )
//	//			//{
//	//			//	strncpy( tcp->dts_buf , tcp->ssl_over_tcp->dts_buf );
//	//			//}
//	//			return d_error;
//	//		}
//	//		break;
//	//	}
//	//	default:;
//	//}
//
//	// try connect tcp
//	switch ( tcp->type )
//	{
//		//case tcph_ssl:
//		case tcph_tcp:
//		{
//			if ( ( d_error = tcps_connect_2_server( tcp , ip , port , timeout_sec , imortalErrStr ) ) )
//			{
//				if ( tcp->ssl_over_tcp )
//				{
//					sslh_cleanup_ctx( tcp->ssl_over_tcp );
//				}
//				return d_error;
//			}
//			break;
//		}
//		default:;
//	}
//
//	// try stablish ssl
//	//switch ( tcp->type )
//	//{
//	//	case tcph_ssl:
//	//	{
//	//		if ( ( d_error = sslh_clientside_post_tcp_ssl_handshake( tcp->ssl_over_tcp , tcp->tcp_fd , imortalErrStr ) ) )
//	//		{
//	//			STORE_BRIEF_ERR( imortalErrStr , "sslh_clientside_post_tcp_ssl_handshake() failed.\n" , false );
//	//			//if ( tcp->dts_buf && tcp->ssl_over_tcp->dts_buf )
//	//			//{
//	//			//	strncpy( tcp->dts_buf , tcp->ssl_over_tcp->dts_buf );
//	//			//}
//
//	//			_close_socket( &tcp->tcp_fd , imortalErrStr );
//	//			tcp->tcp_conn_established = 0;
//	//			sslh_cleanup_ctx( tcp->ssl_over_tcp );
//	//			return d_error;
//	//		}
//	//		break;
//	//	}
//	//	default:;
//	//}
//
//	return d_error;
//}
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

//status tcps_ssl_read( tcp_h_t * tcp , buffer buf , size_t buf_sz , int * num_reads , Brief_Err * imortalErrStr )
//{
//	if ( !tcp ) return errArg;
//	if ( !tcp->ssl_over_tcp ) return errFailure;
//	status ret_err = sslh_read( tcp->ssl_over_tcp , buf , ( int )buf_sz , num_reads , imortalErrStr );
//	switch ( ret_err )
//	{
//		case errPeerClosed:
//		{
//			tcps_close( tcp , imortalErrStr );
//			break;
//		}
//		default:;
//	}
//	return ret_err;
//}

status tcps_write( tcp_h_t * tcp , buffer buf , size_t buf_sz , int * num_writes/*nullable*/ , Brief_Err * imortalErrStr )
{
	if ( !tcp ) return errArg;

	switch ( tcp->type )
	{
		case tcph_curl:
		{
			status ret_err = errOK;
			if ( strlen( tcp->curl_setting.TCP_destination_ip ) )
			{
				ret_err = curlh_set_index_name( tcp->curl , tcp->curl_setting.TCP_destination_ip , tcp->curl_setting.TCP_destination_ports , tcp->curl_setting.elastic_doc_index_name , 
					tcp->curl_setting.elastic_insertion_protocol , tcp->curl_setting.elastic_insertion_cmd , imortalErrStr );
			}
			if ( !ret_err )
			{
				ret_err = curlh_send_http_request( tcp->curl , buf , ( long )buf_sz , imortalErrStr , true );
				switch ( ret_err )
				{
					case errPeerClosed:
					{
						tcps_close( tcp , imortalErrStr );
						break;
					}
				}
			}
			return ret_err;
		}
		//case tcph_ssl:
		//{
		//	status ret_err = sslh_write( tcp->ssl_over_tcp , buf , ( int )buf_sz , num_writes , imortalErrStr );
		//	switch ( ret_err )
		//	{
		//		case errPeerClosed:
		//		{
		//			tcps_close( tcp , imortalErrStr );
		//			break;
		//		}
		//	}
		//	return ret_err;
		//}
		case tcph_tcp:
		{
			if ( num_writes ) *num_writes = ( int )buf_sz;
			return tcp_send_all( tcp->tcp_fd , buf , buf_sz , 0 , SEND_TIMEOUT_ms , ACK_TIMEOUT_ms , RETRY_MECHANISM , imortalErrStr , tcp->dts_buf );
		}
		default:;
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

	if ( tcp->type == tcph_curl )
	{
		if ( tcp->curl )
		{
			curlh_disconnect( tcp->curl , imortalErrStr );
		}
		tcp->tcp_conn_established = 0;
		return;
	}

	//if ( tcp->ssl_over_tcp )
	//{
	//	switch ( tcp->type )
	//	{
	//		case tcph_ssl:
	//		{
	//			if ( sslh_is_handshaked( tcp->ssl_over_tcp ) )
	//			{
	//				sslh_cleanup_handshaked_ssl( tcp->ssl_over_tcp );
	//			}
	//			break;
	//		}
	//		default:;
	//	}
	//}

	if ( tcp->tcp_conn_established )
	{
		switch ( tcp->type )
		{
			//case tcph_ssl:
			case tcph_tcp:
			{
				_close_socket( &tcp->tcp_fd , imortalErrStr );
				break;
			}
			default:;
		}
		tcp->tcp_conn_established = 0;
	}

	//if ( tcp->ssl_over_tcp )
	//{
	//	switch ( tcp->type )
	//	{
	//		case tcph_ssl:
	//		{
	//			if ( sslh_has_context( tcp->ssl_over_tcp ) )
	//			{
	//				sslh_cleanup_ctx( tcp->ssl_over_tcp );
	//			}
	//			break;
	//		}
	//		default:;
	//	}
	//}
}

bool tcps_socket_is_accessible( tcp_h_t * tcp )
{
	if ( tcp && tcp->tcp_conn_established )
	{
		//if ( tcp->ssl_over_tcp )
		//{
		//	return sslh_is_handshaked( tcp->ssl_over_tcp );
		//}
		//else
		{
			return true;
		}
	}
	return false;
}

status tcps_connect_curl( tcp_h_t * tcp , LPCSTR elastic_username , LPCSTR elastic_pass , LPCSTR elastic_http_agt_CAuth_path ,
	LPCSTR elastic_listener_ip , int elastic_listener_port , LPCSTR elastic_doc_index_name , LPCSTR elastic_insertion_protocol , LPCSTR elastic_insertion_cmd ,
	Brief_Err * imortalErrStr )
{
	if ( !tcp ) return errArg;
	if ( !tcp->curl ) return errFailure;
	status ret_stat = curlh_connect_curl( tcp->curl , elastic_username , elastic_pass , elastic_http_agt_CAuth_path , elastic_listener_ip , elastic_listener_port ,
		elastic_doc_index_name , elastic_insertion_protocol , elastic_insertion_cmd , imortalErrStr );
	switch ( ret_stat )
	{
		case errOK:
		{
			tcp->tcp_conn_established = 1;

			strncpy( tcp->curl_setting.TCP_destination_ip , elastic_listener_ip , sizeof( tcp->curl_setting.TCP_destination_ip ) );
			tcp->curl_setting.TCP_destination_ports = elastic_listener_port;
			strncpy( tcp->curl_setting.elastic_doc_index_name , elastic_doc_index_name , sizeof( tcp->curl_setting.elastic_doc_index_name ) );
			strncpy( tcp->curl_setting.elastic_insertion_protocol , elastic_insertion_protocol , sizeof( tcp->curl_setting.elastic_insertion_protocol ) );
			strncpy( tcp->curl_setting.elastic_insertion_cmd , elastic_insertion_cmd , sizeof( tcp->curl_setting.elastic_insertion_cmd ) );

			break;
		}
		default:;
	}
	return ret_stat;
}
