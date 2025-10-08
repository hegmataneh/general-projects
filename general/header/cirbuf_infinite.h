#pragma once

 /*
 * seg_manager.c
 *
 * Circular, unrolled, doubly-linked segment manager for variable-length raw items.
 * - Each segment holds a contiguous data buffer and an offsets array for item lookup.
 * - Segments are linked in a circular doubly-linked ring.
 * - Active segment receives appends until full; then manager delegates to next available.
 * - Filled segments are pushed to a filled-queue for consumers to retrieve and flush to disk.
 * - Multi-consumer safe (pthread mutex + cond); single-manager for producers assumed.
 *
 * Author: (provided to user)
 * Date: 2025 09 14
 * 
 * and also make filled as stack so its like double linked list
 * totally this struct implement two double linked list inside each other
 * 
 * i assume each segment just keep complete item bytes. so items not fragment into some segment
 */

#if defined Uses_cirbuf_infinite || !defined __COMPILING

//#define _POSIX_C_SOURCE 200809L

/* ---------- Configuration defaults ---------- */
//#ifndef DEFAULT_SEG_CAP
//#define DEFAULT_SEG_CAP (64 * 1024)   /* bytes per segment data buffer default */
//#endif
 
#ifndef DEFAULT_OFFSETS_CAP
#define DEFAULT_OFFSETS_CAP 128       /* initial offsets array capacity per segment (number of items) */
#endif
 
#ifndef OFFSETS_GROW_FACTOR
#define OFFSETS_GROW_FACTOR 2
#endif

typedef struct cirbuf_inf_segment
{
	struct cirbuf_inf_segment * next;
	struct cirbuf_inf_segment * prev;
	struct cirbuf_inf_segment * queue_next;       /* filled queue link */
	struct cirbuf_inf_segment * queue_prev;       /* filled queue prev */

	/* segment is buffer and filled with variable length items*/
	buffer buf;				/* start of data buffer */
	size_t buf_capacity;	/* buf total capacity (bytes) */
	size_t buf_used;		/* bytes used */

	/* Offsets and sizes for items inside this segment */
	size_t * offsets;		/* offsets[i] is start offset of i-th item inside buf -> buf[ offsets[i] ] */
	size_t * sizes;			/* sizes[i] is size of i-th item */
	size_t offsets_capacity;/* capacity of offsets/sizes arrays (number of items) */
	size_t itm_count;       /* number of items currently stored */

	/* Flags / metadata */
	Boolean active_lock;	// just for debug
	//Boolean filled;         /* segment is filled and ready for consumers */
	Boolean in_filled_queue;/* whether pushed to filled queue */
} ci_sgm_t;

/* Manager struct */
typedef struct cirbuf_inf_sgmgr
{
	pthread_mutex_t lock;
	pthread_cond_t filled_cond;  /* signaled when filled queue non-empty */

	/* The circular ring of all segments (possibly empty) */
	ci_sgm_t * ring;      /* pointer to any segment in the ring (NULL if none) */
	ci_sgm_t * active;    /* pointer to active segment (may be NULL) */
	size_t segment_total; /* number of segments in ring */

	/* Queue of filled segments (simple singly-linked queue using segment->next as queue link) */
	ci_sgm_t * filled_head;
	ci_sgm_t * filled_tail; // sequentially link filled seg in queue
	size_t filled_count;

	/* Limits and policies */
	size_t default_seg_capacity;
	size_t default_offsets_capacity;
	Boolean allow_grow;      /* whether manager may allocate new segments */

	/* Optional stats */
	size_t total_items;
	size_t total_bytes;
} ci_sgmgr_t;

typedef status (*seg_item_cb)(buffer data, size_t len, pass_p ud);

typedef enum seg_trv
{
	seg_trv_FIFO ,
	seg_trv_LIFO
} seg_trv;

// default_seg_capacity = each sausage byte size , default_offsets_capacity = max item add of each sausage
status segmgr_init( ci_sgmgr_t * mgr , size_t default_seg_capacity , size_t default_offsets_capacity , Boolean allow_grow );
status segmgr_append( ci_sgmgr_t * mgr , const pass_p data , size_t len );
//status ci_sgm_get_item( ci_sgm_t * s , size_t idx , const void ** out_ptr , size_t * out_len );
void segmgr_destroy( ci_sgmgr_t * mgr );

ci_sgm_t * segmgr_pop_filled_segment( ci_sgmgr_t * mgr , Boolean block , seg_trv trv ); // pop filled segment
status ci_sgm_iter_items( ci_sgm_t * s , seg_item_cb cb , pass_p ud , bool try_all /*false -> until first erro , true->try them all*/ ); // iterate through items
status ci_sgm_mark_empty( ci_sgmgr_t * mgr , ci_sgm_t * s ); // finally back filled segment to available segment

// presume time exist in packet structure and just caller of this fxn know how to retrive it so instead of memcpy use fxn call to check active segment age. and if condition be ok filled sgm
void ci_sgm_peek_decide_active( ci_sgmgr_t * mgr , bool ( *callback )( const buffer buf , size_t sz ) );

#endif

/*
The implementation aims to satisfy :
Circular doubly - linked list of segments( each segment = unrolled node holding multiple variable - length items ).
Each segment stores raw items contiguously in a byte buffer plus an offsets array( so each item can be retrieved in O( 1 ) given its index inside the segment ).
An active segment is used for appends until full; manager finds an empty segment( priority: empty segments first; otherwise create new if allowed ).
Filled segments are pushed onto a thread - safe filled - queue so multiple consumers can pop filled segments and flush them to disk.After flush , consumer calls ci_sgm_mark_empty() to return the segment to the pool.
Append( sequential insert ) is amortized O( 1 ).Random access within a segment is O( 1 ).Segment deletion from the circular list is O( 1 ).
Thread - safety: manager operations that change structure use pthread_mutex_t and condition variables.The filled - queue uses the same mutex / cond for simplicity.You can replace with lock - free structures later if desired.
Memory management : segments can be dynamically added if allow_grow is true and system memory( malloc ) succeeds.Manager keeps counts and returns errors when memory insufficient.
Each stored item is appended as a raw byte blob with its size recorded so retrieval yields( void * , size_t ).
*/

#ifdef sample

static status print_item_cb( const void * data , size_t len , void * ud )
{
	( void )ud;
	printf( " item(len=%zu): " , len );
	fwrite( data , 1 , len , stdout );
	printf( "\n" );
	return errOK;
}

void * consumer_thread( void * arg )
{
	ci_sgmgr_t * mgr = ( ci_sgmgr_t * )arg;
	while ( 1 )
	{
		ci_sgm_t * s = segmgr_pop_filled_segment( mgr , true );
		if ( !s ) continue;
		printf( "[consumer] got filled segment with %zu items, %zu bytes\n" , s->count , s->buf_used );
		ci_sgm_iter_items( s , print_item_cb , NULL );
		/* simulate write to disk */
		usleep( 100000 );
		ci_sgm_mark_empty( mgr , s );
	}
	return NULL;
}

int main( void )
{
	ci_sgmgr_t mgr;
	segmgr_init( &mgr , 1024 , 8 , true );

	pthread_t tid;
	pthread_create( &tid , NULL , consumer_thread , &mgr );

	/* Append some items */
	for ( int i = 0; i < 100; ++i )
	{
		char buf[ 128 ];
		int n = snprintf( buf , sizeof( buf ) , "msg-%03d" , i );
		int rc = segmgr_append( &mgr , buf , ( size_t )n );
		if ( rc != SM_OK )
		{
			fprintf( stderr , "append rc=%d\n" , rc );
		}
		usleep( 20000 );
	}

	sleep( 2 );
	segmgr_destroy( &mgr );
	return 0;
}

#endif
