#define Uses_WARNING
#define Uses_MEMSET_ZERO_O
#define Uses_INIT_BREAKABLE_FXN
#define Uses_dyn_mms_arr
#include <general.dep>

#ifdef BLOCK_SIZE
#undef BLOCK_SIZE
#endif
#define BLOCK_SIZE ( sizeof( void * ) )

// Initialize memmove safe dynamic array with given item_size
status mms_array_init( dyn_mms_arr * arr , size_t item_size , size_t init_capacity , size_t growStep , size_t init_occopied_count )
{
	INIT_BREAKABLE_FXN();
	if ( !arr || item_size == 0 || growStep == 0 || init_capacity < 1 || init_occopied_count > init_capacity || growStep < 1 ) return errArg;

	MEMSET_ZERO_O( arr );

	N_BREAK_IF( !( arr->data = CALLOC( init_capacity , BLOCK_SIZE ) ) , errMemoryLow , 0 );
	//MEMSET( arr->data , 0 , growStep * item_size ); // CALLOC do that

	arr->item_size = item_size;
	arr->capacity = init_capacity;
	arr->count = init_occopied_count;
	arr->growStep = growStep;

	BEGIN_SMPL
	N_END_RET
}

void mms_array_free( dyn_mms_arr * arr )
{
	if ( !arr ) return;
	for ( int iitm = 0 ; iitm < arr->count ; iitm++ )
	{
		DAC( arr->data[ iitm ] );
	}
	FREE( arr->data );
	arr->data = NULL;
	arr->capacity = 0;
	arr->count = 0;
	arr->item_size = 0;
}

status mms_array_resize( dyn_mms_arr * arr , size_t new_capacity , size_t new_used_count /*=0 to just reserve expand reserve capacity*/ )
{
	if ( new_used_count == 0 ) new_used_count = arr->count;
	if ( !arr || new_capacity == 0 ) return errArg;
	if ( new_capacity <= arr->count ) return errArg; // can't downsized
	if ( new_used_count > new_capacity ) return errArg; // just allow use maximum to new items
	if ( new_used_count < arr->count )
	{
		return errArg;
	}
	char * new_data = REALLOC( arr->data , BLOCK_SIZE * new_capacity );
	if ( !new_data ) return errMemoryLow;
	MEMSET( new_data + arr->count * BLOCK_SIZE , 0 , ( new_capacity - arr->count ) * BLOCK_SIZE ); // zero expanded slot
	arr->data = ( void ** )new_data;
	arr->capacity = new_capacity;
	arr->count = new_used_count;
	return errOK;
}

status mms_array_add( dyn_mms_arr * arr , void * item /*one item*/ )
{
	status d_error = errOK;
	void * pout = NULL;
	if ( ( d_error = mms_array_get_one_available_unoccopied_item( arr , &pout ) ) != errOK ) return d_error;
	MEMCPY_OR( pout , item , arr->item_size );
	return errOK;
}

status mms_array_get_one_available_unoccopied_item( dyn_mms_arr * arr , _NEW_OUT_P void ** item /*add you struct pointer*/ )
{
	status d_error = errOK;
	if ( !arr || !item ) return errArg;
	if ( arr->count >= arr->capacity )
	{
		if ( ( d_error = mms_array_resize( arr , arr->capacity + arr->growStep , 0 ) ) != errOK )
			return d_error;
	}
	if ( !( arr->data[ arr->count ] = CALLOC_AR( ( ( char * )arr->data[ arr->count ] ) , arr->item_size ) ) )
		return errMemoryLow;
	*item = arr->data[ arr->count ];
	arr->count++;
	return errOK;
}

// pointer to container keep data. it used to assign manual allocated unit to keep in arr
// so dangerius fxn
status mms_array_get_one_available_unoccopied_item_holder( dyn_mms_arr * arr , _NEW_OUT_P void *** item /*add you struct pointer*/ )
{
	status d_error = errOK;
	if ( !arr || !item ) return errArg;
	if ( arr->count >= arr->capacity )
	{
		if ( ( d_error = mms_array_resize( arr , arr->capacity + arr->growStep , 0 ) ) != errOK )
			return d_error;
	}
	//if ( !( arr->data[ arr->count ] = CALLOC_AR( ( ( char * )arr->data[ arr->count ] ) , arr->item_size ) ) )
	//	return errMemoryLow;
	*item = &arr->data[ arr->count ];
	arr->count++;
	return errOK;
}

status mms_array_delete( dyn_mms_arr * arr , size_t index )
{
	status d_error = errOK;
	if ( ( d_error = mms_array_idx_exist_s( arr , index ) ) ) return d_error;
	//char * base = ( char * )arr->data;
	void * target = arr->data + index;
	void * next = arr->data + index + 1;
	size_t move_size = ( arr->count - index - 1 ) * BLOCK_SIZE;
	DAC( arr->data[ index ] );
	if ( move_size )
	{
		MEMMOVE( target , next , move_size );
	}
	arr->count--;
	return errOK;
}

size_t mms_array_get_count( dyn_mms_arr * arr )
{
	if ( !arr ) return 0;
	return arr->count;
}

/// <summary>
/// if array is allocated pointer kkeper the value of item should not be freed
/// </summary>
_MEMMOVE_UNSAFE_FXN void * mms_array_get( dyn_mms_arr * arr , size_t index )
{
	if ( !arr || index >= arr->count ) return NULL;
	return arr->data[ index ];
}

_MEMMOVE_UNSAFE_FXN status mms_array_get_s( dyn_mms_arr * arr , size_t index , void ** pitem )
{
	status d_error = errOK;
	if ( ( d_error = mms_array_idx_exist_s( arr , index ) ) ) return d_error;
	return ( *pitem = mms_array_get( arr , index ) ) ? errOK : errNotFound;
}

// just could use temporarily . get unsafe pointer so be careful
_MEMMOVE_UNSAFE_FXN status mms_array_get_us( dyn_mms_arr * arr , size_t index , void *** ppitem )
{
	status d_error = errOK;
	if ( ( d_error = mms_array_idx_exist_s( arr , index ) ) ) return d_error;
	*ppitem = &arr->data[ index ];
	return errOK;
}

status mms_array_set( dyn_mms_arr * arr , size_t index , void * item )
{
	if ( !arr || index >= arr->count || !item ) return errArg;
	WARNING( index < arr->count );
	DAC( arr->data[ index ] );
	if ( !( arr->data[ index ] = CALLOC_AR( ( ( char * )arr->data[ arr->count ] ) , arr->item_size ) ) )
		return errMemoryLow;
	MEMCPY_OR( arr->data[ arr->count ] , item , arr->item_size );
	return errOK;
}

status mms_array_idx_exist_s( dyn_mms_arr * arr , size_t idx )
{
	if ( !arr ) return errArg;
	if ( !arr->count ) return errEmpty;
	if ( idx >= arr->count ) return errOverflow;
	return errOK;
}

Boolean mms_array_idx_exist_b( dyn_mms_arr * arr , size_t idx )
{
	return ToBoolean( mms_array_idx_exist_s( arr , idx ) == errOK );
}
