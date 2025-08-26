#pragma once

#if defined Uses_TokenRing_p || !defined __COMPILING

typedef struct token_item_p
{
	void_p data;
	struct token_item_p * next;
} token_item_p_t;

typedef struct
{
	token_item_p_t * head;
	token_item_p_t * current;
	pthread_mutex_t lock;
} token_ring_p_t;

// Initialize a token ring
void token_ring_p_init( token_ring_p_t * ring );

// Add an item to the ring
status token_ring_p_add( token_ring_p_t * ring , void_p turn_key );

// Advance to the next item that has token and call its callback
void token_ring_p_next( token_ring_p_t * ring , void_p * turn );

// Free all items and destroy the ring
void token_ring_destroy( token_ring_p_t * ring );

#endif

