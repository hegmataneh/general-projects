#pragma once

#if defined Uses_Curl_handle || !defined __COMPILING

typedef struct s_Curl_handle
{
	CURL *curl;
	struct curl_slist *curl_hdrs;
	FILE *devnull;

	DtsErrBuf dts_buf;
	//bool restart_needed;

} Curl_h_t; // ssl handle

status curlh_init( Curl_h_t * curlh );

status curlh_connect_curl( Curl_h_t * curlh , Brief_Err * imortalErrStr );

status curlh_send_http_request( Curl_h_t * curlh , buffer msg , long msg_sz , Brief_Err * imortalErrStr , bool bretry );

status curlh_disconnect( Curl_h_t * curlh , Brief_Err * imortalErrStr );

#endif

