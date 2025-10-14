#define Uses_sem_wait_with_timeout
#define Uses_ETIMEDOUT
#define Uses_MEMSET_ZERO
#define Uses_pthread_mutex_init
#define Uses_cbuf_pked
#include <general.dep>

#define BUF_SIZE_LEN 2

// helper: advance index in circular buffer
_PRIVATE_FXN inline size_t advance_index( cbuf_pked * vc , size_t idx , size_t step )
{
	return ( idx + step ) % vc->buf_sz;
}

// compute free space
_PRIVATE_FXN size_t free_space( cbuf_pked * vc )
{
	if ( vc->head >= vc->tail )
		return vc->buf_sz - ( vc->head - vc->tail ) - 1/*prevent to allow head and tail overlap on each other*/;
	else
		return ( vc->tail - vc->head ) - 1;
}

status cbuf_pked_init( cbuf_pked * vc , size_t buf_sz , bool * app_closed_signal )
{
	if ( !vc ) return errArg;

	MEMSET_ZERO_O( vc );
	vc->buf = CALLOC_AR( vc->buf , buf_sz );
	if ( !vc->buf )
	{
		return errMemoryLow;
	}

	vc->buf_sz = buf_sz;
	vc->head = 0;
	vc->tail = 0;
	sem_init( &vc->gateway , 0 , 0 );
	vc->pAppShutdown = app_closed_signal;

	return errOK;
}

void cbuf_pked_destroy( cbuf_pked * vc )
{
	if ( !vc ) return;

	sem_destroy( &vc->gateway );

	if ( vc->buf )
	{
		FREE( vc->buf );
		MEMSET_ZERO_O( vc );
	}
}

status cbuf_pked_push( cbuf_pked * vc , const buffer buf , size_t buf_len , size_t alloc_len , _RET_VAL_P size_t * ring_addr )
{
	if ( alloc_len > vc->buf_sz || buf_len > vc->buf_sz || buf_len > alloc_len ) return errGeneral; // too big

	size_t need = BUF_SIZE_LEN + alloc_len; // size field + data
	size_t space = free_space( vc );

	if ( space < need )
	{
		vc->err_full++;
		return errGeneral; // buffer full
	}

	// write size (2 bytes)
	if ( vc->head + BUF_SIZE_LEN <= vc->buf_sz )
	{
		memcpy( &vc->buf[ vc->head ] , &buf_len , BUF_SIZE_LEN );
	}
	else
	{
		// wrap-around write for size
		size_t first = vc->buf_sz - vc->head; // first part
		memcpy( &vc->buf[ vc->head ] , &buf_len , first );
		memcpy( &vc->buf[ 0 ] , ( ( uint8 * )&buf_len ) + first , BUF_SIZE_LEN - first );
	}
	if ( ring_addr ) *ring_addr = vc->head;
	vc->head = advance_index( vc , vc->head , BUF_SIZE_LEN );

	// write data
	if ( vc->head + buf_len <= vc->buf_sz )
	{
		memcpy( &vc->buf[ vc->head ] , buf , buf_len );
	}
	else
	{
		size_t first = vc->buf_sz - vc->head;
		memcpy( &vc->buf[ vc->head ] , buf , first );
		memcpy( &vc->buf[ 0 ] , ( ( uint8 * )buf ) + first , buf_len - first );
	}
	vc->head = advance_index( vc , vc->head , alloc_len );

	sem_post( &vc->gateway ); // signal consumer . increments the semaphore
	return 0;
}

status cbuf_pked_pop( cbuf_pked * vc , void * out_buf , size_t * out_len , long timeout_sec )
{
	//struct timespec ts;
	//clock_gettime( CLOCK_REALTIME , &ts );
	//ts.tv_sec += timeout_sec;
	
	do
	{
		// TOTEST more
		status ret = sem_wait_with_timeout( &vc->gateway , timeout_sec , vc->pAppShutdown );
		switch ( ret )
		{
			case errOK:
			{
				break;
			}
			default: return ret;
		}

		//if ( sem_timedwait( &vc->gateway , &ts ) < 0 ) // wait for open signal . decrements the semaphore . if zero wait
		//{
		//	if ( errno == ETIMEDOUT ) return errTimeout;
		//	int val = 0;
		//	if ( sem_getvalue( &vc->gateway , &val ) < 0 || val < 1 ) return errGeneral;
		//}

	} while( vc->head == vc->tail );

	// read size
	uint16 size16;
	if ( vc->tail + BUF_SIZE_LEN <= vc->buf_sz )
	{
		memcpy( &size16 , &vc->buf[ vc->tail ] , BUF_SIZE_LEN );
	}
	else
	{
		size_t first = vc->buf_sz - vc->tail;
		memcpy( &size16 , &vc->buf[ vc->tail ] , first );
		memcpy( ( ( uint8 * )&size16 ) + first , &vc->buf[ 0 ] , BUF_SIZE_LEN - first );
	}
	vc->tail = advance_index( vc , vc->tail , BUF_SIZE_LEN );

	if ( size16 > vc->buf_sz )
	{
		return errOverflow; // corrupted
	}

	// read data
	if ( out_buf )
	{
		if ( vc->tail + size16 <= vc->buf_sz )
		{
			memcpy( out_buf , &vc->buf[ vc->tail ] , size16 );
		}
		else
		{
			size_t first = vc->buf_sz - vc->tail;
			memcpy( out_buf , &vc->buf[ vc->tail ] , first );
			memcpy( ( ( uint8 * )out_buf ) + first , &vc->buf[ 0 ] , size16 - first );
		}
	}
	vc->tail = advance_index( vc , vc->tail , size16 );
	if ( out_len ) *out_len = size16;
	return errOK;
}

// does not advance pointer. just copy
status cbuf_pked_blindcopy( cbuf_pked * vc , void * out_buf , size_t block_sz_pos )
{
	// read size
	uint16 size16;
	if ( block_sz_pos + BUF_SIZE_LEN <= vc->buf_sz )
	{
		memcpy( &size16 , &vc->buf[ block_sz_pos ] , BUF_SIZE_LEN );
	}
	else
	{
		size_t first = vc->buf_sz - block_sz_pos;
		memcpy( &size16 , &vc->buf[ block_sz_pos ] , first );
		memcpy( ( ( uint8 * )&size16 ) + first , &vc->buf[ 0 ] , BUF_SIZE_LEN - first );
	}
	block_sz_pos = advance_index( vc , block_sz_pos , BUF_SIZE_LEN );

	if ( size16 > vc->buf_sz )
	{
		return errOverflow; // corrupted
	}

	// read data
	if ( block_sz_pos + size16 <= vc->buf_sz )
	{
		memcpy( out_buf , &vc->buf[ block_sz_pos ] , size16 );
	}
	else
	{
		size_t first = vc->buf_sz - block_sz_pos;
		memcpy( out_buf , &vc->buf[ block_sz_pos ] , first );
		memcpy( ( ( uint8 * )out_buf ) + first , &vc->buf[ 0 ] , size16 - first );
	}
	block_sz_pos = advance_index( vc , block_sz_pos , size16 );
	return errOK;
}
