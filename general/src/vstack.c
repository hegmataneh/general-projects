#define Uses_LOCK_LINE
#define Uses_WARNING
#define Uses_memcpy
#define Uses_VStack
#include <general.dep>

void vstack_init( HDLR VStack * stack , _INx void_p buffer , _INx size_t capacity , bool reset /*first time initialized or bind it to existing memory*/ )
{
	stack->buf = buffer;
	if ( reset )
	{
		stack->capacity = capacity;
		stack->top = 0;
	}
	pthread_mutex_init( &stack->lock , NULL );
}

void vstack_destroy( HDLR VStack * stack )
{
	//vstack_clear( stack , false ); // i should not clear variable because they should be in memmap and store
	pthread_mutex_destroy( &stack->lock );
}

// Each item layout: [data bytes][size_t size]
status vstack_push( HDLR VStack * stack , const void_p data , size_t size , tchs * touches )
{
	WARNING( size );

	LOCK_LINE( pthread_mutex_lock( &stack->lock ) );

	if ( stack->top + size + sizeof( size_t ) > stack->capacity )
	{
		pthread_mutex_unlock( &stack->lock );
		return errOverflow; // overflow
	}

	memcpy( stack->buf + stack->top , data , size );
	size_t * szptr = ( size_t * )( stack->buf + stack->top + size );
	*szptr = size;
	stack->top += size + sizeof( size_t );
	if ( touches )
	{
		( *touches )[ 0 ].addr = stack->buf + stack->top;
		( *touches )[ 0 ].sz = size;
		( *touches )[ 1 ].addr = &stack->top;
		( *touches )[ 1 ].sz = sizeof( stack->top );
	}

	pthread_mutex_unlock( &stack->lock );
	return errOK;
}

status vstack_peek( HDLR VStack * stack , OUTx void_p * item , OUTx size_t * size )
{
	LOCK_LINE( pthread_mutex_lock( &stack->lock ) );

	if ( stack->top == 0 )
	{
		pthread_mutex_unlock( &stack->lock );
		return errEmpty; // empty
	}

	size_t tmp_sz = *( size_t * )( stack->buf + stack->top - sizeof( size_t ) );
	if ( tmp_sz + sizeof( size_t ) > stack->top )
	{
		pthread_mutex_unlock( &stack->lock );
		return errCorrupted; // corrupted
	}
	if ( size ) *size = tmp_sz;
	if ( item ) *item = ( void_p )( stack->buf + stack->top - sizeof( size_t ) - *size );

	pthread_mutex_unlock( &stack->lock );
	return errOK;
}

status vstack_pop( HDLR VStack * stack , OUTx void_p * item , OUTx size_t * size , bool * pemptied )
{
	LOCK_LINE( pthread_mutex_lock( &stack->lock ) );

	if ( stack->top == 0 )
	{
		pthread_mutex_unlock( &stack->lock );
		return errEmpty; // empty
	}

	size_t tmp_sz = *( size_t * )( stack->buf + stack->top - sizeof( size_t ) );
	if ( tmp_sz + sizeof( size_t ) > stack->top )
	{
		pthread_mutex_unlock( &stack->lock );
		return errCorrupted; // corrupted
	}
	if ( size ) *size = tmp_sz;
	stack->top -= tmp_sz + sizeof( size_t );
	if ( item ) *item = ( void_p )( stack->buf + stack->top );
	if ( pemptied ) *pemptied = ( stack->top == 0 );

	pthread_mutex_unlock( &stack->lock );
	return errOK;
}

void vstack_clear( HDLR VStack * stack , bool clear_var )
{
	if ( clear_var )
	{
		pthread_mutex_lock( &stack->lock );
		stack->top = 0;
		pthread_mutex_unlock( &stack->lock );
	}
}
