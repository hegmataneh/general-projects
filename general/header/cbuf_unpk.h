#pragma once

#if defined Uses_cbuf_unpk || !defined __COMPILING

// TODO . implement fast push with False sharing and lignment

// variable circular buffer not block on push and pop . but used as one to one
// unpacked with fixed room
// non blockable
//_FALSE_SHARE_SAFE
typedef struct unpack_cbuf_NB // conveys synchronized one producer one consumer
{
	buffer buf;			// raw memory for all rooms
	size_t room_size;	// size of each room
	size_t capacity;	// number of rooms
	size_t head;		// write index
	size_t tail;		// read index
	size_t err_full;

	sem_t gateway;
} cbuf_unpk;

// Initialize the queue
status cbuf_unpk_init( cbuf_unpk * vc , size_t capacity , size_t room_size );

// Destroy the queue
void cbuf_unpk_destroy( cbuf_unpk * vc );

// overwrite oldest one if full
status cbuf_unpk_push( cbuf_unpk * vc , const buffer buf , size_t len );

// Blocking pop: waits if empty
status cbuf_unpk_pop( cbuf_unpk * vc , buffer out_buf , size_t * out_len , long timeout_sec /*=-1*/ );

#endif
