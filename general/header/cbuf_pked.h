#pragma once

#if defined Uses_cbuf_pked || !defined __COMPILING

// implement fast push with False sharing and alignment

// variable circular buffer not block on push and pop . but used as one to one
// packer and serialize data
// non blockable
// 2 byte blocksize max
_FALSE_SHARE_SAFE typedef struct packed_cbuf_NB // conveys synchronized one producer one consumer
{
	union
	{
		buffer buf;			// raw memory for all rooms
		char pad1[CACHE_LINE_SIZE];
	};
	union
	{
		size_t buf_sz;		// size of each room
		char pad2[ CACHE_LINE_SIZE ];
	};
	union
	{
		/*volatile */size_t head;		// write address
		char pad3[ CACHE_LINE_SIZE ];
	};
	union
	{
		/*volatile */size_t tail;		// read address
		char pad4[ CACHE_LINE_SIZE ];
	};
	STAT_FLD size_t err_full;
	union
	{
		sem_t gateway;
		char pad5[ CACHE_LINE_SIZE ];
	};
	
	volatile bool * pAppShutdown;
} cbuf_pked_t;

// Initialize the queue
status cbuf_pked_init( cbuf_pked_t * vc , size_t buf_sz , volatile  bool * app_closed_signal );

// Destroy the queue
void cbuf_pked_destroy( cbuf_pked_t * vc );

// overwrite oldest one if full . this fxn at producer side so must be so fast
status cbuf_pked_push( cbuf_pked_t * vc , const buffer buf , size_t buf_len , size_t alloc_len , _RET_VAL_P size_t * ring_addr , bool auto_opengate ); // you can allocate more that use want

// Blocking pop: waits if empty
status cbuf_pked_pop( cbuf_pked_t * vc , void * out_buf , size_t expectation_size /*zero to no exp*/ , size_t * out_len , long timeout_sec /*=-1*/ , bool auto_checkgate );

status cbuf_pked_blindcopy( cbuf_pked_t * vc , void * out_buf , size_t out_buf_sz , size_t block_sz_pos );

int cbuf_pked_unreliable_sem_count( cbuf_pked_t * vc ); // donot rely on this number

#endif
