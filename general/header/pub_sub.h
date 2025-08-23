#pragma once

#if defined Uses_pub_sub || !defined __COMPILING

// Possible subscriber types
typedef enum
{
	SUB_STRING ,
	SUB_INT ,
	SUB_DOUBLE
} sub_type_t;

// Different callback signatures
typedef void ( *sub_string_t )( const char * msg );
typedef void ( *sub_int_t )( int v );
typedef void ( *sub_double_t )( double v );

// Union to hold any kind of callback
typedef union
{
	sub_string_t str_cb;
	sub_int_t    int_cb;
	sub_double_t dbl_cb;
} sub_func_t;

// One subscriber entry
typedef struct
{
	sub_type_t type;
	sub_func_t func;
} subscriber_t;

// Distributor
typedef struct
{
	subscriber_t *subs;
	int sub_count;
	int count;
} distributor_t;

status distributor_init( distributor_t * d , int count );
status distributor_subscribe( distributor_t * d , sub_type_t type , sub_func_t func );

// Publish different kinds of events
void distributor_publish_str( distributor_t * d , const char * msg );
void distributor_publish_int( distributor_t * d , int v );
void distributor_publish_double( distributor_t * d , double v );

#endif

