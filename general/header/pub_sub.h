#pragma once

#if defined Uses_pub_sub || !defined __COMPILING

/*
Simple in-process synchronous version
Synchronous publisher–subscriber (pub/sub)
*/

// Possible subscriber types
typedef enum
{
	SUB_STRING ,
	SUB_INT ,
	SUB_DOUBLE ,
	SUB_INT_DOUBLE ,
	SUB_STRING_DOUBLE ,
	SUB_DIRECT_ONE_CALL_BUFFER_INT ,
	SUB_DIRECT_MULTICAST_CALL_BUFFER_INT
} sub_type_t;

// Different callback signatures
typedef void ( *sub_string_t )( void_p data , LPCSTR msg );
typedef void ( *sub_int_t )( void_p data , int v );
typedef void ( *sub_double_t )( void_p data , double v );
typedef void ( *sub_int_double_t )( void_p data , int i , double d );
typedef void ( *sub_string_double_t )( void_p data , LPCSTR i , double d );
typedef status ( *sub_direct_one_call_buffer_int_t )( void_p data , buffer buf , int i );
typedef status ( *sub_multicast_call_buffer_int_t )( void_p data , buffer buf , int i );

// Union to hold any kind of callback
typedef union
{
	sub_string_t str_cb;
	sub_int_t    int_cb;
	sub_double_t dbl_cb;
	sub_int_double_t int_dbl_cb;
	sub_string_double_t str_dbl_cb;
	sub_direct_one_call_buffer_int_t direct_one_call_bfr_int_cb;
	sub_multicast_call_buffer_int_t multicast_call_buffer_int_cb;
} sub_func_t;

#define SUB_FXN( fxn ) ( sub_func_t )fxn

// One subscriber entry
typedef struct
{
	sub_type_t	type;
	sub_func_t	func;
	void_p		data;

	void_p tring_p_t;

} subscriber_t , *alloc_sub_t , **ar_alloc_sub_t;

// Distributor
typedef struct
{
	ar_alloc_sub_t * subs_grp; // each publish distribute to all
	int * subs_grp_subd; // each int is subscribed item in each sub grp
	int grp_count;

} distributor_t;

status distributor_init( distributor_t * d , int grp_count );
void destroy( distributor_t * d );

status distributor_subscribe_t( distributor_t * d , int iGrp , sub_type_t type , sub_func_t func , void_p data , subscriber_t ** subed /*=NULL*/ );
status distributor_subscribe( distributor_t * d , int iGrp , sub_type_t type , sub_func_t func , void_p data );
status distributor_subscribe_with_token( distributor_t * d , int iGrp , sub_type_t type , sub_func_t func , void_p data , void_p tring_p_t /*make it void_p to detach dependency*/ );

// Publish different kinds of events
void distributor_publish_str( distributor_t * d , LPCSTR src_msg  );
void distributor_publish_int( distributor_t * d , int src_v  );
void distributor_publish_double( distributor_t * d , double src_v  );
void distributor_publish_int_double( distributor_t * dis , int src_i , double src_d  );
void distributor_publish_str_double( distributor_t * dis , LPCSTR src_str , double src_d  );
status distributor_publish_buffer_int( distributor_t * dis , buffer src_buf , int src_i  );

#endif

