#define Uses_is_double_zero
#define Uses_sqrt
#define Uses_CALLOC_AR
#define Uses_circular_sliding_input_window
#include <general.dep>

status cr_in_wnd_init( cr_in_wnd_t * rm , size_t src_windows_size )
{
	if ( !rm || !src_windows_size ) return errArg;

	rm->window_size = src_windows_size;
	rm->bytes = CALLOC_AR( rm->bytes , src_windows_size );
	if ( !rm->bytes )
	{
		DAC( rm->bytes );
		return errMemoryLow;
	}

	rm->last_time = 0;        // no packets yet
	rm->last_idx = -1; IMPORTANT
	rm->total_pkt_sz = 0;
	rm->filled_count = 0;
	pthread_mutex_init( &rm->lock , NULL );
	return errOK;
}

_PRIVATE_FXN void cr_in_wnd_clear_all( cr_in_wnd_t * rm )
{
	MEMSET_ZERO( rm->bytes , rm->window_size );
	rm->total_pkt_sz = 0;
	rm->last_idx = -1; IMPORTANT
	rm->last_time = 0;
	rm->filled_count = 0;
}

void cr_in_wnd_free( cr_in_wnd_t * rm )
{
	if ( !rm ) return;
	cr_in_wnd_clear_all( rm );
	pthread_mutex_destroy( &rm->lock );
	DAC( rm->bytes );
}

/* Add packet of given size (bytes). This updates internal slots,
   clears intermediate seconds when time advanced, keeps running total. */
void cr_in_wnd_add_packet( cr_in_wnd_t * rm , size_t packet_size )
{
	if ( !rm ) return;

	pthread_mutex_lock( &rm->lock );

	time_t tnow = time( NULL );

	if ( rm->last_idx == -1 )
	{
		/* first packet ever */
		int idx = ( int )( tnow % ( long )rm->window_size );
		cr_in_wnd_clear_all( rm ); /* ensure clean */
		rm->bytes[ idx ].byte = packet_size;
		rm->bytes[ idx ].sec = tnow;
		rm->total_pkt_sz = packet_size;
		rm->last_idx = idx;
		rm->last_time = tnow;
		rm->filled_count = 1;
		pthread_mutex_unlock( &rm->lock );
		return;
	}

	if ( tnow == rm->last_time )
	{
		/* still same second: just accumulate */
		int idx = rm->last_idx;
		rm->bytes[ idx ].byte += packet_size;
		rm->total_pkt_sz += packet_size;
		pthread_mutex_unlock( &rm->lock );
		return;
	}

	/* now > last_time : advance and clear intermediate slots */
	time_t gap = tnow - rm->last_time;

	if ( gap >= rm->window_size )
	{
		/* all previous data expired; whole window becomes zeros */
		cr_in_wnd_clear_all( rm );
		int idx = ( int )( tnow % (long)rm->window_size );
		rm->bytes[ idx ].byte = packet_size;
		rm->bytes[ idx ].sec = tnow;
		rm->total_pkt_sz = packet_size;
		rm->last_idx = idx;
		rm->last_time = tnow;
		rm->filled_count = 1;
		pthread_mutex_unlock( &rm->lock );
		return;
	}

	/* gap < window_size: clear each intermediate slot */
	for ( time_t k = 1; k <= gap; ++k )
	{
		int idx = ( int )( ( rm->last_idx + 1 ) % (long)rm->window_size );
		/* subtract old bytes from total (they may still hold old data) */
		rm->total_pkt_sz -= rm->bytes[ idx ].byte;
		
		/* If slot was active, decrease filled */
		if ( rm->bytes[ idx ].sec != 0 &&
			( rm->last_time - rm->bytes[ idx ].sec < rm->window_size ) )
		{
			rm->filled_count--;
		}
		
		/* reset slot */
		rm->bytes[ idx ].byte = 0;
		rm->bytes[ idx ].sec = rm->last_time + k; /* assign exact timestamp for that slot */
		
		/* this newly created second is active */
		rm->filled_count++;
		
		/* advance last_idx */
		rm->last_idx = idx;
	}

	/* now last_idx corresponds to 'now' */
	rm->bytes[ rm->last_idx ].byte += packet_size;
	rm->total_pkt_sz += packet_size;
	rm->last_time = tnow;
	pthread_mutex_unlock( &rm->lock );
}

//int cr_in_wnd_regression( cr_in_wnd_t * rm )
//{
//	if ( rm->last_idx == -1 || rm->filled_count < 2 ) return 0;
//
//	double sum_x = 0 , sum_y = 0 , sum_xy = 0 , sum_x2 = 0;
//	double slope;
//
//	for ( size_t i = 0; i < rm->filled_count ; ++i )
//	{
//		size_t idx = ( rm->last_idx - rm->filled_count + rm->window_size + i ) % rm->window_size;
//
//		double x = ( double )( i + 1 ); // x = 1..n
//		sum_x += ( double )( x );
//		sum_y += ( double )( rm->bytes[ idx ] );
//		sum_xy += x * ( double )( rm->bytes[ idx ] );
//		sum_x2 += ( double )( x * x );
//	}
//
//	slope = ( ( double )( rm->filled_count ) * sum_xy - sum_x * sum_y ) / ( ( double )( rm->filled_count ) * sum_x2 - sum_x * sum_x );
//
//	// Return slope as integer (rounded)
//	return ( int )( slope + ( slope >= 0 ? 0.5 : -0.5 ) );
//}

//bool cr_in_wnd_has_item( cr_in_wnd_t * rm )
//{
//	if ( !rm ) return false;
//	return rm->last_idx != -1;
//}

/* Returns bytes per second averaged over the full configured window.
   If you prefer to average only over non-empty seconds, modify accordingly. */
double cr_in_wnd_get_bps( cr_in_wnd_t * rm )
{
	if ( !rm ) return 0.0;
	pthread_mutex_lock( &rm->lock );
	time_t tnow = time( NULL );
	time_t gap = tnow - rm->last_time;
	if ( gap >= rm->window_size )
	{
		/* all previous data expired; whole window becomes zeros */
		cr_in_wnd_clear_all( rm );
	}

	double d = ( double )rm->total_pkt_sz / ( double )rm->window_size;
	pthread_mutex_unlock( &rm->lock );
	return d;
}

size_t cr_in_wnd_get_ordered_items( cr_in_wnd_t * rm , cr_wnd_slide * buffer_bytes )
{
	pthread_mutex_lock( &rm->lock );
	if ( rm->last_idx == -1 || rm->filled_count < 1 )
	{
		pthread_mutex_unlock( &rm->lock );
		return 0;
	}

	size_t idx = ( ( size_t )rm->last_idx - rm->filled_count + rm->window_size ) % rm->window_size;
	memcpy( buffer_bytes , &rm->bytes[ idx ] , sizeof( cr_wnd_slide ) * ( rm->window_size - idx ) );
	memcpy( buffer_bytes + ( rm->window_size - idx ) , rm->bytes , sizeof( cr_wnd_slide ) * ( idx ) );
	size_t ret = rm->filled_count;
	pthread_mutex_unlock( &rm->lock );
	return ret;
}

double cr_in_wnd_calc_pearson_correlation( cr_in_wnd_t * rw1 , cr_in_wnd_t * rw2 )
{
	cr_wnd_slide * buf1 = CALLOC_AR( buf1 , rw1->window_size );
	cr_wnd_slide * buf2 = CALLOC_AR( buf2 , rw2->window_size );
	if ( !buf1 || !buf2 ) return 0;
	
	size_t ret1 = cr_in_wnd_get_ordered_items( rw1 , buf1 );
	size_t ret2 = cr_in_wnd_get_ordered_items( rw2 , buf2 );
	if ( ret1 < 2 || ret2 < 2 )
	{
		DAC( buf1 );
		DAC( buf2 );
		return 0;
	}

	double mean_1 = 0.0;
	double mean_2 = 0.0;

	// 1) mean
	for ( size_t i = 0 ; i < rw1->window_size ; i++ )
	{
		mean_1 += ( double )buf1[ i ].byte;
		mean_2 += ( double )buf2[ i ].byte;
	}
	mean_1 /= ( double )rw1->window_size;
	mean_2 /= ( double )rw2->window_size;

	// 2) numerator and denominators
	double num = 0.0;
	double den1 = 0.0;
	double den2 = 0.0;

	for ( size_t i = 0 ; i < rw1->window_size ; i++ )
	{
		double dx = ( double )buf1[ i ].byte - mean_1;
		double dy = ( double )buf2[ i ].byte - mean_2;

		num += dx * dy;
		den1 += dx * dx;
		den2 += dy * dy;
	}
	DAC( buf1 );
	DAC( buf2 );

	if ( is_double_zero( den1 ) || is_double_zero( den2 ) )
	{
		return 0.0;
	}

	return num / sqrt( den1 * den2 );
}
