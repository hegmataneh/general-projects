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

uint64 cbuf_m_sum_last( const cbuf_metr * buf , size_t last_n )
{
	if ( !buf || !buf->samples || buf->filled == 0 ) return 0;

	size_t n = ( last_n < buf->filled ) ? last_n : buf->filled;
	uint64 sum = 0;

	for ( size_t i = 0; i < n; ++i )
	{
		size_t idx = ( buf->head + buf->capacity - 1 - i ) % buf->capacity;
		sum += buf->samples[ idx ];
	}

	return sum;
}

status cbuf_m_peek_latest( const cbuf_metr * buf , uint64 * out_val )
{
	if ( !buf || !buf->samples || buf->filled == 0 || !out_val ) return errArg;
	size_t latest_idx = ( buf->head + buf->capacity - 1 ) % buf->capacity;
	*out_val = buf->samples[ latest_idx ];
	return errOK;
}

float cbuf_m_mean_last( const cbuf_metr * buf , size_t last_n )
{
	if ( !buf || !buf->samples || buf->filled == 0 ) return 0.0f;

	size_t n = ( last_n < buf->filled ) ? last_n : buf->filled;
	if ( n == 0 ) return 0.0f;

	uint64 sum = 0;

	for ( size_t i = 0; i < n; ++i )
	{
		size_t idx = ( buf->head + buf->capacity - 1 - i ) % buf->capacity;
		sum += buf->samples[ idx ];
	}

	return ( float )sum / ( float )n;
}

uint64 cbuf_m_sum_all( const cbuf_metr * buf )
{
	if ( !buf || !buf->samples || buf->filled == 0 ) return 0;

	uint64 sum = 0;
	for ( size_t i = 0; i < buf->filled; ++i )
	{
		size_t idx = ( buf->head + buf->capacity - buf->filled + i ) % buf->capacity;
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

int cbuf_m_regression_slope_all( const cbuf_metr * buf )
{
	if ( !buf || !buf->samples || buf->filled < 2 ) return 0;

	return regression_slope_int( buf->samples , buf->filled );
}
