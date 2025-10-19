#pragma once

#if defined Uses_pub_sub || !defined __COMPILING

/*
Simple in-process synchronous version
Synchronous publisher–subscriber (pub/sub)
*/

// Possible subscriber types
typedef enum
{
	SUB_VOID ,
	SUB_STRING ,
	SUB_LONG ,
	SUB_DOUBLE ,
	SUB_LONG_DOUBLE ,
	SUB_STRING_DOUBLE ,
	SUB_DIRECT_ONE_CALL_BUFFER_SIZE ,
	SUB_DIRECT_MULTICAST_CALL_BUFFER_SIZE ,
	SUB_DIRECT_ONE_CALL_VOIDP ,
	SUB_DIRECT_ONE_CALL_3VOIDP
} sub_type_t;

// Different callback signatures
typedef void ( *sub_void_t )( pass_p data );
typedef void ( *sub_string_t )( pass_p data , LPCSTR msg );
typedef void ( *sub_long_t )( pass_p data , long v );
typedef void ( *sub_double_t )( pass_p data , double v );
typedef void ( *sub_long_double_t )( pass_p data , long i , double d );
typedef void ( *sub_string_double_t )( pass_p data , LPCSTR i , double d );
typedef status ( *sub_direct_one_call_buffer_size_t )( pass_p data , buffer buf , size_t sz );
typedef status ( *sub_multicast_call_buffer_size_t )( pass_p data , buffer buf , size_t sz );
typedef status ( *sub_direct_one_call_voidp_t )( pass_p data , void_p ptr );
typedef status ( *sub_direct_one_call_3voidp_t )( void_p ptr1 , void_p ptr2 , void_p ptr3 );

// Union to hold any kind of callback
typedef union
{
	sub_void_t								void_cb;
	sub_string_t							str_cb;
	sub_long_t								long_cb;
	sub_double_t							dbl_cb;
	sub_long_double_t						long_dbl_cb;
	sub_string_double_t						str_dbl_cb;
	sub_direct_one_call_buffer_size_t		direct_one_call_bfr_size_cb;
	sub_multicast_call_buffer_size_t		multicast_call_buffer_size_cb;
	sub_direct_one_call_voidp_t				direct_one_call_voidp_cb;
	sub_direct_one_call_3voidp_t			direct_one_call_3voidp_cb;
} sub_func_t;

#define SUB_FXN( fxn ) ( sub_func_t )fxn

// One subscriber entry
typedef struct
{
	sub_type_t	type;
	int pad1;
	sub_func_t	func;
	pass_p		data;
	void_p		token;

	void_p		tring_p_t; // used in round robin

} subscriber_t;

typedef struct
{
	dyn_mms_arr	subs; // subscriber_t

} subscribers_t;

// Distributor
typedef struct
{
	dyn_mms_arr grps; // subscribers_t
	e_direction iteration_dir;
	dyn_mms_arr * pheap;

} distributor_t;

typedef struct
{
	int order;
	subscriber_t * psubscriber;
} sub_custome_ord_t;

#define SUBSCRIBER_PROVIDED NULL

status distributor_init( distributor_t * dis , size_t grp_count );
status distributor_init_withOrder( distributor_t * dis , size_t grp_count );
void sub_destroy( distributor_t * dis );

status distributor_subscribe_t( distributor_t * dis , size_t iGrp /*1 on flat list*/ , sub_type_t type , sub_func_t func , pass_p data , OUTcpy subscriber_t ** subed /*=NULL*/ , int * order /*if not null then this is order*/ );

status distributor_subscribe( distributor_t * dis , sub_type_t type , sub_func_t func , pass_p data );
status distributor_subscribe_ingrp( distributor_t * dis , size_t iGrp /*1 on flat list*/ , sub_type_t type , sub_func_t func , pass_p data);
status distributor_subscribe_with_ring( distributor_t * dis , size_t iGrp /*1 on flat list*/ , sub_type_t type , sub_func_t func , pass_p data , void_p tring_p_t /*make it void_p to detach dependency*/ );

status distributor_subscribe_withOrder( distributor_t * dis , sub_type_t type , sub_func_t func , pass_p data , int order/*greater priority go first*/ );

status distributor_subscribe_onedirectcall( distributor_t * dis , sub_type_t type , sub_func_t func , void_p token , pass_p data ); // only callback with same token call

// Publish different kinds of events
status distributor_publish_void( distributor_t * dis , pass_p data /*=NULL if subscriber precede*/ );
status distributor_publish_str( distributor_t * dis , LPCSTR src_msg , pass_p data /*=NULL if subscriber precede*/ );
status distributor_publish_long( distributor_t * dis , long src_v , pass_p data /*=NULL if subscriber precede*/ );
status distributor_publish_double( distributor_t * dis , double src_v , pass_p data /*=NULL if subscriber precede*/ );
status distributor_publish_long_double( distributor_t * dis , long src_i , double src_d , pass_p data /*=NULL if subscriber precede*/ );
status distributor_publish_str_double( distributor_t * dis , LPCSTR src_str , double src_d , pass_p data /*=NULL if subscriber precede*/ );
status distributor_publish_buffer_size( distributor_t * dis , buffer src_buf , size_t src_sz , pass_p data /*=NULL if subscriber precede*/ );

status distributor_publish_onedirectcall_voidp( distributor_t * dis , void_p ptr /*caller pointer*/ ,
	void_p token /*token that spec calle*/ , pass_p data /*=NULL custom per call data or per subscriber_t*/ );

status distributor_publish_voidp( distributor_t * dis , void_p ptr /*caller pointer*/ , pass_p data /*=NULL custom per call data or per subscriber_t*/ );

status distributor_publish_onedirectcall_3voidp( distributor_t * dis , void_p ptr1 , void_p ptr2 , void_p ptr3 );

status distributor_get_data( distributor_t * dis , pass_p * pdata );

#endif

