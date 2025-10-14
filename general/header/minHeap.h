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


// create min/max heap
status mh_create( mh_t * mh , size_t init_capacity/*init sz*/ , size_t growStep , mh_HeapType src_type );

status mh_insert( mh_t * mh , long key , void * data_key , OUTcpy mh_HeapNode ** out_node /*NULL=no return*/ );

// Get min (peek)
status mh_min( mh_t * mh , OUTcpy mh_HeapNode ** out_p );

// Extract min
status mh_extract_min( mh_t * mh , OUTcpy mh_HeapNode ** out_p /*caller frees it*/ , size_t min_remain /*no pop every item*/ );

// Remove arbitrary node (O(n) to find, O(log n) to fix)
status mh_remove( mh_t * mh , void * data_key /*caller data addr*/ );

void mh_heap_refresh( mh_t * mh , void * data_key );

// Free the heap
void mh_destroy( mh_t * mh );

#endif
