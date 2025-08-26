#pragma once

#if defined Uses_TokenRing_cl || !defined __COMPILING

typedef struct token_item_cl
{
	callback_t1 callback;
	void_p user_data;
	struct token_item_cl * next;
} token_item_cl_t;

typedef struct
{
	token_item_cl_t * head;
	token_item_cl_t * current;
	pthread_mutex_t lock;
} token_ring_cl_t;

// Initialize a token ring
void token_ring_cl_init( token_ring_cl_t * ring );

// Add an item to the ring
status token_ring_cl_add( token_ring_cl_t * ring , callback_t1 cb , void_p user_data );

// Advance to the next item that has token and call its callback
void token_ring_cl_next( token_ring_cl_t * ring );

#endif

