#pragma once

#if defined Uses_dict_s_i || !defined __COMPILING

// keep persistant key value like normal dict

typedef struct entry_s_i
{
	LPSTR  key;
	int  value;
	struct entry_s_i * next;
} entry_s_i_t;

typedef struct
{
	entry_s_i_t ** buckets;
	size_t size;
	size_t key_count;  // number of keys stored
} dict_s_i_t;

// Create dictionary with a given number of buckets
status dict_s_i_init( dict_s_i_t * dd );

// Free dictionary and all memory
void dict_s_i_free( dict_s_i_t * d );

// Insert or update key/value
// returns 0 on success, -1 on failure
status dict_s_i_put( dict_s_i_t * d , LPCSTR key , int value , int update_Value );

// Get value for key (returns NULL if not found)
// Returned pointer is valid until another dict_put() overwrites that key
status dict_s_i_get( dict_s_i_t * d , LPCSTR key , int * value );

status dict_s_i_try_put( dict_s_i_t * d , LPCSTR key , int value , int * exist );

size_t dict_s_i_count( dict_s_i_t * d );

// Return array of keys. Caller must free the array (but not the strings).
status dict_s_i_get_keys( dict_s_i_t * d , _OUT LPCSTR ** strs , _OUT int * count );

#endif

