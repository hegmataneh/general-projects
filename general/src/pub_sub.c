#define Uses_INIT_BREAKABLE_FXN
#define Uses_MEMSET_ZERO
#define Uses_pub_sub
#include <general.dep>

status distributor_init( distributor_t * d , int count )
{
	INIT_BREAKABLE_FXN();

	MEMSET_ZERO( d , 1 );
	M_MALLOC_AR( d->subs , count );
	d->sub_count = count;

	BEGIN_SMPL
	M_END_RET
}

status distributor_subscribe( distributor_t * d , sub_type_t type , sub_func_t func , void_p data )
{
	INIT_BREAKABLE_FXN();

	M_BREAK_IF( d->count >= d->sub_count , errMaximumExceeded , 0 );
	d->subs[ d->count ].type = type;
	d->subs[ d->count ].func = func;
	d->subs[ d->count ].data = data;
	d->count++;

	BEGIN_SMPL
	M_END_RET
}

// Publish different kinds of events
void distributor_publish_str( distributor_t * d , LPCSTR src_msg )
{
	for ( int i = 0; i < d->count; i++ )
	{
		if ( d->subs[ i ].type == SUB_STRING )
			d->subs[ i ].func.str_cb( d->subs[ i ].data , src_msg );
	}
}

void distributor_publish_int( distributor_t * d , int src_v )
{
	for ( int i = 0; i < d->count; i++ )
	{
		if ( d->subs[ i ].type == SUB_INT )
			d->subs[ i ].func.int_cb( d->subs[ i ].data , src_v );
	}
}

void distributor_publish_double( distributor_t * d , double src_v )
{
	for ( int i = 0; i < d->count; i++ )
	{
		if ( d->subs[ i ].type == SUB_DOUBLE )
			d->subs[ i ].func.dbl_cb( d->subs[ i ].data , src_v );
	}
}

void distributor_publish_int_double( distributor_t * dis , int src_i , double src_d )
{
	for ( int i = 0; i < dis->count; i++ )
	{
		if ( dis->subs[ i ].type == SUB_INT_DOUBLE )
			dis->subs[ i ].func.int_dbl_cb( dis->subs[ i ].data , src_i , src_d );
	}
}

void distributor_publish_str_double( distributor_t * dis , LPCSTR src_str , double src_d )
{
	for ( int i = 0; i < dis->count; i++ )
	{
		if ( dis->subs[ i ].type == SUB_STRING_DOUBLE )
			dis->subs[ i ].func.str_dbl_cb( dis->subs[ i ].data , src_str , src_d );
	}
}
