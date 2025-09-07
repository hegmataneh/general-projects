#define Uses_INIT_BREAKABLE_FXN
#define Uses_MEMSET_ZERO
#define Uses_TokenRing_p
#include <general.dep>

void token_ring_p_init( token_ring_p_t * ring )
{
	MEMSET_ZERO_O( ring );
	ring->head = NULL;
	ring->current = NULL;
	pthread_mutex_init( &ring->lock , NULL );
}

status token_ring_p_add( token_ring_p_t * ring , void_p turn_key )
{
	INIT_BREAKABLE_FXN();

	struct token_item_p * item;

	N_MALLOC_ONE( item , 0 );
	MEMSET_ZERO_O( item );

	item->next = NULL;
	item->data = turn_key;

	pthread_mutex_lock( &ring->lock );

	if ( !ring->head )
	{
		ring->head = item;
		item->next = item; // circular
		ring->current = item;
	}
	else
	{
		struct token_item_p * tail = ring->head;
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

void token_ring_p_curr( token_ring_p_t * ring , void_p * turn )
{
	*turn = NULL;

	pthread_mutex_lock( &ring->lock );

	if ( !ring->current )
	{
		pthread_mutex_unlock( &ring->lock );
		return;
	}

	struct token_item_p * current = ring->current;
	*turn = current->data;
	//ring->current = current->next;

	pthread_mutex_unlock( &ring->lock );
}

void token_ring_p_next( token_ring_p_t * ring , void_p * turn )
{
	*turn = NULL;

	pthread_mutex_lock( &ring->lock );

	if ( !ring->current )
	{
		pthread_mutex_unlock( &ring->lock );
		return;
	}

	struct token_item_p * current = ring->current;
	*turn = current->data;
	ring->current = current->next;

	pthread_mutex_unlock( &ring->lock );
}

void token_ring_destroy( token_ring_p_t * ring )
{
	if ( !ring || !ring->head )
	{
		return;
	}

	pthread_mutex_lock( &ring->lock );

	struct token_item_p * cur = ring->head;
	struct token_item_p * next;

	// Walk the ring and free each node
	do
	{
		next = cur->next;
		free( cur );
		cur = next;
	} while ( cur != ring->head );

	ring->head = NULL;
	ring->current = NULL;

	pthread_mutex_unlock( &ring->lock );
	pthread_mutex_destroy( &ring->lock );
}

