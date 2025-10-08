#define Uses_WARNING
#define Uses_MEMSET_ZERO_O
#define Uses_INIT_BREAKABLE_FXN
#define Uses_dyn_arr
#include <general.dep>

#ifdef DATA_AR
#undef DATA_AR
#endif
#define DATA_AR ( ( char * )arr->data )

#ifdef BLOCK_INDEX_ADD
#undef BLOCK_INDEX_ADD
#endif
#define BLOCK_INDEX_ADD( idx ) ( DATA_AR + ( size_t )idx * arr->item_size )

#define ALC_PTR_BLOCK( idx ) ( *( void ** )BLOCK_INDEX_ADD( idx ) )

status array_init( dyn_arr * arr , size_t item_size , size_t init_capacity , size_t growStep , size_t init_occopied_count )
{
	INIT_BREAKABLE_FXN();
	if ( !arr || item_size == 0 || growStep == 0 || init_capacity < 1 || init_occopied_count > init_capacity || growStep < 1 ) return errArg;
	
	MEMSET_ZERO_O( arr );

	N_BREAK_IF( !( arr->data = CALLOC( init_capacity , item_size ) ) , errMemoryLow , 0 );
	//MEMSET( arr->data , 0 , growStep * item_size ); // CALLOC do that
	
	arr->item_size = item_size;
	arr->capacity = init_capacity;
	arr->count = init_occopied_count;
	arr->growStep = growStep;

	BEGIN_SMPL
	N_END_RET
}

//status alc_ptr_array_init( dyn_arr * arr , size_t growStep )
//{
//	status ret = array_init( arr , sizeof( void * ) , 1 , growStep ); // based on the theory that every pointer has same size
//	arr->allocated_pointer_keeper = 1;
//	return ret;
//}

void array_free( dyn_arr * arr )
{
	if ( !arr ) return;
	if ( arr->allocated_pointer_keeper )
	{
		for ( int iitm = 0 ; iitm < arr->count ; iitm++ )
		{
			FREE( ALC_PTR_BLOCK( iitm ) );
		}
	}
	FREE( arr->data );
	arr->data = NULL;
	arr->capacity = 0;
	arr->count = 0;
	arr->item_size = 0;
}

status array_resize( dyn_arr * arr , size_t new_capacity , size_t new_used_count /*=0 to just reserve expand reserve capacity*/ )
{
	if ( new_used_count == 0 ) new_used_count = arr->count;
	if ( !arr || new_capacity == 0 ) return errArg;
	if ( new_capacity <= arr->count ) return errArg; // can't downsized
	if ( new_capacity <= arr->count ) return errArg; // can't downsized
	if ( new_used_count > new_capacity ) return errArg; // just allow use maximum to new items
	if ( new_used_count < arr->count )
	{
		return errArg;
	}
	void * new_data = REALLOC( arr->data , arr->item_size * new_capacity );
	if ( !new_data ) return errMemoryLow;
	MEMSET( new_data + arr->count * arr->item_size , 0 , ( new_capacity - arr->count ) * arr->item_size ); // zero expanded slot
	arr->data = new_data;
	arr->capacity = new_capacity;
	arr->count = new_used_count;
	return errOK;
}

status array_add( dyn_arr * arr , void * item )
{
	status d_error = errOK;
	if ( !arr || !item ) return errArg;
	if ( arr->count >= arr->capacity )
	{
		if ( ( d_error = array_resize( arr , arr->capacity + arr->growStep , 0 ) ) != errOK )
			return d_error;
	}
	MEMCPY_OR( BLOCK_INDEX_ADD( arr->count ) , item , arr->item_size );
	arr->count++;
	return errOK;
}

_MEMMOVE_UNSAFE_FXN status array_get_one_available_unoccopied_item( dyn_arr * arr , void ** item /*add you struct pointer*/ )
{
	status d_error = errOK;
	if ( !arr || !item ) return errArg;
	if ( arr->count >= arr->capacity )
	{
		if ( ( d_error = array_resize( arr , arr->capacity + arr->growStep , 0 ) ) != errOK )
			return d_error;
	}
	*item = BLOCK_INDEX_ADD( arr->count );
	arr->count++;
	return errOK;
}

status array_delete( dyn_arr * arr , size_t index )
{
	if ( !arr || index >= arr->count ) return errArg;
	char * base = ( char * )arr->data;
	char * target = base + index * arr->item_size;
	char * next = target + arr->item_size;
	size_t move_size = ( arr->count - index - 1 ) * arr->item_size;
	if ( arr->allocated_pointer_keeper )
	{
		DAC( ALC_PTR_BLOCK( index ) );
	}
	MEMMOVE( target , next , move_size );
	arr->count--;
	return errOK;
}

size_t array_get_count( dyn_arr * arr )
{
	if ( !arr ) return 0;
	return arr->count;
}

/// <summary>
/// if array is allocated pointer kkeper the value of item should not be freed
/// </summary>
_MEMMOVE_UNSAFE_FXN void * array_get( dyn_arr * arr , size_t index )
{
	if ( !arr || index >= arr->count ) return NULL;
	return ( void * )BLOCK_INDEX_ADD( index );
}

_MEMMOVE_UNSAFE_FXN status array_get_s( dyn_arr * arr , size_t index , void ** pitem )
{
	if ( !array_idx_exist( arr , index ) ) return errNotFound;
	return ( *pitem = array_get( arr , index ) ) ? errOK : errNotFound;
}

status array_set( dyn_arr * arr , size_t index , void * item )
{
	if ( !arr || index >= arr->count || !item ) return errArg;
	WARNING( index < arr->count );
	if ( arr->allocated_pointer_keeper )
	{
		if ( index < arr->count )
		{
			DAC( ALC_PTR_BLOCK( index ) );
		}
	}
	MEMCPY_OR( BLOCK_INDEX_ADD( index ) , item , arr->item_size );
	return errOK;
}

Boolean array_idx_exist( dyn_arr * arr , size_t idx )
{
	if ( !arr ) return False;
	if ( idx < 0 ) return False;
	return ToBoolean( idx < arr->count );
}
