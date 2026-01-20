#define Uses_timespec_diff_nsec
#define Uses_MEMSET_ZERO_O
//#define Uses_LOCK_LINE
#define Uses_instantaneousBps
#include <general.dep>

void inst_rate_init( instBps_t * rm )
{
	MEMSET_ZERO_O( rm );
	//pthread_mutex_init( &rm->lock , NULL );
}

//void inst_rate_destroy( instBps_t * rm )
//{
//	pthread_mutex_destroy( &rm->lock );
//}

void inst_rate_init2start( instBps_t * rm )
{
	MEMSET_ZERO_O( &rm->data );
}

bool inst_rate_add_packet( instBps_t * rm , size_t packet_size )
{
	//INSTANTANEOUSBPS_LOCK_LINE( pthread_mutex_lock( &rm->lock ) );
	//time_t now = time( NULL );  // current second
	timespec exact_now_tm;
	clock_gettime( CLOCK_REALTIME , &exact_now_tm );
	//bool bwindowtimechanged = false;

	if ( !rm->data.current_exact_tm.tv_sec )
	{
		// first packet
		//rm->data.current_sec = now;
		rm->data.current_exact_tm = exact_now_tm;
	}

	//if ( timeval_diff_nsec( &rm->data.current_exact_tm , &exact_now_tm ) >= 1000000000 )
	//{
	//	// a new second started → finalize last second's rate
	//	rm->data.last_rate = rm->data.bytes_this_sec;
	//	// reset for new second
	//	rm->data.bytes_this_sec = 0;
	//	//rm->data.current_sec = now;
	//	rm->data.current_exact_tm = exact_now_tm;
	//	bwindowtimechanged = true;
	//}

	// add packet to the current second
	rm->data.bytes += packet_size;
	//pthread_mutex_unlock( &rm->lock );
	//return bwindowtimechanged;
	return false;
}

//bool inst_has_item( instBps_t * rm )
//{
//	if ( !rm ) return false;
//	return rm->current_sec != 0;
//}

// get rate and check wheater store time not too many old
//size_t inst_rate_get_last( instBps_t * rm , size_t last_time_timeout )
//{
//	INSTANTANEOUSBPS_LOCK_LINE( pthread_mutex_lock( &rm->lock ) );
//	if ( last_time_timeout )
//	{
//		timespec exact_now_tm;
//		clock_gettime( CLOCK_REALTIME , &exact_now_tm );
//
//		//time_t now = time( NULL );
//		//if ( now - rm->data.current_sec > last_time_timeout )
//		if ( timeval_diff_nsec( &rm->data.current_exact_tm , &exact_now_tm ) > last_time_timeout * 1000000000 )
//		{
//			MEMSET_ZERO_O( &rm->data );
//		}
//	}
//	size_t i = rm->data.last_rate;
//	pthread_mutex_unlock( &rm->lock );
//	return i;
//}

status inst_rate_loadBps( instBps_t * rm , size_t * Bps , double * elapsed_sec )
{
	//INSTANTANEOUSBPS_LOCK_LINE( pthread_mutex_lock( &rm->lock ) );
	if ( rm->data.current_exact_tm.tv_sec )
	{
		timespec exact_now_tm;
		clock_gettime( CLOCK_REALTIME , &exact_now_tm );
		double llsec = ( (double)timespec_diff_nsec( &rm->data.current_exact_tm , &exact_now_tm ) ) / 1000000000.0;
		if ( llsec > 0.0 )
		{
			*elapsed_sec = llsec;
			*Bps = ( size_t )( ( (double)rm->data.bytes ) / llsec );
			//pthread_mutex_unlock( &rm->lock );
			return errOK;
		}
	}
	//pthread_mutex_unlock( &rm->lock );
	return errInvalidData;
}
