#pragma once

#if defined Uses_dict_s_o || !defined __COMPILING

// just keep temporarily an object

typedef struct entry_s_o
{
	LPSTR  key;
	void_p  value;
	struct entry_s_o * next;
} entry_s_o_t;

typedef struct dict_s_o
{
	entry_s_o_t ** buckets;
	size_t size;
	size_t key_count;  // number of keys stored
} dict_s_o_t;

// Create dictionary with a given number of buckets
status dict_s_o_init( dict_s_o_t * dd );

// Free dictionary and all memory
void dict_s_o_free( dict_s_o_t * d );

// Insert or update key/value
// returns 0 on success, -1 on failure
status dict_s_o_put( dict_s_o_t * d , LPCSTR key , void_p value );

// Get value for key (returns NULL if not found)
// Returned pointer is valid until another dict_put() overwrites that key
void_p dict_s_o_get( dict_s_o_t * d , LPCSTR key );

size_t dict_s_o_count( dict_s_o_t * d );

#endif

