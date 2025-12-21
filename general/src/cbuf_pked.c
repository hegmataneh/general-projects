#define Uses_snprintf
#define Uses_mmap
#define Uses_memset
#define Uses_open
#define Uses_errno
#define Uses_write
#define Uses_memmap

#define Uses_sprintf
#define Uses_CALLOC_AR
#define Uses_sem_wait_with_timeout
#define Uses_ETIMEDOUT
#define Uses_MEMSET_ZERO
#define Uses_pthread_mutex_init
#define Uses_cbuf_pked
#include <general.dep>

#define BUF_SIZE_LEN 2

// helper: advance index in circular buffer
_PRIVATE_FXN inline size_t advance_index( cbuf_pked_t * vc , size_t idx , size_t step )
{
	return ( idx + step ) % vc->buf_sz;
}

// compute free space
_PRIVATE_FXN size_t free_space( cbuf_pked_t * vc , bool actual_space )
{
	if ( vc->head >= vc->tail )
		return vc->buf_sz - ( vc->head - vc->tail ) - ( actual_space ? 0 : 1 )/*prevent to allow head and tail overlap on each other*/;
	else
		return ( vc->tail - vc->head ) - ( actual_space ? 0 : 1 );
}

//void * __file_map = NULL;
//int last_pos = 0;

status cbuf_pked_init( cbuf_pked_t * vc , size_t buf_sz , volatile bool * app_closed_signal )
{
	if ( !vc ) return errArg;

	MEMSET_ZERO_O( vc );
	vc->buf = CALLOC_AR( vc->buf , buf_sz );
	if ( !vc->buf )
	{
		return errMemoryLow;
	}

	vc->pAppShutdown = app_closed_signal;
	vc->buf_sz = buf_sz;
	vc->head = 0;
	vc->tail = 0;
	sem_init( &vc->gateway , 0 , 0 );

	//int flags = O_RDWR;
	//flags |= O_CREAT;
	//int fd = open( "./out_push.txt" , flags , 0644 );
	//if ( fd >= 0 )
	//{
	//	__file_map = mmap( NULL , ( ( size_t )128 * 1024 * 1024 ) , PROT_READ | PROT_WRITE , MAP_SHARED , fd , 0 );

	//	off_t cur = lseek( fd , 0 , SEEK_END );
	//	if ( ( size_t )cur < ( ( size_t )128 * 1024 * 1024 ) )
	//	{
	//		ftruncate( fd , ( __off_t )( ( size_t )128 * 1024 * 1024 ) );
	//	}

	//	memset( __file_map , 0 , ( ( size_t )128 * 1024 * 1024 ) );
	//}


	return errOK;
}

void cbuf_pked_destroy( cbuf_pked_t * vc )
{
	if ( !vc ) return;

	sem_destroy( &vc->gateway );

	if ( vc->buf )
	{
		FREE( vc->buf );
		MEMSET_ZERO_O( vc );
	}
}

#ifdef ENABLE_USE_DBG_TAG
//_GLOBAL_VAR static int _counter = 0;
//_GLOBAL_VAR static int _sum_len = 0;
//
//_GLOBAL_VAR static int _pop_count = 0;
//_GLOBAL_VAR static int _pop_size = 0;
#endif


status cbuf_pked_push( cbuf_pked_t * vc , const buffer buf , size_t buf_len , size_t alloc_len , _RET_VAL_P size_t * ring_addr , bool auto_opengate )
{

	//printf( "%zu , %zu \n" , buf_len , alloc_len );
	
	#ifdef ENABLE_USE_DBG_TAG
		//_sum_len += buf_len;
	#endif

	if ( !buf_len )
	{
		return errGeneral;
	}
	if ( alloc_len > vc->buf_sz || buf_len > vc->buf_sz || buf_len > alloc_len ) return errGeneral; // too big

	size_t need = BUF_SIZE_LEN + alloc_len; // size field + data
	size_t space = free_space( vc , false ); // prevent head exceed tail

	if ( space < need )
	{
		vc->err_full++;
		return errGeneral; // buffer full
	}

	size_t tmp_head = vc->head;

	// write size (2 bytes)
	if ( tmp_head + BUF_SIZE_LEN <= vc->buf_sz )
	{
		memcpy( &vc->buf[ tmp_head ] , &alloc_len , BUF_SIZE_LEN );

		//last_pos += sprintf( ( char * )__file_map + last_pos , "(%zu,%d)" , tmp_head , ( int )BUF_SIZE_LEN );

	}
	else
	{
		// wrap-around write for size
		size_t first = vc->buf_sz - tmp_head; // first part
		memcpy( &vc->buf[ tmp_head ] , &alloc_len , first );
		memcpy( &vc->buf[ 0 ] , ( ( uint8 * )&alloc_len ) + first , BUF_SIZE_LEN - first );

		//last_pos += sprintf( ( char * )__file_map + last_pos , "((%zu,%d)" , tmp_head , ( int )first );
		//last_pos += sprintf( ( char * )__file_map + last_pos , "((%d,%d)" , ( int )0 , ( int )( BUF_SIZE_LEN - first ) );

	}
	if ( ring_addr ) *ring_addr = tmp_head;
	tmp_head = advance_index( vc , tmp_head , BUF_SIZE_LEN );

	// write data
	if ( tmp_head + buf_len <= vc->buf_sz )
	{
		memcpy( &vc->buf[ tmp_head ] , buf , buf_len );

		//last_pos += sprintf( ( char * )__file_map + last_pos , "(%zu,%d)" , tmp_head , ( int )buf_len );
	}
	else
	{
		size_t first = vc->buf_sz - tmp_head;
		memcpy( &vc->buf[ tmp_head ] , buf , first );
		memcpy( &vc->buf[ 0 ] , ( ( uint8 * )buf ) + first , buf_len - first );

		//last_pos += sprintf( ( char * )__file_map + last_pos , "((%zu,%d)" , tmp_head , ( int )first );
		//last_pos += sprintf( ( char * )__file_map + last_pos , "((%d,%d)" , ( int )0 , ( int )( buf_len - first ) );

	}
	tmp_head = advance_index( vc , tmp_head , alloc_len );
	vc->head = tmp_head;
	if ( auto_opengate )
	{
		sem_post( &vc->gateway ); // signal consumer . increments the semaphore
	}

	#ifdef ENABLE_USE_DBG_TAG
		//_counter++;
	#endif

	return 0;
}

status cbuf_pked_pop( cbuf_pked_t * vc , void * out_buf , size_t expectation_size /*zero to no exp*/ , OUTx size_t * out_len , long timeout_sec , bool auto_checkgate )
{
#ifdef ENABLE_USE_DBG_TAG
	//_pop_count++;
#endif
	
	//struct timespec ts;
	//clock_gettime( CLOCK_REALTIME , &ts );
	//ts.tv_sec += timeout_sec;
	
	if ( auto_checkgate )
	{
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

		} while( !free_space( vc , true ) );
	}

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

	if ( expectation_size && size16 != expectation_size )
	{
		vc->tail = advance_index( vc , vc->tail , size16 );
		return errCorrupted;
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

	#ifdef ENABLE_USE_DBG_TAG
		//_pop_size += size16 + BUF_SIZE_LEN;
	#endif

	return errOK;
}

// does not advance pointer. just copy
status cbuf_pked_blindcopy( cbuf_pked_t * vc , void * out_buf , size_t block_sz_pos )
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
	//block_sz_pos = advance_index( vc , block_sz_pos , size16 );
	return errOK;
}

int cbuf_pked_unreliable_sem_count( cbuf_pked_t * vc )
{
	if ( !vc ) return 0;
	int val = 0;
	if ( sem_getvalue( &vc->gateway , &val ) < 0 || val < 1 ) return 0;
	return val;
}
