#pragma once

#if defined Uses_minHeap || !defined __COMPILING

typedef enum
{
	HEAP_MIN ,
	HEAP_MAX
} mh_HeapType;

typedef struct
{
	long key;    // priority
	void * data_key; // user data
} mh_HeapNode;

typedef struct
{
	dyn_mms_arr arr; // mh_HeapNode
	mh_HeapType type;
} mh_t;


status mh_create( mh_t * mh , size_t init_capacity/*allocated slots*/ , size_t growStep , mh_HeapType src_type );

status mh_insert( mh_t * h , long key , void * data_key , OUTcpy mh_HeapNode ** out_node /*NULL=no return*/ );

// Get min (peek)
status mh_min( mh_t * h , OUTcpy mh_HeapNode ** out_p );

// Extract min
status mh_extract_min( mh_t * h , OUTcpy mh_HeapNode ** out_p /*caller frees it*/ );

// Remove arbitrary node (O(n) to find, O(log n) to fix)
status mh_remove( mh_t * h , void * data_key /*caller data addr*/ );

void mh_heap_refresh( mh_t * h , void * data_key );

// Free the heap
void mh_destroy( mh_t * h );

#endif
