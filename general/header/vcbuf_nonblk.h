#pragma once

#if defined Uses_vcbuf_nonblk || !defined __COMPILING

// variable circular buffer not block on push and pop . but used as one to one
typedef struct vcbuf_nonblk // conveys synchronized one producer one consumer
{
	buffer buf;         // raw memory for all rooms
	size_t room_size;     // size of each room
	size_t capacity;      // number of rooms
	size_t head;          // write index
	size_t tail;          // read index
	size_t err_full;

	sem_t gateway;
} vcbuf_nb;

// Initialize the queue
status vcbuf_nb_init( vcbuf_nb * vc , size_t capacity , size_t room_size );

// Destroy the queue
void vcbuf_nb_destroy( vcbuf_nb * vc );

// overwrite oldest one if full
status vcbuf_nb_push( vcbuf_nb * vc , const buffer buf , size_t len );

// Blocking pop: waits if empty
status vcbuf_nb_pop( vcbuf_nb * vc , buffer out_buf , size_t * out_len , long timeout_sec /*=-1*/ );

#endif
