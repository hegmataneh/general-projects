#define Uses_CALLOC_AR
#define Uses_circular_sliding_rate_window
#include <general.dep>

status cwnd_rate_init( cir_rate_wnd_t * rm , size_t src_windows_size )
{
	if ( !rm || !src_windows_size ) return errArg;

	rm->window_size = src_windows_size;
	rm->bytes = CALLOC_AR( rm->bytes , src_windows_size );
	rm->sec = CALLOC_AR( rm->sec , src_windows_size );
	if ( !rm->bytes || !rm->sec )
	{
		DAC( rm->bytes );
		DAC( rm->sec );
		return errMemoryLow;
	}

	rm->last_time = 0;        // no packets yet
	rm->last_idx = -1;
	rm->total = 0;
	return errOK;
}

void cwnd_rate_free( cir_rate_wnd_t * rm )
{
	if ( !rm ) return;
	DAC( rm->bytes );
	DAC( rm->sec );
}

void cwnd_clear_all( cir_rate_wnd_t * rm )
{
	memset( rm->bytes , 0 , rm->window_size * sizeof( uint64_t ) );
	memset( rm->sec , 0 , rm->window_size * sizeof( time_t ) );
	rm->total = 0;
	rm->last_idx = -1;
	rm->last_time = 0;
}

/* Add packet of given size (bytes). This updates internal slots,
   clears intermediate seconds when time advanced, keeps running total. */
void cwnd_rate_add_packet( cir_rate_wnd_t * rm , uint64_t packet_size )
{
	if ( !rm ) return;

	time_t tnow = time( NULL );

	if ( rm->last_idx == -1 )
	{
		/* first packet ever */
		int idx = ( int )( tnow % (long)rm->window_size );
		cwnd_clear_all( rm ); /* ensure clean */
		rm->bytes[ idx ] = packet_size;
		rm->sec[ idx ] = tnow;
		rm->total = packet_size;
		rm->last_idx = idx;
		rm->last_time = tnow;
		return;
	}

	if ( tnow == rm->last_time )
	{
		/* still same second: just accumulate */
		int idx = rm->last_idx;
		rm->bytes[ idx ] += packet_size;
		rm->total += packet_size;
		return;
	}

	/* now > last_time : advance and clear intermediate slots */
	time_t gap = tnow - rm->last_time;

	if ( gap >= rm->window_size )
	{
		/* all previous data expired; whole window becomes zeros */
		cwnd_clear_all( rm );
		int idx = ( int )( tnow % (long)rm->window_size );
		rm->bytes[ idx ] = packet_size;
		rm->sec[ idx ] = tnow;
		rm->total = packet_size;
		rm->last_idx = idx;
		rm->last_time = tnow;
		return;
	}

	/* gap < window_size: clear each intermediate slot */
	for ( time_t k = 1; k <= gap; ++k )
	{
		int idx = ( int )( ( rm->last_idx + 1 ) % (long)rm->window_size );
		/* subtract old bytes from total (they may still hold old data) */
		rm->total -= rm->bytes[ idx ];
		/* reset slot */
		rm->bytes[ idx ] = 0;
		rm->sec[ idx ] = rm->last_time + k; /* assign exact timestamp for that slot */
		/* advance last_idx */
		rm->last_idx = idx;
	}

	/* now last_idx corresponds to 'now' */
	rm->bytes[ rm->last_idx ] += packet_size;
	rm->total += packet_size;
	rm->last_time = tnow;
}

bool cwnd_rate_has_item( cir_rate_wnd_t * rm )
{
	if ( !rm ) return false;
	return rm->last_idx != -1;
}

/* Returns bytes per second averaged over the full configured window.
   If you prefer to average only over non-empty seconds, modify accordingly. */
double cwnd_rate_get_bps( cir_rate_wnd_t * rm )
{
	if ( !rm ) return 0.0;
	time_t tnow = time( NULL );
	time_t gap = tnow - rm->last_time;
	if ( gap >= rm->window_size )
	{
		/* all previous data expired; whole window becomes zeros */
		cwnd_clear_all( rm );
	}

	return ( double )rm->total / ( double )rm->window_size;
}
