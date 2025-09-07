#define Uses_ETIMEDOUT
#define Uses_MEMSET_ZERO
#define Uses_pthread_mutex_init
#define Uses_vcbuf_nonblk
#include <general.dep>

#define HDR_TYPE ushort
#define HDR_SIZE 2
#define ROOM_SIZE(s) (s + HDR_SIZE)
#define HDR( p )  ( *( ( HDR_TYPE * )( p ) ) )

status vcbuf_nb_init( vcbuf_nb * vc , size_t capacity , size_t room_size )
{
	if ( !vc ) return errArg;

	vcbuf_nb_destroy( vc );
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
	sem_init( &vc->gateway , 0 , 0 );

	return errOK;
}

void vcbuf_nb_destroy( vcbuf_nb * vc )
{
	if ( !vc ) return;

	sem_destroy( &vc->gateway );

	if ( vc->buf )
	{
		DAC( vc->buf );
		MEMSET_ZERO_O( vc );
	}
}

status vcbuf_nb_push( vcbuf_nb * vc , const buffer buf , size_t len )
{
	ASSERT( len <= vc->room_size );

	// head should not close to tail less than 1 space . but tail could . to distinct empty from not empty
	if ( (vc->head + 1) == vc->tail || ( ( vc->head + 1 ) % vc->capacity ) == vc->tail )
	{
		vc->err_full++;
		return errGeneral; // no overwrite old one . just lost them
	}
	
	HDR( vc->buf + ( vc->head * ROOM_SIZE( vc->room_size ) ) ) = ( HDR_TYPE )len;
	MEMCPY_OR( vc->buf + ( vc->head * ROOM_SIZE( vc->room_size ) ) + HDR_SIZE , buf , len );
	vc->head = ( vc->head + 1 ) % vc->capacity;

	sem_post( &vc->gateway );
	return errOK;
}

status vcbuf_nb_pop( vcbuf_nb * vc , buffer out_buf , size_t * out_len , long timeout_sec )
{
	struct timespec ts;
	clock_gettime( CLOCK_REALTIME , &ts );
	ts.tv_sec += timeout_sec;
	if ( sem_timedwait( &vc->gateway , &ts ) < 0 )   // wait for open signal
	{
		if ( errno == ETIMEDOUT ) return errTimeout;
		int val = 0;
		if ( sem_getvalue( &vc->gateway , &val ) < 0 || val < 1 ) return errGeneral;
	}

	*out_len = ( size_t )HDR( vc->buf + ( vc->tail * ROOM_SIZE( vc->room_size ) ) );
	MEMCPY_OR( out_buf , vc->buf + ( vc->tail * ROOM_SIZE( vc->room_size ) ) + HDR_SIZE , *out_len );
	vc->tail = ( vc->tail + 1 ) % vc->capacity;
	
	return errOK;
}