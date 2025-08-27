#pragma once

#if defined Uses_dict_o || !defined __COMPILING

// just keep temporarily an object

typedef struct entry_o
{
	int  key;
	void_p  value;
	struct entry_o * next;
} entry_o_t;

typedef struct dict_o
{
	entry_o_t ** buckets;
	size_t size;
	size_t key_count;  // number of keys stored
} dict_o_t;

// Create dictionary with a given number of buckets
status dict_o_init( dict_o_t * dd );

// Free dictionary and all memory
void dict_o_free( dict_o_t * d );

// Insert or update key/value
// returns 0 on success, -1 on failure
status dict_o_put( dict_o_t * d , int key , void_p value );

// Get value for key (returns NULL if not found)
// Returned pointer is valid until another dict_put() overwrites that key
void_p dict_o_get( dict_o_t * d , int key );

size_t dict_o_count( dict_o_t * d );

#endif

