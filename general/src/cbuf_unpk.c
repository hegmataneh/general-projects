#define Uses_ETIMEDOUT
#define Uses_MEMSET_ZERO
#define Uses_pthread_mutex_init
#define Uses_cbuf_unpk
#include <general.dep>

#define HDR_TYPE ushort
#define HDR_SIZE 2
#define ROOM_SIZE(s) (s + HDR_SIZE)
#define HDR( p )  ( *( ( HDR_TYPE * )( p ) ) )

status cbuf_unpk_init( cbuf_unpk * vc , size_t capacity , size_t room_size )
{
	if ( !vc ) return errArg;
	//if ( ( capacity & ( capacity - 1 ) ) != 0 )
	//{
	//	WARNING( 0 );
	//	return errArg; // Capacity must be power of two
	//}

	//vcbuf_nb_destroy( vc );
	MEMSET_ZERO_O( vc );

	size_t buf_size = capacity * ( ROOM_SIZE( room_size ) );
	vc->buf = CALLOC_AR( vc->buf , buf_size );
	if ( !vc->buf )
	{
		return errMemoryLow;
	}

	vc->room_size = room_size;
	vc->capacity = capacity;
	vc->head = 0;
	vc->tail = 0;
	sem_init( &vc->gateway , 0 , 0 );

	//perr_full = &vc->err_full;

	return errOK;
}

void cbuf_unpk_destroy( cbuf_unpk * vc )
{
	if ( !vc ) return;

	sem_destroy( &vc->gateway );

	if ( vc->buf )
	{
		DAC( vc->buf );
		MEMSET_ZERO_O( vc );
	}
}

status cbuf_unpk_push( cbuf_unpk * vc , const buffer buf , size_t len )
{
	WARNING( len <= vc->room_size );

	// head should not close to tail less than 1 space . but tail could . to distinct empty from not empty
	if ( (vc->head + 1) == vc->tail || ( ( vc->head + 1 ) % ( vc->capacity ) ) == vc->tail )
	{
		vc->err_full++;
		return errGeneral; // no overwrite old one . just lost them
	}
	
	HDR( vc->buf + ( vc->head * ROOM_SIZE( vc->room_size ) ) ) = ( HDR_TYPE )len;
	MEMCPY_OR( vc->buf + ( vc->head * ROOM_SIZE( vc->room_size ) ) + HDR_SIZE , buf , len );
	vc->head = ( vc->head + 1 ) % vc->capacity; // slow
	//vc->head = ( vc->head + 1 ) & ( vc->capacity - 1 ); // faster . but buffer should be power of two

	sem_post( &vc->gateway );
	return errOK;
}

status cbuf_unpk_pop( cbuf_unpk * vc , buffer out_buf , size_t * out_len , long timeout_sec )
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
	//vc->tail = ( vc->tail + 1 ) & ( vc->capacity - 1 );
	
	return errOK;
}
