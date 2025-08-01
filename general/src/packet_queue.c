#define Uses_memcpy
#define Uses_pthread_mutex_t
#define Uses_PacketQueue
#include <general.dep>

void queue_init( struct PacketQueue * q )
{
	q->head = 0;
	q->tail = 0;
	q->count = 0;
	pthread_mutex_init( &q->lock , NULL );
	pthread_cond_init( &q->not_empty , NULL );
	pthread_cond_init( &q->not_full , NULL );
}

void queue_destroy( struct PacketQueue * q )
{
	pthread_mutex_destroy( &q->lock );
	pthread_cond_destroy( &q->not_empty );
	pthread_cond_destroy( &q->not_full );
}

void queue_push( struct PacketQueue * q , const char * buf , size_t len )
{
	pthread_mutex_lock( &q->lock );

	while ( q->count == QUEUE_CAPACITY )
	{
		pthread_cond_wait( &q->not_full , &q->lock );
	}

	memcpy( q->data[ q->tail ] , buf , len );
	q->lengths[ q->tail ] = len;
	q->tail = ( q->tail + 1 ) % QUEUE_CAPACITY;
	q->count++;

	pthread_cond_signal( &q->not_empty );
	pthread_mutex_unlock( &q->lock );
}

int queue_pop( struct PacketQueue * q , char * out_buf , size_t * out_len )
{
	pthread_mutex_lock( &q->lock );

	while ( q->count == 0 )
	{
		pthread_cond_wait( &q->not_empty , &q->lock );
	}

	size_t len = q->lengths[ q->head ];
	memcpy( out_buf , q->data[ q->head ] , len );
	*out_len = len;

	q->head = ( q->head + 1 ) % QUEUE_CAPACITY;
	q->count--;

	pthread_cond_signal( &q->not_full );
	pthread_mutex_unlock( &q->lock );

	return 0;
}

int queue_peek_available( struct PacketQueue * q )
{
	pthread_mutex_lock( &q->lock );
	int available = ( q->count > 0 );
	pthread_mutex_unlock( &q->lock );
	return available;
}

int queue_try_pop( struct PacketQueue * q , char * out_buf , size_t * out_len )
{
	pthread_mutex_lock( &q->lock );

	if ( q->count == 0 )
	{
		pthread_mutex_unlock( &q->lock );
		return -1;
	}

	size_t len = q->lengths[ q->head ];
	memcpy( out_buf , q->data[ q->head ] , len );
	*out_len = len;

	q->head = ( q->head + 1 ) % QUEUE_CAPACITY;
	q->count--;

	pthread_cond_signal( &q->not_full );
	pthread_mutex_unlock( &q->lock );

	return 0;
}
