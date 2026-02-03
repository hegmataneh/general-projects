#pragma once

#if defined Uses_SSL_handle || !defined __COMPILING

typedef struct s_SSL_handle
{
	SSL_CTX * ssl_ctx; // SSL context
	SSL * ssl_obj;
	
	DtsErrBuf dts_buf;

	bool is_handshaked;

} SSL_h_t; // ssl handle


status sslh_init( SSL_h_t * ssl ); // first of all init every object

void sslh_init_msg_ext_buf( SSL_h_t * ssl , ExternalErrBuf * buf , size_t max_buf_sz ); // if not used error message just ignored
void sslh_init_msg_dts_buf( SSL_h_t * ssl , DtsErrBuf buf ); // if not used error message just ignored

#ifndef server_side
status sslh_serverside_pre_tcp_initialize_ctx( SSL_h_t * ssl , LPCSTR server_certificate_path , LPCSTR server_key_path , LPCSTR ca_crt_path , Brief_Err * imortalErrStr );
status sslh_serverside_post_tcp_ssl_handshake( SSL_h_t * ssl , sockfd client_sock_id , Brief_Err * imortalErrStr );
#endif

#ifndef client_side
status sslh_clientside_pre_tcp_initialize_ctx( SSL_h_t * ssl , LPCSTR server_certificate_path , LPCSTR server_key_path , LPCSTR ca_crt_path , Brief_Err * imortalErrStr );
status sslh_clientside_post_tcp_ssl_handshake( SSL_h_t * ssl , sockfd client_sock_id , Brief_Err * imortalErrStr );
#endif

bool sslh_is_handshaked( SSL_h_t * ssl );
bool sslh_has_context( SSL_h_t * ssl );

//void sslh_cleanup( SSL_h_t * ssl );
void sslh_cleanup_handshaked_ssl( SSL_h_t * ssl );
void sslh_cleanup_ssl_obj( SSL_h_t * ssl );
void sslh_cleanup_ctx( SSL_h_t * ssl );

status sslh_read( SSL_h_t * ssl , buffer buf , int buf_sz , int * num_reads , Brief_Err * imortalErrStr );
status sslh_write( SSL_h_t * ssl , buffer buf , int buf_sz , int * byte_writes , Brief_Err * imortalErrStr );

#endif

