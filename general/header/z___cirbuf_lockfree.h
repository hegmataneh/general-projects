#pragma once

#if defined Uses_cirbuf_lockfree || !defined __COMPILING

typedef struct cirbuf_lockfree
{
	LPSTR buf;         // raw memory for all rooms
	size_t room_size;     // size of each room
	size_t capacity;      // number of rooms
	size_t head;          // write index
	size_t tail;          // read index
	int full;             // overwrite flag

	l_pkg * lock_pkg; // if lock package available lock by caller method
} cbuf_lf;

// Initialize circular buffer
status cbuf_lf_init( cbuf_lf * cblf , size_t capacity , size_t room_size , l_pkg * plpkg /*=NULL*/ );

// Free buffer resources
void cbuf_lf_free( cbuf_lf * cblf );

// Push item into buffer (overwrites oldest if full)
void cbuf_lf_push( cbuf_lf * cblf , const void_p item , size_t data_len , int caller_thread_data /*=0 passed to lock*/ );

// Pop item from buffer (returns 0 if empty, 1 if success)
status cbuf_lf_pop( cbuf_lf * cblf , void_p item , size_t *data_len , int caller_thread_data /*=0 passed to lock*/ );

// Check if buffer is empty
int cbuf_lf_empty( cbuf_lf * cblf , int caller_thread_data /*=0 passed to lock*/ );

// Check if buffer is full
//int cbuf_lf_full( cbuf_lf * cblf );

// Get number of stored items
//size_t cbuf_lf_size( cbuf_lf * cblf );

#endif
