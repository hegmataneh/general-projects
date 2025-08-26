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

status distributor_subscribe_t( distributor_t * dis , int iGrp , sub_type_t type , sub_func_t func , void_p data , subscriber_t ** subed )
{
	INIT_BREAKABLE_FXN();

	M_BREAK_IF( iGrp >= dis->grp_count , errMaximumExceeded , 0 );

	dis->subs_grp[ iGrp ] = REALLOC_AR( dis->subs_grp[ iGrp ] , dis->subs_grp_subd[ iGrp ] + 1 );
	M_BREAK_IF( !dis->subs_grp[ iGrp ] , errMemoryLow , 0 );

	dis->subs_grp[ iGrp ][ dis->subs_grp_subd[ iGrp ] ].type = type;
	dis->subs_grp[ iGrp ][ dis->subs_grp_subd[ iGrp ] ].func = func;
	dis->subs_grp[ iGrp ][ dis->subs_grp_subd[ iGrp ] ].data = data;
	dis->subs_grp_subd[ iGrp ]++;
	subed = &dis->subs_grp[ iGrp ];

	BEGIN_SMPL
	M_END_RET
}

status distributor_subscribe( distributor_t * dis , int iGrp , sub_type_t type , sub_func_t func , void_p data )
{
	subscriber_t * subed;
	return distributor_subscribe_t( dis , iGrp , type , func , data , &subed );
}

status distributor_subscribe_with_token( distributor_t * dis , int iGrp , sub_type_t type , sub_func_t func , void_p data , void_p tring_p_t )
{
	INIT_BREAKABLE_FXN();

	token_ring_p_t * tring = ( token_ring_p_t * )tring_p_t;

	subscriber_t * subed;
	BREAK_STAT( distributor_subscribe_t( dis , iGrp , type , func , data , &subed ) , 0 );
	subed->tring_p_t = tring; 
	BREAK_STAT( token_ring_p_add( tring , subed ) , 0 );

	BEGIN_SMPL
	M_END_RET
}

// Publish different kinds of events
void distributor_publish_str( distributor_t * dis , LPCSTR src_msg )
{
	for ( int i = 0; i < dis->grp_count ; i++ )
	{
		for ( int j = 0 ; j < dis->subs_grp_subd[ i ] ; j++ )
		{
			if ( dis->subs_grp[ i ][ j ].type == SUB_STRING )
			{
				if ( dis->subs_grp[ i ][ j ].tring_p_t )
				{
					token_ring_p_t * tring = ( token_ring_p_t * )dis->subs_grp[ i ][ j ].tring_p_t;
					void_p turn_key;
					token_ring_p_next( tring , &turn_key );
					if ( turn_key == ( void_p )&dis->subs_grp[ i ][ j ] )
					{
						dis->subs_grp[ i ][ j ].func.str_cb( dis->subs_grp[ i ][ j ].data , src_msg );	
						break;
					}
				}
				else
				{
					dis->subs_grp[ i ][ j ].func.str_cb( dis->subs_grp[ i ][ j ].data , src_msg );
				}
			}
		}
	}
}

void distributor_publish_int( distributor_t * dis , int src_v )
{
	for ( int i = 0; i < dis->grp_count ; i++ )
	{
		for ( int j = 0 ; j < dis->subs_grp_subd[ i ] ; j++ )
		{
			if ( dis->subs_grp[ i ][ j ].type == SUB_INT )
			{
				if ( dis->subs_grp[ i ][ j ].tring_p_t )
				{
					token_ring_p_t * tring = ( token_ring_p_t * )dis->subs_grp[ i ][ j ].tring_p_t;
					void_p turn_key;
					token_ring_p_next( tring , &turn_key );
					if ( turn_key == ( void_p )&dis->subs_grp[ i ][ j ] )
					{
						dis->subs_grp[ i ][ j ].func.int_cb( dis->subs_grp[ i ][ j ].data , src_v );
						break;
					}
				}
				else
				{
					dis->subs_grp[ i ][ j ].func.int_cb( dis->subs_grp[ i ][ j ].data , src_v );
				}
			}
		}
	}
}

void distributor_publish_double( distributor_t * dis , double src_v  )
{
	for ( int i = 0; i < dis->grp_count ; i++ )
	{
		for ( int j = 0 ; j < dis->subs_grp_subd[ i ] ; j++ )
		{
			if ( dis->subs_grp[ i ][ j ].type == SUB_DOUBLE )
			{
				if ( dis->subs_grp[ i ][ j ].tring_p_t )
				{
					token_ring_p_t * tring = ( token_ring_p_t * )dis->subs_grp[ i ][ j ].tring_p_t;
					void_p turn_key;
					token_ring_p_next( tring , &turn_key );
					if ( turn_key == ( void_p )&dis->subs_grp[ i ][ j ] )
					{
						dis->subs_grp[ i ][ j ].func.dbl_cb( dis->subs_grp[ i ][ j ].data , src_v );
						break;
					}
				}
				else
				{
					dis->subs_grp[ i ][ j ].func.dbl_cb( dis->subs_grp[ i ][ j ].data , src_v );
				}
			}
		}
	}
}

void distributor_publish_int_double( distributor_t * dis , int src_i , double src_d  )
{
	for ( int i = 0; i < dis->grp_count ; i++ )
	{
		for ( int j = 0 ; j < dis->subs_grp_subd[ i ] ; j++ )
		{
			if ( dis->subs_grp[ i ][ j ].type == SUB_INT_DOUBLE )
			{
				if ( dis->subs_grp[ i ][ j ].tring_p_t )
				{
					token_ring_p_t * tring = ( token_ring_p_t * )dis->subs_grp[ i ][ j ].tring_p_t;
					void_p turn_key;
					token_ring_p_next( tring , &turn_key );
					if ( turn_key == ( void_p )&dis->subs_grp[ i ][ j ] )
					{
						dis->subs_grp[ i ][ j ].func.int_dbl_cb( dis->subs_grp[ i ][ j ].data , src_i , src_d );
						break;
					}
				}
				else
				{
					dis->subs_grp[ i ][ j ].func.int_dbl_cb( dis->subs_grp[ i ][ j ].data , src_i , src_d );
				}
			}
		}
	}
}

void distributor_publish_str_double( distributor_t * dis , LPCSTR src_str , double src_d  )
{
	for ( int i = 0; i < dis->grp_count ; i++ )
	{
		for ( int j = 0 ; j < dis->subs_grp_subd[ i ] ; j++ )
		{
			if ( dis->subs_grp[ i ][ j ].type == SUB_STRING_DOUBLE )
			{
				if ( dis->subs_grp[ i ][ j ].tring_p_t )
				{
					token_ring_p_t * tring = ( token_ring_p_t * )dis->subs_grp[ i ][ j ].tring_p_t;
					void_p turn_key;
					token_ring_p_next( tring , &turn_key );
					if ( turn_key == ( void_p )&dis->subs_grp[ i ][ j ] )
					{
						dis->subs_grp[ i ][ j ].func.str_dbl_cb( dis->subs_grp[ i ][ j ].data , src_str , src_d );
						break;
					}
				}
				else
				{
					dis->subs_grp[ i ][ j ].func.str_dbl_cb( dis->subs_grp[ i ][ j ].data , src_str , src_d );
				}
			}
		}
	}
}

status distributor_publish_buffer_int( distributor_t * dis , buffer src_buf , int src_i )
{
	for ( int i = 0; i < dis->grp_count ; i++ )
	{
		for ( int j = 0 ; j < dis->subs_grp_subd[ i ] ; j++ )
		{
			if ( dis->subs_grp[ i ][ j ].type == SUB_DIRECT_ONE_CALL_BUFFER_INT )
			{
				if ( dis->subs_grp[ i ][ j ].tring_p_t )
				{
					token_ring_p_t * tring = ( token_ring_p_t * )dis->subs_grp[ i ][ j ].tring_p_t;
					void_p turn_key;
					token_ring_p_next( tring , &turn_key );
					if ( turn_key == ( void_p )&dis->subs_grp[ i ][ j ] )
					{
						return dis->subs_grp[ i ][ j ].func.direct_one_call_bfr_int_cb( dis->subs_grp[ i ][ j ].data , src_buf , src_i );
						break;
					}
				}
				else
				{
					return dis->subs_grp[ i ][ j ].func.direct_one_call_bfr_int_cb( dis->subs_grp[ i ][ j ].data , src_buf , src_i );
				}
			}
		}
	}
	return errNoPeer;
}
