#define Uses_LOCK_LINE
#define Uses_WARNING
#define Uses_memcpy
#define Uses_Vqueue
#include <general.dep>

void vqueue_init( HDLR Vqueue_t * pque , _INx void_p buffer , _INx size_t capacity , bool reset /*first time initialized or bind it to existing memory*/ )
{
	//MEMSET_ZERO_O( pque );
	pque->buf = buffer;
	if ( reset ) // is reset mode do not emptied buffer because it loaded from file content
	{
		pque->capacity = capacity;
		pque->pos_reader = pque->pos_writer = 0;
	}
	pthread_mutex_init( &pque->lock , NULL );
}

void vqueue_destroy( HDLR Vqueue_t * pque )
{
	//vstack_clear( stack , false ); // i should not clear variable because they should be in memmap and store
	pthread_mutex_destroy( &pque->lock );
}

/*
Each item layout : [data bytes] [size_t size]
touches[ 0 ] -> buf that changed
touches[ 1 ] -> position of stat filed changed that should be stored on file
*/
status vqueue_enque( HDLR Vqueue_t * pque , const void_p data , size_t size , vqtchs * touches )
{
	WARNING( size );

	VQUEUE_LOCK_LINE( pthread_mutex_lock( &pque->lock ) );

	if ( pque->pos_writer + size + sizeof( size_t ) > pque->capacity )
	{
		pthread_mutex_unlock( &pque->lock );
		return errOverflow; // overflow
	}

	*( size_t * )( pque->buf + pque->pos_writer ) = size;
	memcpy( pque->buf + sizeof( size_t ) + pque->pos_writer , data , size );
	if ( touches )
	{
		( *touches )[ 0 ].addr = pque->buf + sizeof( size_t ) + pque->pos_writer;
		( *touches )[ 0 ].sz = size;
		
		( *touches )[ 1 ].addr = &pque->pos_writer;
		( *touches )[ 1 ].sz = sizeof( pque->pos_writer );
	}
	pque->pos_writer += size + sizeof( size_t ); // it must be after touch evaluated. think about it to know what i do know sir

	pthread_mutex_unlock( &pque->lock );
	return errOK;
}

status vqueue_peek( HDLR Vqueue_t * pque , OUTx void_p * item , OUTx size_t * size )
{
	VQUEUE_LOCK_LINE( pthread_mutex_lock( &pque->lock ) );

	if ( pque->pos_reader >= pque->pos_writer )
	{
		pthread_mutex_unlock( &pque->lock );
		return errEmpty; // empty
	}

	size_t tmp_sz = *( size_t * )( pque->buf + pque->pos_reader );
	if ( pque->pos_reader + sizeof( size_t ) + tmp_sz > pque->pos_writer )
	{
		pthread_mutex_unlock( &pque->lock );
		return errCorrupted; // corrupted
	}
	if ( size ) *size = tmp_sz;
	if ( item ) *item = ( void_p )( pque->buf + pque->pos_reader + sizeof( size_t ) );

	pthread_mutex_unlock( &pque->lock );
	return errOK;
}

status vqueue_deque( HDLR Vqueue_t * pque , OUTx void_p * item , OUTx size_t * size , bool * pemptied )
{
	VQUEUE_LOCK_LINE( pthread_mutex_lock( &pque->lock ) );

	if ( pque->pos_reader >= pque->pos_writer )
	{
		pthread_mutex_unlock( &pque->lock );
		return errEmpty; // empty
	}

	size_t tmp_sz = *( size_t * )( pque->buf + pque->pos_reader );
	if ( pque->pos_reader + sizeof( size_t ) + tmp_sz > pque->pos_writer )
	{
		pthread_mutex_unlock( &pque->lock );
		return errCorrupted; // corrupted
	}
	if ( size ) *size = tmp_sz;
	if ( item ) *item = ( void_p )( pque->buf + pque->pos_reader + sizeof( size_t ) );
	pque->pos_reader += tmp_sz + sizeof( size_t );
	if ( pemptied ) *pemptied = ( pque->pos_reader >= pque->pos_writer );

	pthread_mutex_unlock( &pque->lock );
	return errOK;
}

void vqueue_clear( HDLR Vqueue_t * pque , bool clear_var )
{
	if ( clear_var )
	{
		pthread_mutex_lock( &pque->lock );
		pque->pos_reader = pque->pos_writer = 0;
		pthread_mutex_unlock( &pque->lock );
	}
}
