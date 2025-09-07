#define Uses_MEMSET_ZERO
#define Uses_pthread_mutex_t
#define Uses_cirbuf_packet_sync
#include <general.dep>

void cbuf_ps_init( cbuf_packet * q )
{
	q->head = 0;
	q->tail = 0;
	q->count = 0;
	pthread_mutex_init( &q->lock , NULL );
	pthread_cond_init( &q->not_empty , NULL );
	pthread_cond_init( &q->not_full , NULL );
}

void cbuf_ps_destroy( cbuf_packet * q )
{
	pthread_mutex_destroy( &q->lock );
	pthread_cond_destroy( &q->not_empty );
	pthread_cond_destroy( &q->not_full );
}

void cbuf_ps_push( cbuf_packet * q , const buffer buf , size_t len )
{
	pthread_mutex_lock( &q->lock );

	while ( q->count == QUEUE_CAPACITY )
	{
		pthread_cond_wait( &q->not_full , &q->lock );
	}

	MEMCPY_OR( q->data[ q->tail ] , buf , len );
	q->lengths[ q->tail ] = len;
	q->tail = ( q->tail + 1 ) % QUEUE_CAPACITY;
	q->count++;

	pthread_cond_signal( &q->not_empty );
	pthread_mutex_unlock( &q->lock );
}

void cbuf_ps_pop( cbuf_packet * q , buffer out_buf , size_t * out_len )
{
	pthread_mutex_lock( &q->lock );

	while ( q->count == 0 )
	{
		pthread_cond_wait( &q->not_empty , &q->lock );
	}

	size_t len = q->lengths[ q->head ];
	MEMCPY_OR( out_buf , q->data[ q->head ] , len );
	*out_len = len;

	q->head = ( q->head + 1 ) % QUEUE_CAPACITY;
	q->count--;

	pthread_cond_signal( &q->not_full );
	pthread_mutex_unlock( &q->lock );
}

int cbuf_ps_peek_available( cbuf_packet * q )
{
	pthread_mutex_lock( &q->lock );
	int available = ( q->count > 0 );
	pthread_mutex_unlock( &q->lock );
	return available;
}

int cbuf_ps_try_pop( cbuf_packet * q , buffer out_buf , size_t * out_len )
{
	pthread_mutex_lock( &q->lock );

	if ( q->count == 0 )
	{
		pthread_mutex_unlock( &q->lock );
		return -1;
	}

	size_t len = q->lengths[ q->head ];
	MEMCPY_OR( out_buf , q->data[ q->head ] , len );
	*out_len = len;

	q->head = ( q->head + 1 ) % QUEUE_CAPACITY;
	q->count--;

	pthread_cond_signal( &q->not_full );
	pthread_mutex_unlock( &q->lock );

	return 0;
}
