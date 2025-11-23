#if defined Uses_cbuf_metric || !defined __COMPILING

// non thread safe

typedef struct cirbuf_metric_t /*variable length snapshot window*/
{
	uint64 * samples;   // per-slot counts
	size_t capacity;     // number of time slots
	size_t head;         // current position (circular index)
	size_t filled;       // number of valid samples stored
} cbuf_metr;

// Initialize the buffer with `capacity` samples (e.g., per second)
status cbuf_m_init( cbuf_metr * buf , size_t capacity );

// Free the internal memory
void cbuf_m_free( cbuf_metr * buf );

// Reset buffer to zero samples
void cbuf_m_reset( cbuf_metr * buf );

// Advance one slot (like one second passed), optionally set count
void cbuf_m_advance( cbuf_metr * buf , uint64 count );

// Get last N samples (returns sum of last N if available)
uint64 cbuf_m_sum_last( const cbuf_metr * buf , size_t last_n );

// Peek most recent sample
status cbuf_m_peek_latest( const cbuf_metr * buf , uint64 * out_val );

// Compute mean of last N samples (returns float)
float cbuf_m_mean_last( const cbuf_metr * buf , size_t last_n );

// Compute sum of all stored samples
uint64 cbuf_m_sum_all( const cbuf_metr * buf );

// Compute mean of all stored samples
float cbuf_m_mean_all( const cbuf_metr * buf );

int cbuf_m_regression_slope_all( const cbuf_metr * buf );

#endif

/* example:
circbuf_t tracker10s , tracker40s;

circbuf_init( &tracker10s , 10 );  // Track 10 recent samples
circbuf_init( &tracker40s , 40 );  // Track 40 recent samples

// Simulate adding samples per second
for ( int i = 1; i <= 50; ++i )
{
	circbuf_advance( &tracker10s , i % 5 );
	circbuf_advance( &tracker40s , i % 3 );

	if ( i % 10 == 0 )
	{
		printf( "10s Sum: %u\n" , circbuf_sum_last( &tracker10s , 10 ) );
		printf( "40s Sum: %u\n" , circbuf_sum_last( &tracker40s , 40 ) );
	}
}

circbuf_free( &tracker10s );
circbuf_free( &tracker40s );
*/

