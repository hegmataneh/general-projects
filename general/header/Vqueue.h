#pragma once

#if defined Uses_Vqueue || !defined __COMPILING

/*
virtual queue container
multi producer multi consumer
just onetime usage
*/

typedef struct
{
	buffer buf;			// base buffer
	size_t capacity;	// total buffer size
	size_t pos_reader , pos_writer;	// current stack pointer
	pthread_mutex_t lock; // mutex for MPMC safety
} Vqueue_t;

typedef struct
{
	void_p addr;
	size_t sz;
} vque_touched;

typedef vque_touched vqtchs[ 2 ];

// init pque with user buffer
void vqueue_init( HDLR Vqueue_t * pque , _INx void_p buffer , _INx size_t capacity , bool reset /*first time initialized or bind it to existing memory*/ );

// destroy pque (frees mutex)
void vqueue_destroy( HDLR Vqueue_t * pque );

// thread-safe push (returns 0 = ok, -1 = overflow)
status vqueue_enque( HDLR Vqueue_t * pque , const void_p data , size_t size , vqtchs * touches );

// peek (non-destructive)
status vqueue_peek( HDLR Vqueue_t * pque , OUTx void_p * item , OUTx size_t * size );

// pop (returns pointer and size)
status vqueue_deque( HDLR Vqueue_t * pque , OUTx void_p * item , OUTx size_t * size , bool * pemptied );

// clear pque
void vqueue_clear( HDLR Vqueue_t * pque , bool clear_var );

#endif
