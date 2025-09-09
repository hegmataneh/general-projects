#define Uses_MEMSET_ZERO
#define Uses_pthread_mutex_init
#define Uses_cirbuf_lockfree
#include <general.dep>

#define HDR_TYPE ushort
#define HDR_SIZE 2
#define ROOM_SIZE(s) (s + HDR_SIZE)
#define HDR( p )  ( *( ( HDR_TYPE * )( p ) ) )

status cbuf_lf_init( cbuf_lf * cblf , size_t capacity , size_t room_size , l_pkg * plpkg )
{
	cbuf_lf_free( cblf );
	MEMSET_ZERO_O( cblf );

	cblf->lock_pkg = plpkg;

	size_t buf_size = capacity * ( ROOM_SIZE( room_size ) );
	cblf->buf = MALLOC( buf_size );
	if ( !cblf->buf )
	{
		return errMemoryLow;
	}

	cblf->room_size = room_size;
	cblf->capacity = capacity;
	cblf->head = 0;
	cblf->tail = 0;
	cblf->full = 0;

	if ( cblf->lock_pkg && cblf->lock_pkg->clb_init )
	{
		cblf->lock_pkg->clb_init( cblf->lock_pkg->clb_pdata );
	}
	//pthread_mutex_init( &cblf->lock , NULL );

	return errOK;
}

void cbuf_lf_free( cbuf_lf * cblf )
{
	if ( cblf )
	{
		if ( cblf->lock_pkg && cblf->lock_pkg->clb_destroy )
		{
			cblf->lock_pkg->clb_destroy( cblf->lock_pkg->clb_pdata );
		}
		//pthread_mutex_destroy( &cblf->lock );
		DAC( cblf->buf );
		MEMSET_ZERO_O( cblf );
	}
}

void cbuf_lf_push( cbuf_lf * cblf , const void_p data , size_t data_len , int caller_thread_data /*=0 passed to lock*/ )
{
	ASSERT( data_len <= cblf->room_size );
	if ( cblf->lock_pkg )
	{
		cblf->lock_pkg->clb_lock( cblf->lock_pkg->clb_pdata , caller_thread_data );
	}
	//pthread_mutex_lock( &cblf->lock );

	HDR( cblf->buf + ( cblf->head * ROOM_SIZE( cblf->room_size ) ) ) = ( HDR_TYPE )data_len;
	MEMCPY_OR( cblf->buf + ( cblf->head * ROOM_SIZE( cblf->room_size ) ) + HDR_SIZE , data , data_len );
	cblf->head = ( cblf->head + 1 ) % cblf->capacity;
	cblf->full = ( cblf->head == cblf->tail );
	if ( cblf->full )
	{
		cblf->tail = ( cblf->tail + 1 ) % cblf->capacity; // overwrite oldest
	}

	if ( cblf->lock_pkg )
	{
		cblf->lock_pkg->clb_unlock( cblf->lock_pkg->clb_pdata , caller_thread_data );
	}
	//pthread_mutex_unlock( &cblf->lock );
}

status cbuf_lf_pop( cbuf_lf * cblf , void_p item , size_t *data_len , int caller_thread_data /*=0 passed to lock*/ )
{
	if ( cblf->lock_pkg )
	{
		cblf->lock_pkg->clb_lock( cblf->lock_pkg->clb_pdata , caller_thread_data );
	}
	//pthread_mutex_lock( &cblf->lock );

	if ( cblf->head == cblf->tail && !cblf->full )
	{
		if ( cblf->lock_pkg )
		{
			cblf->lock_pkg->clb_unlock( cblf->lock_pkg->clb_pdata , caller_thread_data );
		}
		//pthread_mutex_unlock( &cblf->lock );
		return errGeneral; // empty
	}

	*data_len = ( size_t )HDR( cblf->buf + ( cblf->tail * ROOM_SIZE( cblf->room_size ) ) );
	MEMCPY_OR( item , cblf->buf + ( cblf->tail * ROOM_SIZE( cblf->room_size ) ) + HDR_SIZE , *data_len );
	cblf->full = 0;
	cblf->tail = ( cblf->tail + 1 ) % cblf->capacity;

	if ( cblf->lock_pkg )
	{
		cblf->lock_pkg->clb_unlock( cblf->lock_pkg->clb_pdata , caller_thread_data );
	}
	//pthread_mutex_unlock( &cblf->lock );

	return errOK;
}

int cbuf_lf_empty( cbuf_lf * cblf , int caller_thread_data /*=0 passed to lock*/ )
{
	if ( cblf->lock_pkg )
	{
		cblf->lock_pkg->clb_lock( cblf->lock_pkg->clb_pdata , caller_thread_data );
	}
	//pthread_mutex_lock( &cblf->lock );

	int empty = ( !cblf->full && ( cblf->head == cblf->tail ) );
	
	if ( cblf->lock_pkg )
	{
		cblf->lock_pkg->clb_unlock( cblf->lock_pkg->clb_pdata , caller_thread_data );
	}
	//pthread_mutex_unlock( &cblf->lock );

	return empty;
}

//int cbuf_lf_full( cbuf_lf * cblf )
//{
//	if ( cblf->lock_pkg )
//	{
//		cblf->lock_pkg->clb_lock( cblf->lock_pkg->clb_pdata );
//	}
//	//pthread_mutex_lock( &cblf->lock );
//
//	int f = cblf->full;
//
//	if ( cblf->lock_pkg )
//	{
//		cblf->lock_pkg->clb_unlock( cblf->lock_pkg->clb_pdata );
//	}
//	//pthread_mutex_unlock( &cblf->lock );
//	return f;
//}
//
//size_t cbuf_lf_size( cbuf_lf * cblf )
//{
//	if ( cblf->lock_pkg )
//	{
//		cblf->lock_pkg->clb_lock( cblf->lock_pkg->clb_pdata );
//	}
//	//pthread_mutex_lock( &cblf->lock );
//
//	size_t size = cblf->capacity;
//
//	if ( !cblf->full )
//	{
//		if ( cblf->head >= cblf->tail )
//		{
//			size = cblf->head - cblf->tail;
//		}
//		else
//		{
//			size = cblf->capacity + cblf->head - cblf->tail;
//		}
//	}
//
//	if ( cblf->lock_pkg )
//	{
//		cblf->lock_pkg->clb_unlock( cblf->lock_pkg->clb_pdata );
//	}
//	//pthread_mutex_unlock( &cblf->lock );
//
//	return size;
//}

