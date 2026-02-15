#pragma once

#if defined Uses_socket_handle || !defined __COMPILING

typedef enum
{
	tcph_invalid = 0,

	tcph_tcp ,
	//tcph_ssl ,
	tcph_curl
} e_tcp_socket_handle_type;

typedef struct s_tcp_socket_handle
{
	sockfd tcp_fd;
	int tcp_conn_established;
	e_tcp_socket_handle_type type;

	_EXTERNAL_VAR DtsErrBuf dts_buf;
	//_EXTERNAL_VAR SSL_h_t * ssl_over_tcp;
	_EXTERNAL_VAR Curl_h_t * curl;

	union
	{
		struct
		{
			char TCP_destination_ip[ DEFAULT_SFS_BUF_SZ ];
			int TCP_destination_ports;
			char elastic_doc_index_name[ DEFAULT_MFS_BUF_SZ ];
			char elastic_insertion_protocol[ DEFAULT_BT_BUF_SZ ];
			char elastic_insertion_cmd[ DEFAULT_SFS_BUF_SZ ];

		} curl_setting;

	};

} tcp_h_t; // ssl handle

status tcps_init_tcp( tcp_h_t * tcp ); // first of all, init every object
//status tcps_init_ssl_tcp( tcp_h_t * tcp , SSL_h_t * ssl ); // first of all, init every object
status tcps_init_curl( tcp_h_t * tcp , Curl_h_t * curl ); // first of all, init every object

void tcps_init_msg_ext_buf( tcp_h_t * tcp , ExternalErrBuf * buf , size_t max_buf_sz ); // if not used error message just ignored
void tcps_init_msg_dts_buf( tcp_h_t * tcp , DtsErrBuf buf ); // if not used error message just ignored . deter

void tcps_enable_keepalive( tcp_h_t * tcp , Brief_Err * imortalErrStr );
void tcps_close( tcp_h_t * tcp , Brief_Err * imortalErrStr );

bool tcps_socket_is_accessible( tcp_h_t * tcp );

status tcps_write( tcp_h_t * tcp , buffer buf , size_t buf_sz , int * num_writes , Brief_Err * imortalErrStr );

#ifndef tcp
	//status tcps_read( tcp_h_t * tcp , buffer buf , size_t buf_sz , int * num_reads , Brief_Err * imortalErrStr );

	#ifndef server_side
		status tcps_accept_tcp_connection( tcp_h_t * tcp , LPCSTR ip , int port , int timeout_sec , Brief_Err * imortalErrStr );
	#endif

	#ifndef client_side
		status tcps_connect_2_server( tcp_h_t * tcp , LPCSTR ip , int port , int timeout_sec , Brief_Err * imortalErrStr );
	#endif
#endif

//#ifndef ssl_over_tcp
//	status tcps_ssl_read( tcp_h_t * tcp , buffer buf , size_t buf_sz , int * num_reads , Brief_Err * imortalErrStr );
//
//	#ifndef server_side
//		status tcps_accept_ssl_tcp_connection( tcp_h_t * tcp , LPCSTR ip , int port , int timeout_sec , LPCSTR server_certificate_path ,
//			LPCSTR server_key_path , LPCSTR ca_crt_path , Brief_Err * imortalErrStr );
//	#endif
//
//	#ifndef client_side
//		status tcps_connect_2_ssl_server( tcp_h_t * tcp , LPCSTR ip , int port , int timeout_sec , LPCSTR server_certificate_path ,
//			LPCSTR server_key_path , LPCSTR ca_crt_path , Brief_Err * imortalErrStr );
//	#endif
//#endif

#ifndef curl
	#ifndef client_side
		status tcps_connect_curl( tcp_h_t * tcp , LPCSTR elastic_username , LPCSTR elastic_pass , LPCSTR elastic_http_agt_CAuth_path ,
			LPCSTR elastic_listener_ip , int elastic_listener_port , LPCSTR elastic_doc_index_name , LPCSTR elastic_insertion_protocol , LPCSTR elastic_insertion_cmd , Brief_Err * imortalErrStr );
	#endif
#endif

#endif
