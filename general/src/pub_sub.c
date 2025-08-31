#define Uses_token_ring_p_t
#define Uses_INIT_BREAKABLE_FXN
#define Uses_MEMSET_ZERO
#define Uses_pub_sub
#include <general.dep>

status distributor_init( distributor_t * dis , int grp_count )
{
	INIT_BREAKABLE_FXN();

	MEMSET_ZERO_O( dis );

	M_MALLOC_AR( dis->subs_grp , grp_count , 0 );
	MEMSET_ZERO( dis->subs_grp , grp_count );

	M_MALLOC_AR( dis->subs_grp_subd , grp_count , 1 );
	MEMSET_ZERO( dis->subs_grp_subd , grp_count );

	dis->grp_count = grp_count;

	BEGIN_SMPL
	M_END_RET
}

void destroy( distributor_t * dis )
{
	if ( !dis ) return;
	for ( int igrp = 0 ; igrp < dis->grp_count ; igrp++ )
	{
		for ( int ilist = 0 ; ilist < dis->subs_grp_subd[ igrp ] ; ilist++ )
		{
			FREE( dis->subs_grp[ igrp ][ ilist ] );
		}
		FREE(  dis->subs_grp[ igrp ] );
	}
	DAC( dis->subs_grp_subd );
	DAC( dis->subs_grp );
}

status distributor_subscribe_t( distributor_t * dis , int iGrp /*1 on flat list*/ , sub_type_t type , sub_func_t func , pass_p data , subscriber_t ** subed )
{
	INIT_BREAKABLE_FXN();

	M_BREAK_IF( iGrp >= dis->grp_count , errMaximumExceeded , 0 );

	dis->subs_grp[ iGrp ] = ( ar_alloc_sub_t )REALLOC_AR( ( ar_alloc_sub_t )dis->subs_grp[ iGrp ] , dis->subs_grp_subd[ iGrp ] + 1 );
	M_BREAK_IF( !dis->subs_grp[ iGrp ] , errMemoryLow , 0 );
	MEMSET_ZERO( dis->subs_grp[ iGrp ] + dis->subs_grp_subd[ iGrp ] , 1 );

	M_MALLOC_ONE( dis->subs_grp[ iGrp ][ dis->subs_grp_subd[ iGrp ] ] , 0 );
	MEMSET_ZERO_O( dis->subs_grp[ iGrp ][ dis->subs_grp_subd[ iGrp ] ] );

	dis->subs_grp[ iGrp ][ dis->subs_grp_subd[ iGrp ] ]->type = type;
	dis->subs_grp[ iGrp ][ dis->subs_grp_subd[ iGrp ] ]->func = func;
	dis->subs_grp[ iGrp ][ dis->subs_grp_subd[ iGrp ] ]->data = data;
	if ( subed )
	{
		*subed = dis->subs_grp[ iGrp ][ dis->subs_grp_subd[ iGrp ] ];
	}
	dis->subs_grp_subd[ iGrp ]++;

	BEGIN_SMPL
	M_END_RET
}

status distributor_subscribe( distributor_t * dis , sub_type_t type , sub_func_t func , pass_p data )
{
	return distributor_subscribe_t( dis , 0 , type , func , data , NULL );
}

status distributor_subscribe_ingrp( distributor_t * dis , int iGrp /*1 on flat list*/ , sub_type_t type , sub_func_t func , pass_p data )
{
	return distributor_subscribe_t( dis , iGrp , type , func , data , NULL );
}

status distributor_subscribe_with_ring( distributor_t * dis , int iGrp /*1 on flat list*/ , sub_type_t type , sub_func_t func , pass_p data , void_p src_tring )
{
	INIT_BREAKABLE_FXN();

	token_ring_p_t * tring = ( token_ring_p_t * )src_tring;

	subscriber_t * subed = NULL;
	BREAK_STAT( distributor_subscribe_t( dis , iGrp , type , func , data , &subed ) , 0 );
	subed->tring_p_t = tring; 
	BREAK_STAT( token_ring_p_add( tring , subed ) , 0 );

	BEGIN_SMPL
	M_END_RET
}

status distributor_subscribe_onedirectcall( distributor_t * dis , sub_type_t type , sub_func_t func , void_p token , pass_p data )
{
	INIT_BREAKABLE_FXN();

	subscriber_t * subed = NULL;
	BREAK_STAT( distributor_subscribe_t( dis , 0 , type , func , data , &subed ) , 0 );
	subed->token = token;

	BEGIN_SMPL
	M_END_RET
}

// Publish different kinds of events
void distributor_publish_str( distributor_t * dis , LPCSTR src_msg , pass_p data /*=NULL*/ )
{
	for ( int igrp = 0; igrp < dis->grp_count ; igrp++ )
	{
		int one_token_ring_called = 0;
		for ( int isub = 0 ; isub < dis->subs_grp_subd[ igrp ] ; isub++ )
		{
			if ( dis->subs_grp[ igrp ][ isub ]->type == SUB_STRING )
			{
				if ( dis->subs_grp[ igrp ][ isub ]->tring_p_t )
				{
					if ( !one_token_ring_called )
					{
						token_ring_p_t * tring = ( token_ring_p_t * )dis->subs_grp[ igrp ][ isub ]->tring_p_t;
						void_p turn_key;
						token_ring_p_curr( tring , &turn_key );
						if ( turn_key == ( void_p )dis->subs_grp[ igrp ][ isub ] )
						{
							token_ring_p_next( tring , &turn_key );
							one_token_ring_called = 1;
							dis->subs_grp[ igrp ][ isub ]->func.str_cb( ISNULL( data , dis->subs_grp[ igrp ][ isub ]->data ) , src_msg );	
							break;
						}
					}
				}
				else
				{
					dis->subs_grp[ igrp ][ isub ]->func.str_cb( ISNULL( data , dis->subs_grp[ igrp ][ isub ]->data ) , src_msg );
				}
			}
		}
	}
}

void distributor_publish_int( distributor_t * dis , int src_v , pass_p data /*=NULL*/ )
{
	for ( int igrp = 0; igrp < dis->grp_count ; igrp++ )
	{
		int one_token_ring_called = 0;
		for ( int isub = 0 ; isub < dis->subs_grp_subd[ igrp ] ; isub++ )
		{
			if ( dis->subs_grp[ igrp ][ isub ]->type == SUB_INT )
			{
				if ( dis->subs_grp[ igrp ][ isub ]->tring_p_t )
				{
					if ( !one_token_ring_called )
					{
						token_ring_p_t * tring = ( token_ring_p_t * )dis->subs_grp[ igrp ][ isub ]->tring_p_t;
						void_p turn_key;
						token_ring_p_curr( tring , &turn_key );
						if ( turn_key == ( void_p )dis->subs_grp[ igrp ][ isub ] )
						{
							token_ring_p_next( tring , &turn_key );
							one_token_ring_called = 1;
							dis->subs_grp[ igrp ][ isub ]->func.int_cb( ISNULL( data , dis->subs_grp[ igrp ][ isub ]->data ) , src_v );
							break;
						}
					}
				}
				else
				{
					dis->subs_grp[ igrp ][ isub ]->func.int_cb( ISNULL( data , dis->subs_grp[ igrp ][ isub ]->data ) , src_v );
				}
			}
		}
	}
}

void distributor_publish_double( distributor_t * dis , double src_v , pass_p data /*=NULL*/ )
{
	for ( int igrp = 0; igrp < dis->grp_count ; igrp++ )
	{
		int one_token_ring_called = 0;
		for ( int isub = 0 ; isub < dis->subs_grp_subd[ igrp ] ; isub++ )
		{
			if ( dis->subs_grp[ igrp ][ isub ]->type == SUB_DOUBLE )
			{
				if ( dis->subs_grp[ igrp ][ isub ]->tring_p_t )
				{
					if ( !one_token_ring_called )
					{
						token_ring_p_t * tring = ( token_ring_p_t * )dis->subs_grp[ igrp ][ isub ]->tring_p_t;
						void_p turn_key;
						token_ring_p_curr( tring , &turn_key );
						if ( turn_key == ( void_p )dis->subs_grp[ igrp ][ isub ] )
						{
							token_ring_p_next( tring , &turn_key );
							one_token_ring_called = 1;
							dis->subs_grp[ igrp ][ isub ]->func.dbl_cb( ISNULL( data , dis->subs_grp[ igrp ][ isub ]->data ) , src_v );
							break;
						}
					}
				}
				else
				{
					dis->subs_grp[ igrp ][ isub ]->func.dbl_cb( ISNULL( data , dis->subs_grp[ igrp ][ isub ]->data ) , src_v );
				}
			}
		}
	}
}

void distributor_publish_int_double( distributor_t * dis , int src_i , double src_d , pass_p data /*=NULL*/ )
{
	for ( int igrp = 0; igrp < dis->grp_count ; igrp++ )
	{
		int one_token_ring_called = 0;
		for ( int isub = 0 ; isub < dis->subs_grp_subd[ igrp ] ; isub++ )
		{
			if ( dis->subs_grp[ igrp ][ isub ]->type == SUB_INT_DOUBLE )
			{
				if ( dis->subs_grp[ igrp ][ isub ]->tring_p_t )
				{
					if ( !one_token_ring_called )
					{
						token_ring_p_t * tring = ( token_ring_p_t * )dis->subs_grp[ igrp ][ isub ]->tring_p_t;
						void_p turn_key;
						token_ring_p_curr( tring , &turn_key );
						if ( turn_key == ( void_p )dis->subs_grp[ igrp ][ isub ] )
						{
							token_ring_p_next( tring , &turn_key );
							one_token_ring_called = 1;
							dis->subs_grp[ igrp ][ isub ]->func.int_dbl_cb( ISNULL( data , dis->subs_grp[ igrp ][ isub ]->data ) , src_i , src_d );
							break;
						}
					}
				}
				else
				{
					dis->subs_grp[ igrp ][ isub ]->func.int_dbl_cb( ISNULL( data , dis->subs_grp[ igrp ][ isub ]->data ) , src_i , src_d );
				}
			}
		}
	}
}

void distributor_publish_str_double( distributor_t * dis , LPCSTR src_str , double src_d , pass_p data /*=NULL*/ )
{
	for ( int igrp = 0; igrp < dis->grp_count ; igrp++ )
	{
		int one_token_ring_called = 0;
		for ( int isub = 0 ; isub < dis->subs_grp_subd[ igrp ] ; isub++ )
		{
			if ( dis->subs_grp[ igrp ][ isub ]->type == SUB_STRING_DOUBLE )
			{
				if ( dis->subs_grp[ igrp ][ isub ]->tring_p_t )
				{
					if ( !one_token_ring_called )
					{
						token_ring_p_t * tring = ( token_ring_p_t * )dis->subs_grp[ igrp ][ isub ]->tring_p_t;
						void_p turn_key;
						token_ring_p_curr( tring , &turn_key );
						if ( turn_key == ( void_p )dis->subs_grp[ igrp ][ isub ] )
						{
							token_ring_p_next( tring , &turn_key );
							one_token_ring_called = 1;
							dis->subs_grp[ igrp ][ isub ]->func.str_dbl_cb( ISNULL( data , dis->subs_grp[ igrp ][ isub ]->data ) , src_str , src_d );
							break;
						}
					}
				}
				else
				{
					dis->subs_grp[ igrp ][ isub ]->func.str_dbl_cb( ISNULL( data , dis->subs_grp[ igrp ][ isub ]->data ) , src_str , src_d );
				}
			}
		}
	}
}

status distributor_publish_buffer_int( distributor_t * dis , buffer src_buf , int src_i , pass_p data /*=NULL*/ )
{
	status aggr_ret = errOK;
	int any_call_happend = 0;
	for ( int igrp = 0; igrp < dis->grp_count ; igrp++ )
	{
		int one_token_ring_called = 0;
		for ( int isub = 0 ; isub < dis->subs_grp_subd[ igrp ] ; isub++ )
		{
			if ( dis->subs_grp[ igrp ][ isub ]->type == SUB_DIRECT_ONE_CALL_BUFFER_INT )
			{
				if ( dis->subs_grp[ igrp ][ isub ]->tring_p_t )
				{
					if ( !one_token_ring_called )
					{
						token_ring_p_t * tring = ( token_ring_p_t * )dis->subs_grp[ igrp ][ isub ]->tring_p_t;
						void_p turn_key = NULL;
						token_ring_p_curr( tring , &turn_key );
						if ( turn_key == ( void_p )dis->subs_grp[ igrp ][ isub ] )
						{
							token_ring_p_next( tring , &turn_key );
							one_token_ring_called = 1;
							return dis->subs_grp[ igrp ][ isub ]->func.direct_one_call_bfr_int_cb( ISNULL( data , dis->subs_grp[ igrp ][ isub ]->data ) , src_buf , src_i );
						}
					}
				}
				else
				{
					return dis->subs_grp[ igrp ][ isub ]->func.direct_one_call_bfr_int_cb( ISNULL( data , dis->subs_grp[ igrp ][ isub ]->data ) , src_buf , src_i );
				}
			}
			else if ( dis->subs_grp[ igrp ][ isub ]->type == SUB_DIRECT_MULTICAST_CALL_BUFFER_INT )
			{
				if ( dis->subs_grp[ igrp ][ isub ]->tring_p_t )
				{
					if ( !one_token_ring_called )
					{
						token_ring_p_t * tring = ( token_ring_p_t * )dis->subs_grp[ igrp ][ isub ]->tring_p_t;
						void_p turn_key = NULL;
						token_ring_p_curr( tring , &turn_key );
						if ( turn_key == ( void_p )dis->subs_grp[ igrp ][ isub ] )
						{
							token_ring_p_next( tring , &turn_key );
							one_token_ring_called = 1;
							any_call_happend = 1;
							aggr_ret |= dis->subs_grp[ igrp ][ isub ]->func.multicast_call_buffer_int_cb( ISNULL( data , dis->subs_grp[ igrp ][ isub ]->data ) , src_buf , src_i );
						}
					}
				}
				else
				{
					any_call_happend = 1;
					aggr_ret |= dis->subs_grp[ igrp ][ isub ]->func.multicast_call_buffer_int_cb( ISNULL( data , dis->subs_grp[ igrp ][ isub ]->data ) , src_buf , src_i );
				}
			}
		}
	}
	return any_call_happend ? aggr_ret : errNoPeer;
}

status distributor_publish_onedirectcall_voidp( distributor_t * dis , void_p ptr /*caller pointer*/ ,
	void_p token /*token that spec calle*/ , pass_p data /*=NULL custom per call data or per subscriber_t*/ )
{
	for ( int igrp = 0; igrp < dis->grp_count ; igrp++ )
	{
		for ( int isub = 0 ; isub < dis->subs_grp_subd[ igrp ] ; isub++ )
		{
			if
			(
				dis->subs_grp[ igrp ][ isub ]->type == SUB_DIRECT_ONE_CALL_VOIDP &&
				dis->subs_grp[ igrp ][ isub ]->token == token
			)
			{
				return dis->subs_grp[ igrp ][ isub ]->func.direct_one_call_voidp_cb( ISNULL( data , dis->subs_grp[ igrp ][ isub ]->data ) , ptr );
			}
		}
	}
	return errNoPeer;
}
