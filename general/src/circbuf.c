#define Uses_memset
#define Uses_calloc
#define Uses_circbuf
#include <general.dep>

int circbuf_init( struct circbuf_t * buf , size_t capacity )
{
	if ( !buf || capacity == 0 ) return -1;
	buf->samples = ( uint64_t * )calloc( capacity , sizeof( uint64_t ) );
	if ( !buf->samples ) return -1;
	buf->capacity = capacity;
	buf->head = 0;
	buf->filled = 0;
	return 0;
}

void circbuf_free( struct circbuf_t * buf )
{
	if ( !buf ) return;
	free( buf->samples );
	buf->samples = NULL;
	buf->capacity = 0;
	buf->head = 0;
	buf->filled = 0;
}

void circbuf_reset( struct circbuf_t * buf )
{
	if ( !buf || !buf->samples ) return;
	memset( buf->samples , 0 , buf->capacity * sizeof( uint64_t ) );
	buf->head = 0;
	buf->filled = 0;
}

void circbuf_advance( struct circbuf_t * buf , uint64_t count )
{
	if ( !buf || !buf->samples ) return;
	buf->samples[ buf->head ] = count;
	buf->head = ( buf->head + 1 ) % buf->capacity;
	if ( buf->filled < buf->capacity )
	{
		buf->filled++;
	}
}

uint64_t circbuf_sum_last( const struct circbuf_t * buf , size_t last_n )
{
	if ( !buf || !buf->samples || buf->filled == 0 ) return 0;

	size_t n = ( last_n < buf->filled ) ? last_n : buf->filled;
	uint64_t sum = 0;

	for ( size_t i = 0; i < n; ++i )
	{
		size_t idx = ( buf->head + buf->capacity - 1 - i ) % buf->capacity;
		sum += buf->samples[ idx ];
	}

	return sum;
}

int circbuf_peek_latest( const struct circbuf_t * buf , uint64_t * out_val )
{
	if ( !buf || !buf->samples || buf->filled == 0 || !out_val ) return -1;
	size_t latest_idx = ( buf->head + buf->capacity - 1 ) % buf->capacity;
	*out_val = buf->samples[ latest_idx ];
	return 0;
}

float circbuf_mean_last( const struct circbuf_t * buf , size_t last_n )
{
	if ( !buf || !buf->samples || buf->filled == 0 ) return 0.0f;

	size_t n = ( last_n < buf->filled ) ? last_n : buf->filled;
	if ( n == 0 ) return 0.0f;

	uint64_t sum = 0;

	for ( size_t i = 0; i < n; ++i )
	{
		size_t idx = ( buf->head + buf->capacity - 1 - i ) % buf->capacity;
		sum += buf->samples[ idx ];
	}

	return ( float )sum / ( float )n;
}

uint64_t circbuf_sum_all( const struct circbuf_t * buf )
{
	if ( !buf || !buf->samples || buf->filled == 0 ) return 0;

	uint64_t sum = 0;
	for ( size_t i = 0; i < buf->filled; ++i )
	{
		size_t idx = ( buf->head + buf->capacity - buf->filled + i ) % buf->capacity;
		sum += buf->samples[ idx ];
	}
	return sum;
}

float circbuf_mean_all( const struct circbuf_t * buf )
{
	if ( !buf || !buf->samples || buf->filled == 0 ) return 0.0f;

	uint64_t sum = circbuf_sum_all( buf );
	return ( float )sum / ( float )buf->filled;
}
