#define Uses_LOCK_LINE
#define Uses_DBG_PT
#define Uses_token_ring_p_t
#define Uses_INIT_BREAKABLE_FXN
#define Uses_MEMSET_ZERO
#define Uses_pub_sub
#include <general.dep>

status distributor_init( distributor_t * dis , size_t grp_count )
{
	INIT_BREAKABLE_FXN();

	MEMSET_ZERO_O( dis );

	BREAK_STAT( mms_array_init( &dis->grps , sizeof( subscribers_t ) , grp_count , GROW_STEP , 0 ) , 0 );

	for ( size_t idx = 0 ; idx < grp_count ; idx++ )
	{
		subscribers_t * psubscribers = NULL;
		BREAK_STAT( mms_array_get_one_available_unoccopied_item( &dis->grps , ( void ** )&psubscribers ) , 0 );
		BREAK_STAT( mms_array_init( &psubscribers->subs , sizeof( subscriber_t ) , 1 , GROW_STEP , 0 ) , 0 );
	}

	dis->iteration_dir = e_dir_default;

	BEGIN_SMPL
	N_END_RET
}

status distributor_init_withLock( distributor_t * dis , size_t grp_count )
{
	status ret = distributor_init( dis , grp_count );
	if ( ret == errOK )
	{
		if ( ( dis->pmtx = CALLOC_ONE( dis->pmtx ) ) )
		{
			if ( pthread_mutex_init( dis->pmtx , NULL ) )
			{
				ret = errCreation;
			}
		}
	}
	return ret;
}

status distributor_init_withOrder( distributor_t * dis , size_t grp_count )
{
	INIT_BREAKABLE_FXN();

	BREAK_STAT( distributor_init( dis , grp_count ) , 0 );
	
	N_MALLOC_ONE( dis->pheap , 0 );
	BREAK_STAT( mms_array_init( dis->pheap , sizeof( sub_custome_ord_t ) , 1 , GROW_STEP , 0 ) , 0 );
	
	BEGIN_SMPL
	N_END_RET
}

status distributor_init_withOrder_lock( distributor_t * dis , size_t grp_count )
{
	INIT_BREAKABLE_FXN();

	BREAK_STAT( distributor_init( dis , grp_count ) , 0 );

	N_MALLOC_ONE( dis->pheap , 0 );
	BREAK_STAT( mms_array_init( dis->pheap , sizeof( sub_custome_ord_t ) , 1 , GROW_STEP , 0 ) , 0 );

	if ( ( dis->pmtx = CALLOC_ONE( dis->pmtx ) ) )
	{
		BREAK_IF( pthread_mutex_init( dis->pmtx , NULL ) , errCreation , 0 );
	}

	BEGIN_SMPL
	N_END_RET
}

void sub_destroy( distributor_t * dis )
{
	if ( !dis ) return;
	//DBG_PT();
	for ( size_t igrp = 0 ; igrp < dis->grps.count ; igrp++ )
	{
		subscribers_t * psubscribers = NULL;
		if ( mms_array_get_s( &dis->grps , igrp , (void**)&psubscribers ) == errOK )
		{
			mms_array_free( &psubscribers->subs );
		}
	}
	//DBG_PT();
	mms_array_free( &dis->grps );
	//DBG_PT();
	mms_array_free( dis->pheap );
	//DBG_PT();
	DAC( dis->pheap );
	//DBG_PT();
	if ( dis->pmtx )
	{
		pthread_mutex_destroy( dis->pmtx );
		//DBG_PT();
		DAC( dis->pmtx );
	}
	//DBG_PT();
}

_PRIVATE_FXN status distributor_subscribe_t( distributor_t * dis , size_t iGrp /*1 on flat list*/ , sub_type_e type , sub_func_t func , pass_p data ,
				OUTcpy subscriber_t ** subed , int * orderr /*if not null then this is order*/ , void * forward_compatiblity )
{
	INIT_BREAKABLE_FXN();

	if ( dis->pmtx )
	{
		PUB_SUB_LOCK_LINE( pthread_mutex_lock( dis->pmtx ) );
	}

	subscribers_t * psubscribers = NULL;
	BREAK_STAT( mms_array_get_s( &dis->grps , iGrp , (void**)&psubscribers ) , 0 );

	subscriber_t * psubscriber = NULL;
	BREAK_STAT( mms_array_get_one_available_unoccopied_item( &psubscribers->subs , ( void ** )&psubscriber ) , 0 );
	
	psubscriber->type = type;
	psubscriber->func = func;
	psubscriber->data = data;

	if ( subed )
	{
		*subed = psubscriber;
	}
	if ( dis->pheap )
	{
		sub_custome_ord_t * pord = NULL;
		BREAK_STAT( mms_array_get_one_available_unoccopied_item( dis->pheap , (void**)&pord ) , 0 );
		pord->psubscriber = psubscriber;
		pord->order = orderr ? *orderr : 0;
	}

	BEGIN_SMPL
	N_V_END_RET
		if ( dis->pmtx )
		{
			pthread_mutex_unlock( dis->pmtx );
		}
	return d_error;
}

status distributor_get_data( distributor_t * dis , pass_p * pdata )
{
	INIT_BREAKABLE_FXN();

	subscribers_t * psubscribers = NULL;
	BREAK_STAT( mms_array_get_s( &dis->grps , 0 , (void**)&psubscribers ) , 0 );

	subscriber_t * psubscriber = NULL;
	BREAK_STAT( mms_array_get_s( &psubscribers->subs , 0 , (void**)&psubscriber ) , 0 );

	*pdata = psubscriber->data;
	
	BEGIN_SMPL
	N_END_RET
}

status distributor_subscribe( distributor_t * dis , sub_type_e type , sub_func_t func , pass_p data )
{
	return distributor_subscribe_t( dis , 0 , type , func , data , NULL , NULL , NULL );
}

status distributor_subscribe_out( distributor_t * dis , sub_type_e type , sub_func_t func , pass_p data , OUTcpy subscriber_t ** subed )
{
	return distributor_subscribe_t( dis , 0 , type , func , data , subed , NULL , NULL );
}

status distributor_subscribe_withOrder( distributor_t * dis , sub_type_e type , sub_func_t func , pass_p data , int order )
{
	return distributor_subscribe_t( dis , 0 , type , func , data , NULL , &order , NULL );
}

status distributor_subscribe_ingrp( distributor_t * dis , size_t iGrp /*1 on flat list*/ , sub_type_e type , sub_func_t func , pass_p data )
{
	return distributor_subscribe_t( dis , iGrp , type , func , data , NULL , NULL , NULL );
}

status distributor_subscribe_with_ring( distributor_t * dis , size_t iGrp /*1 on flat list*/ , sub_type_e type , sub_func_t func , pass_p data , void_p src_tring )
{
	INIT_BREAKABLE_FXN();

	token_ring_p_t * tring = ( token_ring_p_t * )src_tring;

	subscriber_t * subed = NULL;
	BREAK_STAT( distributor_subscribe_t( dis , iGrp , type , func , data , &subed , NULL , NULL ) , 0 );
	subed->tring_p_t = tring; 
	BREAK_STAT( token_ring_p_add( tring , subed ) , 0 );

	BEGIN_SMPL
	N_END_RET
}

status distributor_subscribe_onedirectcall( distributor_t * dis , sub_type_e type , sub_func_t func , void_p token , pass_p data )
{
	INIT_BREAKABLE_FXN();

	subscriber_t * subed = NULL;
	BREAK_STAT( distributor_subscribe_t( dis , 0 , type , func , data , &subed , NULL , NULL ) , 0 );
	subed->token = token;

	BEGIN_SMPL
	N_END_RET
}

_PRIVATE_FXN int compare_subscribers( const void * a , const void * b )
{
	sub_custome_ord_t * arg1 = *( sub_custome_ord_t ** )a;
	sub_custome_ord_t * arg2 = *( sub_custome_ord_t ** )b;
	if ( arg1->order < arg2->order ) return 1; // less value means low priority
	if ( arg1->order > arg2->order ) return -1; // more value means higher order
	return 0;
}

#define DATA_ORDER_( exp ) ( ( exp->data_order == ord_producer ) ? ISNULL( data , exp->data ) : ISNULL( exp->data , data ) )

status distributor_publish_void( distributor_t * dis , pass_p data /*=NULL if subscriber precede*/ )
{
	INIT_BREAKABLE_FXN();

	if ( dis->pmtx )
	{
		PUB_SUB_LOCK_LINE( pthread_mutex_lock( dis->pmtx ) );
	}

	if ( dis->pheap ) // 
	{
		if ( dis->pheap->count ) qsort( dis->pheap->data , dis->pheap->count , sizeof( void * ) , compare_subscribers );

		for ( size_t idx = 0 ; idx < dis->pheap->count ; idx++ )
		{
			sub_custome_ord_t * pord = NULL;
			if ( mms_array_get_s( dis->pheap , idx , ( void ** )&pord ) == errOK && pord->psubscriber->type == SUB_VOID )
			{
				pord->psubscriber->func.void_cb( DATA_ORDER_(pord->psubscriber) );
			}
		}
		BREAK( errOK , 0 );
	}
	for ( size_t igrp = 0; igrp < dis->grps.count ; igrp++ )
	{
		int start , end , step;

		subscribers_t * psubscribers = NULL;
		BREAK_STAT( mms_array_get_s( &dis->grps , igrp , (void**)&psubscribers ) , 0 );

		if ( dis->iteration_dir == head_2_tail )
		{
			start = 0;
			end = ( int )psubscribers->subs.count;
			step = 1;
		}
		else
		{
			start = ( int )psubscribers->subs.count - 1;
			end = -1;  // because we'll stop when isub < 0
			step = -1;
		}
		for ( int isub = start ; isub != end ; isub += step )
		{
			subscriber_t * psubscriber = NULL;
			BREAK_STAT( mms_array_get_s( &psubscribers->subs , ( size_t )isub , (void**)&psubscriber ) , 0 );
			if ( psubscriber->type == SUB_VOID )
			{
				psubscriber->func.void_cb( DATA_ORDER_(psubscriber) );
			}
		}
	}

	BEGIN_SMPL
	N_V_END_RET
	if ( dis->pmtx )
	{
		pthread_mutex_unlock( dis->pmtx );
	}
	return d_error;
}

// Publish different kinds of events
status distributor_publish_str( distributor_t * dis , LPCSTR src_msg , pass_p data /*=NULL if subscriber precede*/ )
{
	INIT_BREAKABLE_FXN();

	if ( dis->pmtx )
	{
		PUB_SUB_LOCK_LINE( pthread_mutex_lock( dis->pmtx ) );
	}

	if ( dis->pheap )
	{
		if ( dis->pheap->count ) qsort( dis->pheap->data , dis->pheap->count , sizeof( void * ) , compare_subscribers );

		for ( size_t idx = 0 ; idx < dis->pheap->count ; idx++ )
		{
			sub_custome_ord_t * pord = NULL;
			if ( mms_array_get_s( dis->pheap , idx , ( void ** )&pord ) == errOK && pord->psubscriber->type == SUB_STRING )
			{
				pord->psubscriber->func.str_cb( DATA_ORDER_(pord->psubscriber) , src_msg );
			}
		}
		
		BREAK( errOK , 0 );
	}
	for ( size_t igrp = 0; igrp < dis->grps.count ; igrp++ )
	{
		int one_token_ring_called = 0;
		int start , end , step;

		subscribers_t * psubscribers = NULL;
		BREAK_STAT( mms_array_get_s( &dis->grps , igrp , (void**)&psubscribers ) , 0 );

		if ( dis->iteration_dir == head_2_tail )
		{
			start = 0;
			end = ( int )psubscribers->subs.count;
			step = 1;
		}
		else
		{
			start = ( int )psubscribers->subs.count - 1;
			end = -1;  // because we'll stop when isub < 0
			step = -1;
		}
		for ( int isub = start ; isub != end ; isub += step )
		{
			subscriber_t * psubscriber = NULL;
			BREAK_STAT( mms_array_get_s( &psubscribers->subs , ( size_t )isub , (void**)&psubscriber ) , 0 );

			if ( psubscriber->type == SUB_STRING )
			{
				if ( psubscriber->tring_p_t )
				{
					if ( !one_token_ring_called )
					{
						token_ring_p_t * tring = ( token_ring_p_t * )psubscriber->tring_p_t;
						void_p turn_key;
						token_ring_p_curr( tring , &turn_key );
						if ( turn_key == ( void_p )psubscriber )
						{
							token_ring_p_next( tring , &turn_key );
							one_token_ring_called = 1;
							psubscriber->func.str_cb( DATA_ORDER_(psubscriber) , src_msg );
							break;
						}
					}
				}
				else
				{
					psubscriber->func.str_cb( DATA_ORDER_(psubscriber) , src_msg );
				}
			}
		}
	}

	BEGIN_SMPL
	N_V_END_RET
	if ( dis->pmtx )
	{
		pthread_mutex_unlock( dis->pmtx );
	}
	return d_error;
}

status distributor_publish_long( distributor_t * dis , long src_v , pass_p data /*=NULL if subscriber precede*/ )
{
	INIT_BREAKABLE_FXN();

	if ( dis->pmtx )
	{
		PUB_SUB_LOCK_LINE( pthread_mutex_lock( dis->pmtx ) );
	}

	if ( dis->pheap )
	{
		if ( dis->pheap->count ) qsort( dis->pheap->data , dis->pheap->count , sizeof( void * ) , compare_subscribers );

		for ( size_t idx = 0 ; idx < dis->pheap->count ; idx++ )
		{
			sub_custome_ord_t * pord = NULL;
			if ( mms_array_get_s( dis->pheap , idx , ( void ** )&pord ) == errOK && pord->psubscriber->type == SUB_LONG )
			{
				pord->psubscriber->func.long_cb( DATA_ORDER_(pord->psubscriber) , src_v == NP ? ( long )pord->order : src_v ); // pass order if src_v is NP
			}
		}

		BREAK( errOK , 0 );
	}
	for ( size_t igrp = 0; igrp < dis->grps.count ; igrp++ )
	{
		int one_token_ring_called = 0;
		int start , end , step;

		subscribers_t * psubscribers = NULL;
		BREAK_STAT( mms_array_get_s( &dis->grps , igrp , (void**)&psubscribers ) , 0 );

		if ( dis->iteration_dir == head_2_tail )
		{
			start = 0;
			end = ( int )psubscribers->subs.count;
			step = 1;
		}
		else
		{
			start = ( int )psubscribers->subs.count - 1;
			end = -1;  // because we'll stop when isub < 0
			step = -1;
		}

		for ( int isub = start ; isub != end ; isub += step )
		{
			subscriber_t * psubscriber = NULL;
			BREAK_STAT( mms_array_get_s( &psubscribers->subs , ( size_t )isub , (void**)&psubscriber ) , 0 );

			if ( psubscriber->type == SUB_LONG )
			{
				if ( psubscriber->tring_p_t )
				{
					if ( !one_token_ring_called )
					{
						token_ring_p_t * tring = ( token_ring_p_t * )psubscriber->tring_p_t;
						void_p turn_key;
						token_ring_p_curr( tring , &turn_key );
						if ( turn_key == ( void_p )psubscriber )
						{
							token_ring_p_next( tring , &turn_key );
							one_token_ring_called = 1;
							psubscriber->func.long_cb( DATA_ORDER_(psubscriber) , src_v );
							break;
						}
					}
				}
				else
				{
					psubscriber->func.long_cb( DATA_ORDER_(psubscriber) , src_v );
				}
			}
		}
	}

	BEGIN_SMPL
	N_V_END_RET
	if ( dis->pmtx )
	{
		pthread_mutex_unlock( dis->pmtx );
	}
	return d_error;
}

status distributor_publish_double( distributor_t * dis , double src_v , pass_p data /*=NULL if subscriber precede*/ )
{
	INIT_BREAKABLE_FXN();

	if ( dis->pmtx )
	{
		PUB_SUB_LOCK_LINE( pthread_mutex_lock( dis->pmtx ) );
	}

	if ( dis->pheap )
	{
		if ( dis->pheap->count ) qsort( dis->pheap->data , dis->pheap->count , sizeof( void * ) , compare_subscribers );

		for ( size_t idx = 0 ; idx < dis->pheap->count ; idx++ )
		{
			sub_custome_ord_t * pord = NULL;
			if ( mms_array_get_s( dis->pheap , idx , ( void ** )&pord ) == errOK && pord->psubscriber->type == SUB_DOUBLE )
			{
				pord->psubscriber->func.dbl_cb( DATA_ORDER_(pord->psubscriber) , src_v );
			}
		}

		BREAK( errOK , 0 );
	}
	for ( size_t igrp = 0; igrp < dis->grps.count ; igrp++ )
	{
		int one_token_ring_called = 0;
		int start , end , step;
		
		subscribers_t * psubscribers = NULL;
		BREAK_STAT( mms_array_get_s( &dis->grps , igrp , (void**)&psubscribers ) , 0 );

		if ( dis->iteration_dir == head_2_tail )
		{
			start = 0;
			end = ( int )psubscribers->subs.count;
			step = 1;
		}
		else
		{
			start = ( int )psubscribers->subs.count - 1;
			end = -1;  // because we'll stop when isub < 0
			step = -1;
		}
		for ( int isub = start ; isub != end ; isub += step )
		{
			subscriber_t * psubscriber = NULL;
			BREAK_STAT( mms_array_get_s( &psubscribers->subs , ( size_t )isub , (void**)&psubscriber ) , 0 );

			if ( psubscriber->type == SUB_DOUBLE )
			{
				if ( psubscriber->tring_p_t )
				{
					if ( !one_token_ring_called )
					{
						token_ring_p_t * tring = ( token_ring_p_t * )psubscriber->tring_p_t;
						void_p turn_key;
						token_ring_p_curr( tring , &turn_key );
						if ( turn_key == ( void_p )psubscriber )
						{
							token_ring_p_next( tring , &turn_key );
							one_token_ring_called = 1;
							psubscriber->func.dbl_cb( DATA_ORDER_(psubscriber) , src_v );
							break;
						}
					}
				}
				else
				{
					psubscriber->func.dbl_cb( DATA_ORDER_(psubscriber) , src_v );
				}
			}
		}
	}

	BEGIN_SMPL
		N_V_END_RET
		if ( dis->pmtx )
		{
			pthread_mutex_unlock( dis->pmtx );
		}
	return d_error;
}

status distributor_publish_long_double( distributor_t * dis , long src_i , double src_d , pass_p data /*=NULL if subscriber precede*/ )
{
	INIT_BREAKABLE_FXN();

	if ( dis->pmtx )
	{
		PUB_SUB_LOCK_LINE( pthread_mutex_lock( dis->pmtx ) );
	}

	if ( dis->pheap )
	{
		if ( dis->pheap->count ) qsort( dis->pheap->data , dis->pheap->count , sizeof( void * ) , compare_subscribers );

		for ( size_t idx = 0 ; idx < dis->pheap->count ; idx++ )
		{
			sub_custome_ord_t * pord = NULL;
			if ( mms_array_get_s( dis->pheap , idx , ( void ** )&pord ) == errOK && pord->psubscriber->type == SUB_LONG_DOUBLE )
			{
				pord->psubscriber->func.long_dbl_cb( DATA_ORDER_(pord->psubscriber) , src_i , src_d );
			}
		}
		
		BREAK( errOK , 0 );
	}
	for ( size_t igrp = 0; igrp < dis->grps.count ; igrp++ )
	{
		int one_token_ring_called = 0;
		int start , end , step;
		subscribers_t * psubscribers = NULL;
		BREAK_STAT( mms_array_get_s( &dis->grps , igrp , (void**)&psubscribers ) , 0 );

		if ( dis->iteration_dir == head_2_tail )
		{
			start = 0;
			end = ( int )psubscribers->subs.count;
			step = 1;
		}
		else
		{
			start = ( int )psubscribers->subs.count - 1;
			end = -1;  // because we'll stop when isub < 0
			step = -1;
		}
		for ( int isub = start ; isub != end ; isub += step )
		{
			subscriber_t * psubscriber = NULL;
			BREAK_STAT( mms_array_get_s( &psubscribers->subs , ( size_t )isub , (void**)&psubscriber ) , 0 );

			if ( psubscriber->type == SUB_LONG_DOUBLE )
			{
				if ( psubscriber->tring_p_t )
				{
					if ( !one_token_ring_called )
					{
						token_ring_p_t * tring = ( token_ring_p_t * )psubscriber->tring_p_t;
						void_p turn_key;
						token_ring_p_curr( tring , &turn_key );
						if ( turn_key == ( void_p )psubscriber )
						{
							token_ring_p_next( tring , &turn_key );
							one_token_ring_called = 1;
							psubscriber->func.long_dbl_cb( DATA_ORDER_(psubscriber) , src_i , src_d );
							break;
						}
					}
				}
				else
				{
					psubscriber->func.long_dbl_cb( DATA_ORDER_(psubscriber) , src_i , src_d );
				}
			}
		}
	}

	BEGIN_SMPL
		N_V_END_RET
		if ( dis->pmtx )
		{
			pthread_mutex_unlock( dis->pmtx );
		}
	return d_error;
}

status distributor_publish_x3long( distributor_t * dis , long src_i , long src_j , long src_k , pass_p data /*=NULL if subscriber precede*/ )
{
	INIT_BREAKABLE_FXN();

	if ( dis->pmtx )
	{
		PUB_SUB_LOCK_LINE( pthread_mutex_lock( dis->pmtx ) );
	}

	if ( dis->pheap )
	{
		if ( dis->pheap->count ) qsort( dis->pheap->data , dis->pheap->count , sizeof( void * ) , compare_subscribers );

		for ( size_t idx = 0 ; idx < dis->pheap->count ; idx++ )
		{
			sub_custome_ord_t * pord = NULL;
			if ( mms_array_get_s( dis->pheap , idx , ( void ** )&pord ) == errOK && pord->psubscriber->type == SUB_x3LONG )
			{
				pord->psubscriber->func.x3long_cb( DATA_ORDER_( pord->psubscriber ) , src_i , src_j , src_k );
			}
		}

		BREAK( errOK , 0 );
	}
	for ( size_t igrp = 0; igrp < dis->grps.count ; igrp++ )
	{
		//int one_token_ring_called = 0;
		int start , end , step;
		subscribers_t * psubscribers = NULL;
		BREAK_STAT( mms_array_get_s( &dis->grps , igrp , ( void ** )&psubscribers ) , 0 );

		if ( dis->iteration_dir == head_2_tail )
		{
			start = 0;
			end = ( int )psubscribers->subs.count;
			step = 1;
		}
		else
		{
			start = ( int )psubscribers->subs.count - 1;
			end = -1;  // because we'll stop when isub < 0
			step = -1;
		}
		for ( int isub = start ; isub != end ; isub += step )
		{
			subscriber_t * psubscriber = NULL;
			BREAK_STAT( mms_array_get_s( &psubscribers->subs , ( size_t )isub , ( void ** )&psubscriber ) , 0 );

			if ( psubscriber->type == SUB_x3LONG )
			{
				// TODO . tring does not implemented for this
				if ( !psubscriber->tring_p_t )
				{
					psubscriber->func.x3long_cb( DATA_ORDER_( psubscriber ) , src_i , src_j , src_k );
				}
			}
		}
	}

	BEGIN_SMPL
		N_V_END_RET
		if ( dis->pmtx )
		{
			pthread_mutex_unlock( dis->pmtx );
		}
	return d_error;
}


status distributor_publish_str_double( distributor_t * dis , LPCSTR src_str , double src_d , pass_p data /*=NULL if subscriber precede*/ )
{
	INIT_BREAKABLE_FXN();

	if ( dis->pmtx )
	{
		PUB_SUB_LOCK_LINE( pthread_mutex_lock( dis->pmtx ) );
	}

	if ( dis->pheap )
	{
		if ( dis->pheap->count ) qsort( dis->pheap->data , dis->pheap->count , sizeof( void * ) , compare_subscribers );

		for ( size_t idx = 0 ; idx < dis->pheap->count ; idx++ )
		{
			sub_custome_ord_t * pord = NULL;
			if ( mms_array_get_s( dis->pheap , idx , ( void ** )&pord ) == errOK && pord->psubscriber->type == SUB_STRING_DOUBLE )
			{
				pord->psubscriber->func.str_dbl_cb( DATA_ORDER_(pord->psubscriber) , src_str , src_d );
			}
		}

		BREAK( errOK , 0 );
	}
	for ( size_t igrp = 0; igrp < dis->grps.count ; igrp++ )
	{
		int one_token_ring_called = 0;
		int start , end , step;

		subscribers_t * psubscribers = NULL;
		BREAK_STAT( mms_array_get_s( &dis->grps , igrp , (void**)&psubscribers ) , 0 );

		if ( dis->iteration_dir == head_2_tail )
		{
			start = 0;
			end = ( int )psubscribers->subs.count;
			step = 1;
		}
		else
		{
			start = ( int )psubscribers->subs.count - 1;
			end = -1;  // because we'll stop when isub < 0
			step = -1;
		}
		for ( int isub = start ; isub != end ; isub += step )
		{
			subscriber_t * psubscriber = NULL;
			BREAK_STAT( mms_array_get_s( &psubscribers->subs , ( size_t )isub , (void**)&psubscriber ) , 0 );

			if ( psubscriber->type == SUB_STRING_DOUBLE )
			{
				if ( psubscriber->tring_p_t )
				{
					if ( !one_token_ring_called )
					{
						token_ring_p_t * tring = ( token_ring_p_t * )psubscriber->tring_p_t;
						void_p turn_key;
						token_ring_p_curr( tring , &turn_key );
						if ( turn_key == ( void_p )psubscriber )
						{
							token_ring_p_next( tring , &turn_key );
							one_token_ring_called = 1;
							psubscriber->func.str_dbl_cb( DATA_ORDER_(psubscriber) , src_str , src_d );
							break;
						}
					}
				}
				else
				{
					psubscriber->func.str_dbl_cb( DATA_ORDER_(psubscriber) , src_str , src_d );
				}
			}
		}
	}

	BEGIN_SMPL
		N_V_END_RET
		if ( dis->pmtx )
		{
			pthread_mutex_unlock( dis->pmtx );
		}
	return d_error;
}

status distributor_publish_buffer_size( distributor_t * dis , buffer src_buf , size_t src_sz , pass_p data /*=NULL if subscriber precede*/ )
{
	INIT_BREAKABLE_FXN();

	if ( dis->pmtx )
	{
		PUB_SUB_LOCK_LINE( pthread_mutex_lock( dis->pmtx ) );
	}

	status aggr_ret = errOK;
	int any_call_happend = 0;
	for ( size_t igrp = 0; igrp < dis->grps.count ; igrp++ )
	{
		int one_token_ring_called = 0;
		int start , end , step;

		subscribers_t * psubscribers = NULL;
		BREAK_STAT( mms_array_get_s( &dis->grps , igrp , (void**)&psubscribers ) , 0 );

		if ( dis->iteration_dir == head_2_tail )
		{
			start = 0;
			end = ( int )psubscribers->subs.count;
			step = 1;
		}
		else
		{
			start = ( int )psubscribers->subs.count - 1;
			end = -1;  // because we'll stop when isub < 0
			step = -1;
		}
		for ( int isub = start ; isub != end ; isub += step )
		{
			subscriber_t * psubscriber = NULL;
			BREAK_STAT( mms_array_get_s( &psubscribers->subs , ( size_t )isub , (void**)&psubscriber ) , 0 );

			if ( psubscriber->type == SUB_DIRECT_ONE_CALL_BUFFER_SIZE )
			{
				if ( psubscriber->tring_p_t )
				{
					if ( !one_token_ring_called )
					{
						token_ring_p_t * tring = ( token_ring_p_t * )psubscriber->tring_p_t;
						void_p turn_key = NULL;
						token_ring_p_curr( tring , &turn_key );
						if ( turn_key == ( void_p )psubscriber )
						{
							token_ring_p_next( tring , &turn_key );
							one_token_ring_called = 1;
							return psubscriber->func.direct_one_call_bfr_size_cb( DATA_ORDER_(psubscriber) , src_buf , src_sz );
						}
					}
				}
				else
				{
					return psubscriber->func.direct_one_call_bfr_size_cb( DATA_ORDER_(psubscriber) , src_buf , src_sz );
				}
			}
			else if ( psubscriber->type == SUB_DIRECT_MULTICAST_CALL_BUFFER_SIZE )
			{
				if ( psubscriber->tring_p_t )
				{
					if ( !one_token_ring_called )
					{
						token_ring_p_t * tring = ( token_ring_p_t * )psubscriber->tring_p_t;
						void_p turn_key = NULL;
						token_ring_p_curr( tring , &turn_key );
						if ( turn_key == ( void_p )psubscriber )
						{
							token_ring_p_next( tring , &turn_key );
							one_token_ring_called = 1;
							any_call_happend = 1;
							aggr_ret |= psubscriber->func.multicast_call_buffer_size_cb( DATA_ORDER_(psubscriber) , src_buf , src_sz );
						}
					}
				}
				else
				{
					any_call_happend = 1;
					aggr_ret |= psubscriber->func.multicast_call_buffer_size_cb( DATA_ORDER_(psubscriber) , src_buf , src_sz );
				}
			}
		}
	}
	BEGIN_SMPL
	N_V_END_RET
		if ( dis->pmtx )
		{
			pthread_mutex_unlock( dis->pmtx );
		}
	return any_call_happend ? aggr_ret : errNoPeer;
}

status distributor_publish_buffer_size_data( distributor_t * dis , buffer src_buf , size_t src_sz , long ex_data , pass_p data /*=NULL if subscriber precede*/ )
{
	INIT_BREAKABLE_FXN();

	if ( dis->pmtx )
	{
		PUB_SUB_LOCK_LINE( pthread_mutex_lock( dis->pmtx ) );
	}

	status aggr_ret = errOK;
	int any_call_happend = 0;
	for ( size_t igrp = 0; igrp < dis->grps.count ; igrp++ )
	{
		//int one_token_ring_called = 0;
		int start , end , step;

		subscribers_t * psubscribers = NULL;
		BREAK_STAT( mms_array_get_s( &dis->grps , igrp , ( void ** )&psubscribers ) , 0 );

		if ( dis->iteration_dir == head_2_tail )
		{
			start = 0;
			end = ( int )psubscribers->subs.count;
			step = 1;
		}
		else
		{
			start = ( int )psubscribers->subs.count - 1;
			end = -1;  // because we'll stop when isub < 0
			step = -1;
		}
		for ( int isub = start ; isub != end ; isub += step )
		{
			subscriber_t * psubscriber = NULL;
			BREAK_STAT( mms_array_get_s( &psubscribers->subs , ( size_t )isub , ( void ** )&psubscriber ) , 0 );

			if ( psubscriber->type == SUB_DIRECT_MULTICAST_CALL_BUFFER_SIZE_LONG )
			{
				any_call_happend = 1;
				aggr_ret |= psubscriber->func.multicast_call_buffer_size_long_cb( DATA_ORDER_( psubscriber ) , src_buf , src_sz , ex_data );
			}
		}
	}
	BEGIN_SMPL
		N_V_END_RET
		if ( dis->pmtx )
		{
			pthread_mutex_unlock( dis->pmtx );
		}
	return any_call_happend ? aggr_ret : errNoPeer;
}

status distributor_publish_onedirectcall_voidp( distributor_t * dis , void_p ptr /*caller pointer*/ ,
	void_p token /*token that spec calle*/ , pass_p data /*=NULL if subscriber precede*/ )
{
	INIT_BREAKABLE_FXN();

	if ( dis->pmtx )
	{
		PUB_SUB_LOCK_LINE( pthread_mutex_lock( dis->pmtx ) );
	}

	for ( size_t igrp = 0; igrp < dis->grps.count ; igrp++ )
	{
		subscribers_t * psubscribers = NULL;
		BREAK_STAT( mms_array_get_s( &dis->grps , igrp , (void**)&psubscribers ) , 0 );

		for ( size_t isub = 0 ; isub < psubscribers->subs.count ; isub++ )
		{
			subscriber_t * psubscriber = NULL;
			BREAK_STAT( mms_array_get_s( &psubscribers->subs , isub , (void**)&psubscriber ) , 0 );

			if
			(
				psubscriber->type == SUB_DIRECT_ONE_CALL_VOIDP &&
				psubscriber->token == token
			)
			{
				return psubscriber->func.direct_one_call_voidp_cb( DATA_ORDER_(psubscriber) , ptr );
			}
		}
	}

	BEGIN_SMPL
	N_V_END_RET
		if ( dis->pmtx )
		{
			pthread_mutex_unlock( dis->pmtx );
		}
	return errNoPeer;
}

status distributor_publish_voidp( distributor_t * dis , void_p ptr /*caller pointer*/ , pass_p data /*=NULL custom per call data or per subscriber_t*/ )
{
	INIT_BREAKABLE_FXN();

	if ( dis->pmtx )
	{
		PUB_SUB_LOCK_LINE( pthread_mutex_lock( dis->pmtx ) );
	}

	for ( size_t igrp = 0; igrp < dis->grps.count ; igrp++ )
	{
		subscribers_t * psubscribers = NULL;
		BREAK_STAT( mms_array_get_s( &dis->grps , igrp , (void**)&psubscribers ) , 0 );

		for ( size_t isub = 0 ; isub < psubscribers->subs.count ; isub++ )
		{
			subscriber_t * psubscriber = NULL;
			BREAK_STAT( mms_array_get_s( &psubscribers->subs , isub , (void**)&psubscriber ) , 0 );

			if
			(
				psubscriber->type == SUB_DIRECT_ONE_CALL_VOIDP
			)
			{
				return psubscriber->func.direct_one_call_voidp_cb( DATA_ORDER_(psubscriber) , ptr );
			}
		}
	}
	
	BEGIN_SMPL
	N_V_END_RET
		if ( dis->pmtx )
		{
			pthread_mutex_unlock( dis->pmtx );
		}
	return errNoPeer;
}

status distributor_publish_onedirectcall_3voidp( distributor_t * dis , void_p ptr1 , void_p ptr2 , void_p ptr3 )
{
	INIT_BREAKABLE_FXN();

	if ( dis->pmtx )
	{
		PUB_SUB_LOCK_LINE( pthread_mutex_lock( dis->pmtx ) );
	}

	for ( size_t igrp = 0; igrp < dis->grps.count ; igrp++ )
	{
		subscribers_t * psubscribers = NULL;
		BREAK_STAT( mms_array_get_s( &dis->grps , igrp , (void**)&psubscribers ) , 0 );

		for ( size_t isub = 0 ; isub < psubscribers->subs.count ; isub++ )
		{
			subscriber_t * psubscriber = NULL;
			BREAK_STAT( mms_array_get_s( &psubscribers->subs , isub , (void**)&psubscriber ) , 0 );

			if ( psubscriber->type == SUB_DIRECT_ONE_CALL_3VOIDP )
			{
				return psubscriber->func.direct_one_call_3voidp_cb( ptr1 , ptr2 , ptr3 );
			}
		}
	}

	BEGIN_SMPL
	N_V_END_RET
		if ( dis->pmtx )
		{
			pthread_mutex_unlock( dis->pmtx );
		}
	return errNoPeer;
}
