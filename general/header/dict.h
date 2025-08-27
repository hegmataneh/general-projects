#pragma once

#if defined Uses_dict_s_s || !defined __COMPILING

// keep persistant key value like normal dict

typedef struct entry_s_s
{
	LPSTR  key;
	LPSTR  value;
	struct entry_s_s * next;
} entry_s_s_t;

typedef struct
{
	entry_s_s_t ** buckets;
	size_t size;
	size_t key_count;  // number of keys stored
} dict_s_s_t;

// Create dictionary with a given number of buckets
status dict_init( dict_s_s_t * dd );

// Free dictionary and all memory
void dict_free( dict_s_s_t * d );

// Insert or update key/value
// returns 0 on success, -1 on failure
status dict_put( dict_s_s_t * d , LPCSTR key , LPCSTR value );

// Get value for key (returns NULL if not found)
// Returned pointer is valid until another dict_put() overwrites that key
LPCSTR dict_get( dict_s_s_t * d , LPCSTR key );

size_t dict_count( dict_s_s_t * d );

// Return array of keys. Caller must free the array (but not the strings).
status dict_get_keys( dict_s_s_t * d , _OUT LPCSTR ** strs , _OUT int * count );

#endif

