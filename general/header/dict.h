#pragma once

#if defined Uses_dict || !defined __COMPILING

typedef struct dict dict_t;

// Create dictionary with a given number of buckets
status dict_init( dict_t ** dd , size_t capacity );

// Free dictionary and all memory
void dict_free( dict_t * d );

// Insert or update key/value
// returns 0 on success, -1 on failure
status dict_put( dict_t * d , LPCSTR key , LPCSTR value );

// Get value for key (returns NULL if not found)
// Returned pointer is valid until another dict_put() overwrites that key
LPCSTR dict_get( dict_t * d , LPCSTR key );

#endif

