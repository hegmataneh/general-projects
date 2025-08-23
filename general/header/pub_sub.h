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
	SUB_STRING_DOUBLE
} sub_type_t;

// Different callback signatures
typedef void ( *sub_string_t )( void_p data , LPCSTR msg );
typedef void ( *sub_int_t )( void_p data , int v );
typedef void ( *sub_double_t )( void_p data , double v );
typedef void ( *sub_int_double_t )( void_p data , int i , double d );
typedef void ( *sub_string_double_t )( void_p data , LPCSTR i , double d );

// Union to hold any kind of callback
typedef union
{
	sub_string_t str_cb;
	sub_int_t    int_cb;
	sub_double_t dbl_cb;
	sub_int_double_t int_dbl_cb;
	sub_string_double_t str_dbl_cb;
} sub_func_t;

#define SUB_FXN( fxn ) ( sub_func_t )fxn

// One subscriber entry
typedef struct
{
	sub_type_t	type;
	sub_func_t	func;
	void_p		data;
} subscriber_t;

// Distributor
typedef struct
{
	subscriber_t *subs;
	int sub_count;
	int count;
} distributor_t;

status distributor_init( distributor_t * d , int count );
status distributor_subscribe( distributor_t * d , sub_type_t type , sub_func_t func , void_p data );

// Publish different kinds of events
void distributor_publish_str( distributor_t * d , LPCSTR src_msg );
void distributor_publish_int( distributor_t * d , int src_v );
void distributor_publish_double( distributor_t * d , double src_v );
void distributor_publish_int_double( distributor_t * dis , int src_i , double src_d );
void distributor_publish_str_double( distributor_t * dis , LPCSTR src_str , double src_d );

#endif

