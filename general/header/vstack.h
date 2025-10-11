#pragma once

#if defined Uses_VStack || !defined __COMPILING

/*
virtual stack container
multi producer multi consumer
*/

typedef struct
{
	buffer buf;     // base buffer
	size_t capacity;     // total buffer size
	size_t top;          // current stack pointer
	pthread_mutex_t lock; // mutex for MPMC safety
} VStack;

// init stack with user buffer
void vstack_init( HDLR VStack * stack , _INx void_p buffer , _INx size_t capacity , bool reset /*first time initialized or bind it to existing memory*/ );

// destroy stack (frees mutex)
void vstack_destroy( HDLR VStack * stack );

// thread-safe push (returns 0 = ok, -1 = overflow)
status vstack_push( HDLR VStack * stack , const void_p data , size_t size );

// peek (non-destructive)
status vstack_peek( HDLR VStack * stack , OUTx void_p * item , OUTx size_t * size );

// pop (returns pointer and size)
status vstack_pop( HDLR VStack * stack , OUTx void_p * item , OUTx size_t * size );

// clear stack
void vstack_clear( HDLR VStack * stack );

#endif
