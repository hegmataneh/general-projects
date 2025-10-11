#pragma once

#if defined Uses_id_dict || !defined __COMPILING



/*
 * Thread-safe dictionary mapping string keys to unique incremental IDs.
 *
 * Usage:
 *   id_dict_t* d = dict_init(128);          // create dictionary with 128 buckets
 *   long id1 = dict_put_or_get(d, "a");  // assigns ID 0
 *   long id2 = dict_put_or_get(d, "b");  // assigns ID 1
 *   long id3 = dict_put_or_get(d, "a");  // returns existing ID 0
 *   dict_free(d);                        // free resources
 */

 /* Linked list entry inside a hash bucket */
typedef struct entry
{
	LPSTR key;             // dynamically allocated copy of the key
	long value;            // unique incremental ID
	struct entry * next;
} id_dict_entry_t;

/* Dictionary structure */
typedef struct id_dict
{
	size_t capacity;			// number of buckets
	id_dict_entry_t ** buckets;			// array of bucket heads
	pthread_mutex_t * locks;	// per-bucket mutexes
	long counter;				// global ID counter
	pthread_mutex_t counter_lock;
} id_dict_t;

 /*
 * dict_init
 * ----------
 * Create a new dictionary.
 *
 * Parameters:
 *   capacity - number of hash buckets (use a prime or power of two).
 *
 * Returns:
 *   Pointer to a dictionary, or NULL on allocation failure.
 */
 status id_dict_init( id_dict_t ** pdict , size_t capacity );

/*
 * dict_free
 * ----------
 * Free the dictionary and all memory inside.
 *
 * Parameters:
 *   d - pointer to dictionary (NULL is safe).
 */
void id_dict_free( id_dict_t * d );

/*
 * dict_put_or_get
 * ----------------
 * Insert a key if it does not exist, or return its existing ID.
 *
 * Thread-safety:
 *   Safe to call from multiple threads simultaneously.
 *
 * Parameters:
 *   d   - dictionary
 *   key - null-terminated string key
 *
 * Returns:
 *   Non-negative ID (starting at 0) associated with the key.
 *   -1 on allocation failure.
 */
long id_dict_put_or_get( id_dict_t * d , LPCSTR key );

#endif

