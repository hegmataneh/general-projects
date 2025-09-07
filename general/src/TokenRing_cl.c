#define Uses_INIT_BREAKABLE_FXN
#define Uses_MEMSET_ZERO
#define Uses_TokenRing_cl
#include <general.dep>

void token_ring_cl_init( token_ring_cl_t * ring )
{
	MEMSET_ZERO_O( ring );
	ring->head = NULL;
	ring->current = NULL;
	pthread_mutex_init( &ring->lock , NULL );
}

status token_ring_cl_add( token_ring_cl_t * ring , callback_t1 cb , void_p user_data )
{
	INIT_BREAKABLE_FXN();

	token_item_cl_t * item;
	
	N_MALLOC_ONE( item , 0 );
	MEMSET_ZERO_O( item );
	
	item->callback = cb;
	item->user_data = user_data;
	item->next = NULL;

	pthread_mutex_lock( &ring->lock );

	if ( !ring->head )
	{
		ring->head = item;
		item->next = item; // circular
		ring->current = item;
	}
	else
	{
		token_item_cl_t * tail = ring->head;
		while ( tail->next != ring->head )
		{
			tail = tail->next;
		}
		tail->next = item;
		item->next = ring->head;
	}

	pthread_mutex_unlock( &ring->lock );

	BEGIN_SMPL
	N_END_RET
}

void token_ring_cl_next( token_ring_cl_t * ring )
{
	pthread_mutex_lock( &ring->lock );

	if ( !ring->current )
	{
		pthread_mutex_unlock( &ring->lock );
		return;
	}

	//token_item_cl_t * start = ring->current;

	token_item_cl_t * call = ring->current;
	ring->current = call->next;
	if ( call->callback )
	{
		call->callback( call->user_data );
	}

	pthread_mutex_unlock( &ring->lock );
}
