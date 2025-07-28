#if defined Uses_circbuf || !defined __COMPILING

// non thread safe

struct circbuf_t
{
	uint64_t * samples;   // per-slot counts
	size_t capacity;     // number of time slots
	size_t head;         // current position (circular index)
	size_t filled;       // number of valid samples stored
};

// Initialize the buffer with `capacity` samples (e.g., per second)
int circbuf_init( struct circbuf_t * buf , size_t capacity );

// Free the internal memory
void circbuf_free( struct circbuf_t * buf );

// Reset buffer to zero samples
void circbuf_reset( struct circbuf_t * buf );

// Advance one slot (like one second passed), optionally set count
void circbuf_advance( struct circbuf_t * buf , uint64_t count );

// Get last N samples (returns sum of last N if available)
uint64_t circbuf_sum_last( const struct circbuf_t * buf , size_t last_n );

// Peek most recent sample
int circbuf_peek_latest( const struct circbuf_t * buf , uint64_t * out_val );

// Compute mean of last N samples (returns float)
float circbuf_mean_last( const struct circbuf_t * buf , size_t last_n );

// Compute sum of all stored samples
uint64_t circbuf_sum_all( const struct circbuf_t * buf );

// Compute mean of all stored samples
float circbuf_mean_all( const struct circbuf_t * buf );

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

