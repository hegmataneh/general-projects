#define Uses_floor
#define Uses_MEMSET_ZERO
#define Uses_cbuf_metric
#include <general.dep>

status cbuf_m_init( cbuf_metr * buf , size_t capacity )
{
	if ( !buf || capacity == 0 ) return errArg;
	buf->samples = ( uint64 * )CALLOC( capacity , sizeof( uint64 ) );
	if ( !buf->samples ) return errMemoryLow;
	buf->capacity = capacity;
	buf->head = 0;
	buf->filled = 0;
	return errOK;
}

void cbuf_m_free( cbuf_metr * buf )
{
	if ( !buf ) return;
	DAC( buf->samples );
	buf->samples = NULL;
	buf->capacity = 0;
	buf->head = 0;
	buf->filled = 0;
}

void cbuf_m_reset( cbuf_metr * buf )
{
	if ( !buf || !buf->samples ) return;
	MEMSET( buf->samples , 0 , buf->capacity * sizeof( uint64 ) );
	buf->head = 0;
	buf->filled = 0;
}

void cbuf_m_advance( cbuf_metr * buf , uint64 count )
{
	if ( !buf || !buf->samples ) return;
	buf->samples[ buf->head ] = count;
	buf->head = ( buf->head + 1 ) % buf->capacity;
	if ( buf->filled < buf->capacity )
	{
		buf->filled++;
	}
}

/*
The most recent item is at head - 1
The oldest item is at: head - filled
------
buf->head + buf->capacity - buf->filled + i
Meaning:
buf->head - buf->filled → index of first (oldest) valid sample (may be negative)
+ buf->capacity → converts possibly-negative index into positive range (makes wrap-around work)
+ i → move forward to the next stored sample
Finally % capacity makes sure it wraps around inside the circular buffer.
*/
uint64 cbuf_m_sum_all( const cbuf_metr * buf )
{
	if ( !buf || !buf->samples || buf->filled == 0 ) return 0;

	uint64 sum = 0;
	for ( size_t i = 0; i < buf->filled; ++i )
	{
		size_t idx = ( buf->head + buf->capacity - buf->filled + i ) % buf->capacity; // The most recent item is at head - 1 . The oldest item is at : head - filled
		sum += buf->samples[ idx ];
	}
	return sum;
}

float cbuf_m_mean_all( const cbuf_metr * buf )
{
	if ( !buf || !buf->samples || buf->filled == 0 ) return 0.0f;

	uint64 sum = cbuf_m_sum_all( buf );
	return ( float )sum / ( float )buf->filled;
}

uint64 cbuf_m_max( const cbuf_metr * buf )
{
	if ( !buf || !buf->samples || buf->filled == 0 ) return 0;

	uint64 maxx = 0;
	for ( size_t i = 0; i < buf->filled; ++i )
	{
		size_t idx = ( buf->head + buf->capacity - buf->filled + i ) % buf->capacity; // The most recent item is at head - 1 . The oldest item is at : head - filled
		if ( buf->samples[ idx ] > maxx )
		{
			maxx = buf->samples[ idx ];
		}
	}
	return maxx;
}

int cbuf_m_regression_slope_all( const cbuf_metr * buf )
{
	if ( !buf || !buf->samples || buf->filled < 2 ) return 0;

	double sum_x = 0 , sum_y = 0 , sum_xy = 0 , sum_x2 = 0;
	double slope;

	for ( size_t i = 0; i < buf->filled; ++i )
	{
		size_t idx = ( buf->head + buf->capacity - buf->filled + i ) % buf->capacity;

		double x = ( double )( i + 1 ); // x = 1..n
		sum_x += ( double )( x );
		sum_y += ( double )( buf->samples[ idx ] );
		sum_xy += x * ( double )( buf->samples[ idx ] );
		sum_x2 += ( double )( x * x );
	}

	slope = ( ( double )( buf->filled ) * sum_xy - sum_x * sum_y ) / ( ( double )( buf->filled ) * sum_x2 - sum_x * sum_x );
	return ( int )floor( slope );
}
