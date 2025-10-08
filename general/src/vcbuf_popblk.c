#define Uses_WARNING
#define Uses_ETIMEDOUT
#define Uses_MEMSET_ZERO
#define Uses_pthread_mutex_init
#define Uses_vcbuf_popblk
#include <general.dep>

#define HDR_TYPE ushort
#define HDR_SIZE 2
#define ROOM_SIZE(s) (s + HDR_SIZE)
#define HDR( p )  ( *( ( HDR_TYPE * )( p ) ) )

status vcbuf_init( vcbuf * vc , size_t capacity , size_t room_size )
{
	if ( !vc ) return errArg;

	vcbuf_destroy( vc );
	MEMSET_ZERO_O( vc );

	size_t buf_size = capacity * ( ROOM_SIZE( room_size ) );
	vc->buf = MALLOC( buf_size );
	if ( !vc->buf )
	{
		return errMemoryLow;
	}

	vc->room_size = room_size;
	vc->capacity = capacity;
	vc->head = 0;
	vc->tail = 0;
	vc->count = 0;

	pthread_mutex_init( &vc->lock , NULL );
	pthread_cond_init( &vc->not_empty , NULL );

	return errOK;
}

void vcbuf_destroy( vcbuf * vc )
{
	if ( !vc ) return;

	pthread_mutex_destroy( &vc->lock );
	pthread_cond_destroy( &vc->not_empty );

	if ( vc->buf )
	{
		DAC( vc->buf );
		MEMSET_ZERO_O( vc );
	}
}

void vcbuf_push( vcbuf * vc , const buffer buf , size_t len )
{
	WARNING( len <= vc->room_size );

	pthread_mutex_lock( &vc->lock );

	HDR( vc->buf + ( vc->head * ROOM_SIZE( vc->room_size ) ) ) = ( HDR_TYPE )len;
	MEMCPY_OR( vc->buf + ( vc->head * ROOM_SIZE( vc->room_size ) ) + HDR_SIZE , buf , len );
	vc->head = ( vc->head + 1 ) % vc->capacity;
	vc->count++;
	int full = ( vc->head == vc->tail );
	if ( full )
	{
		vc->tail = ( vc->tail + 1 ) % vc->capacity; // if overwrite old one tail go forward
	}

	pthread_cond_signal( &vc->not_empty );
	pthread_mutex_unlock( &vc->lock );
}

status vcbuf_pop( vcbuf * vc , buffer out_buf , size_t * out_len , long timeout_sec )
{
	pthread_mutex_lock( &vc->lock );

	while ( !vc->count ) // for Spurious wakeups and Races with other waiters/consumers
	{
		if ( timeout_sec < 0 )
		{
			pthread_cond_wait( &vc->not_empty , &vc->lock );
		}
		else
		{
			struct timespec ts;
			clock_gettime( CLOCK_REALTIME , &ts );
			ts.tv_sec += timeout_sec;
			int rc = pthread_cond_timedwait( &vc->not_empty , &vc->lock , &ts );
			if ( rc == ETIMEDOUT )
			{
				return errTimeout;
			}
		}
	}

	*out_len = ( size_t )HDR( vc->buf + ( vc->tail * ROOM_SIZE( vc->room_size ) ) );
	MEMCPY_OR( out_buf , vc->buf + ( vc->tail * ROOM_SIZE( vc->room_size ) ) + HDR_SIZE , *out_len );
	vc->tail = ( vc->tail + 1 ) % vc->capacity;
	vc->count--;

	pthread_mutex_unlock( &vc->lock );
	return errOK;
}