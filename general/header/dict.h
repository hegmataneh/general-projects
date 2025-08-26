#pragma once

#if defined Uses_dict || !defined __COMPILING

typedef struct entry
{
	LPSTR  key;
	LPSTR  value;
	struct entry * next;
} entry_t;

typedef struct dict
{
	entry_t ** buckets;
	size_t size;
	size_t key_count;  // number of keys stored
} dict_t;

// Create dictionary with a given number of buckets
status dict_init( dict_t * dd );

// Free dictionary and all memory
void dict_free( dict_t * d );

// Insert or update key/value
// returns 0 on success, -1 on failure
status dict_put( dict_t * d , LPCSTR key , LPCSTR value );

// Get value for key (returns NULL if not found)
// Returned pointer is valid until another dict_put() overwrites that key
LPCSTR dict_get( dict_t * d , LPCSTR key );

size_t dict_count( dict_t * d );

// Return array of keys. Caller must free the array (but not the strings).
status dict_get_keys( dict_t * d , _OUT LPCSTR ** strs , _OUT int * count );

#endif

