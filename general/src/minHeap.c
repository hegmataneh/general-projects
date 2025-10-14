#define Uses_MEMSET_ZERO_O
#define Uses_minHeap
#include <general.dep>

// Create a new heap
status mh_create( mh_t * mh , size_t init_capacity/*init sz*/ , size_t growStep , mh_HeapType src_type )
{
	MEMSET_ZERO_O( mh );
	status d_error = errOK;
	if ( ( d_error = mms_array_init( &mh->arr , sizeof( mh_HeapNode ) , init_capacity , growStep , 0 ) ) )
	{
		return d_error;
	}
	mh->type = src_type;
	return d_error;
}

// Swap helper
_PRIVATE_FXN inline void mh_swap( mh_HeapNode ** a , mh_HeapNode ** b )
{
	mh_HeapNode * tmp = *a;
	*a = *b;
	*b = tmp;
}

// Comparison depending on heap type
_PRIVATE_FXN inline int heap_cmp( mh_t * mh , long key1 , long key2 )
{
	if ( mh->type == HEAP_MIN )
		return key1 < key2;   // true if a has higher priority (smaller)
	else
		return key1 > key2;   // true if a has higher priority (larger)
}

// Bubble up
_PRIVATE_FXN status mh_heapify_up( mh_t * mh , size_t idx )
{
	while ( idx > 0 )
	{
		size_t parent = ( idx - 1 ) / 2;
		
		mh_HeapNode ** pparent = NULL;
		status err1 = mms_array_get_us( &mh->arr , parent , ( void *** )&pparent );

		mh_HeapNode ** pchild = NULL;
		status err2 = mms_array_get_us( &mh->arr , idx , ( void *** )&pchild );
		
		if ( err1 || err2 ) return ISNULL( err1 , err2 );
		
		if ( !heap_cmp( mh , (*pchild)->key , (*pparent)->key ) )
			break;

		mh_swap( pparent , pchild );
		idx = parent;
	}
	return errOK;
}

// Bubble down
_PRIVATE_FXN status mh_heapify_down( mh_t * mh , size_t idx )
{
	while ( 1 )
	{
		size_t left = 2 * idx + 1;
		size_t right = 2 * idx + 2;
		size_t best = idx;

		mh_HeapNode ** pleft = NULL;
		status err1 = mms_array_get_us( &mh->arr , left , ( void *** )&pleft );

		mh_HeapNode ** pright = NULL;
		status err2 = mms_array_get_us( &mh->arr , right , ( void *** )&pright );

		mh_HeapNode ** pbest = NULL;
		status err3 = mms_array_get_us( &mh->arr , best , ( void *** )&pbest );

		mh_HeapNode ** pindex = NULL;
		status err4 = mms_array_get_us( &mh->arr , idx , ( void *** )&pindex );

		if ( err1 || err2 || err3 || err4 ) return COALESCE4( err1 , err2 , err3 , err4 );

		if ( left < mh->arr.count && heap_cmp( mh , (*pleft)->key , (*pbest)->key ) )
			best = left;

		if ( right < mh->arr.count && heap_cmp( mh , (*pright)->key , (*pbest)->key ) )
			best = right;

		if ( best == idx ) break;
		mms_array_get_us( &mh->arr , best , ( void *** )&pbest );

		mh_swap( pindex , pbest );
		idx = best;
	}
	return errOK;
}

status mh_insert( mh_t * mh , long key , void * data_key , OUTcpy mh_HeapNode ** out_node )
{
	mh_HeapNode * pnode = NULL;
	status err = mms_array_get_one_available_unoccopied_item( &mh->arr , ( void ** )&pnode );
	if ( err ) return err;
	pnode->key = key;
	pnode->data_key = data_key;
	if ( out_node ) *out_node = pnode;
	return mh_heapify_up( mh , mh->arr.count - 1 );
}

// Get min (peek)
status mh_min( mh_t * mh , OUTcpy mh_HeapNode ** out_p )
{
	status d_error = mms_array_get_s( &mh->arr , 0 , ( void ** )out_p );
	return d_error == errNotFound ? errEmpty : d_error;
}

// Extract min
status mh_extract_min( mh_t * mh , OUTcpy mh_HeapNode ** out_p /*caller frees it*/ , size_t min_remain )
{
	status d_error = errOK;
	mh_HeapNode ** pmin = NULL;
	if ( ( d_error = mms_array_get_us( &mh->arr , 0 , ( void *** )&pmin ) ) ) return d_error;

	if ( out_p ) *out_p = *pmin;

	mh_HeapNode ** ptail = NULL;
	if ( ( d_error = mms_array_get_us( &mh->arr , mh->arr.count - 1 , ( void *** )&ptail ) ) ) return d_error;
	
	*pmin = *ptail;
	if ( mh->arr.count > min_remain )
	{
		mh->arr.count--;
		return mh_heapify_down( mh , 0 );
	}
	return errEmpty;
}

// Remove arbitrary node (O(n) to find, O(log n) to fix)
status mh_remove( mh_t * mh , void * data_key /*caller data addr*/)
{
	status d_error = errOK;
	size_t idx = ( size_t )-1;
	for ( size_t i = 0; i < mh->arr.count; i++ )
	{
		mh_HeapNode * pitem = NULL;
		if ( ( d_error = mms_array_get_s( &mh->arr , i , ( void ** )&pitem ) ) ) return d_error;

		if ( pitem->data_key == data_key )
		{
			idx = i;
			break;
		}
	}
	if ( idx == ( size_t )-1 ) return errNotFound; // not found

	mh_HeapNode ** ppitem = NULL;
	if ( ( d_error = mms_array_get_us( &mh->arr , idx , ( void *** )&ppitem ) ) ) return d_error;

	mh_HeapNode ** pptail = NULL;
	if ( ( d_error = mms_array_get_us( &mh->arr , mh->arr.count - 1 , ( void *** )&pptail ) ) ) return d_error;

	// Move last element into this spot
	*ppitem = *pptail; // TOCHECK
	mh->arr.count--;

	// Fix heap property
	if ( idx < mh->arr.count )
	{
		if ( ( d_error = mh_heapify_up( mh , idx ) ) ) return d_error;
		if ( ( d_error = mh_heapify_down( mh , idx ) ) ) return d_error;
	}
	return d_error;
}

// Refresh a node after key change
void mh_heap_refresh( mh_t * mh , void * data_key )
{
	status d_error = errOK;
	size_t idx = ( size_t )-1;
	for ( size_t i = 0; i < mh->arr.count; i++ )
	{
		mh_HeapNode * pitem = NULL;
		if ( ( d_error = mms_array_get_s( &mh->arr , i , ( void ** )&pitem ) ) ) return;

		if ( pitem->data_key == data_key )
		{
			idx = i;
			break;
		}
	}
	if ( idx == ( size_t )-1 ) return; // not found

	mh_heapify_up( mh , idx );
	mh_heapify_down( mh , idx );
}

// Free the heap
void mh_destroy( mh_t * mh )
{
	mms_array_free( &mh->arr );
}
