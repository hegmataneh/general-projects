#pragma once

#if defined Uses_vcbuf_popblk || !defined __COMPILING

typedef struct vcbuf_popblk // conveys synchronized producer/consumer
{
	buffer buf;         // raw memory for all rooms
	size_t room_size;     // size of each room
	size_t capacity;      // number of rooms
	size_t head;          // write index
	size_t tail;          // read index
	int count;

	pthread_mutex_t lock;
	pthread_cond_t not_empty;

} vcbuf;

// Initialize the queue
status vcbuf_init( vcbuf * vc , size_t capacity , size_t room_size );

// Destroy the queue
void vcbuf_destroy( vcbuf * vc );

// overwrite oldest one if full
void vcbuf_push( vcbuf * vc , const buffer buf , size_t len );

// Blocking pop: waits if empty
status vcbuf_pop( vcbuf * vc , buffer out_buf , size_t * out_len , long timeout_sec /*=-1*/);

#endif
