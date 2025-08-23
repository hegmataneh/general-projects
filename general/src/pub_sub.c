#define Uses_INIT_BREAKABLE_FXN
#define Uses_MEMSET_ZERO
#define Uses_pub_sub
#include <general.dep>

status distributor_init( distributor_t * d , int count )
{
	//INIT_BREAKABLE_FXN();
	//
	//MEMSET_ZERO( d , 1 );
	//M_MALLOC_AR( d->subs , 0 );
	//d->sub_count = 0;

	//BEGIN_SMPL
	//M_END_RET

	return errOK;
}

status distributor_subscribe( distributor_t * d , sub_type_t type , sub_func_t func )
{
	INIT_BREAKABLE_FXN();

	M_BREAK_IF( d->count >= d->sub_count , errMaximumExceeded , 0 );
	d->subs[ d->count ].type = type;
	d->subs[ d->count ].func = func;
	d->count++;

	//d_error=errOK; _ErrLvl=0; while( 0 ) { goto __ret; };  __ret: status ___localError=d_error; while(0) {

	//} d_error=___localError;

	//{
	//	
	//		M_showMsg( __custom_message );
	//}
	// return d_error;
	BEGIN_SMPL
	M_END_RET
}

// Publish different kinds of events
void distributor_publish_str( distributor_t * d , const char * msg )
{
	for ( int i = 0; i < d->count; i++ )
	{
		if ( d->subs[ i ].type == SUB_STRING )
			d->subs[ i ].func.str_cb( msg );
	}
}

void distributor_publish_int( distributor_t * d , int v )
{
	for ( int i = 0; i < d->count; i++ )
	{
		if ( d->subs[ i ].type == SUB_INT )
			d->subs[ i ].func.int_cb( v );
	}
}

void distributor_publish_double( distributor_t * d , double v )
{
	for ( int i = 0; i < d->count; i++ )
	{
		if ( d->subs[ i ].type == SUB_DOUBLE )
			d->subs[ i ].func.dbl_cb( v );
	}
}
