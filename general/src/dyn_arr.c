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

status array_init( dyn_arr * arr , size_t item_size , size_t growStep )
{
	INIT_BREAKABLE_FXN();
	if ( !arr || item_size == 0 || growStep == 0 ) return errArg;
	
	MEMSET_ZERO_O( arr );

	M_BREAK_IF( !( arr->data = MALLOC_AR( arr->data , growStep ) ) , errMemoryLow , 0 );
	memset( arr->data , 0 , growStep * item_size );
	
	arr->item_size = item_size;
	arr->capacity = growStep;
	arr->count = 0;
	arr->growStep = growStep;

	BEGIN_SMPL
	M_END_RET
}

status alc_ptr_array_init( dyn_arr * arr , size_t growStep )
{
	status ret = array_init( arr , sizeof( void * ) , growStep ); // based on the theory that every pointer has same size
	arr->allocated_pointer_keeper = 1;
	return ret;
}

void array_free( dyn_arr * arr )
{
	if ( !arr ) return;
	if ( arr->allocated_pointer_keeper )
	{
		for ( int i = 0 ; i < arr->count ; i++ )
		{
			FREE( ALC_PTR_BLOCK( i ) );
		}
	}
	free( arr->data );
	arr->data = NULL;
	arr->capacity = 0;
	arr->count = 0;
	arr->item_size = 0;
}

status array_resize( dyn_arr * arr , size_t new_capacity )
{
	if ( !arr || new_capacity == 0 ) return errArg;
	if ( new_capacity < arr->count ) return errArg; // can't shrink below used
	void * new_data = realloc( arr->data , arr->item_size * new_capacity );
	if ( !new_data ) return errMemoryLow;
	memset( new_data + arr->count * arr->item_size , 0 , ( new_capacity - arr->count ) * arr->item_size ); // zero expanded slot
	arr->data = new_data;
	arr->capacity = new_capacity;
	return errOK;
}

status array_add( dyn_arr * arr , void * item )
{
	status d_error = errOK;
	if ( !arr || !item ) return errArg;
	if ( arr->count >= arr->capacity )
	{
		if ( ( d_error = array_resize( arr , arr->capacity + arr->growStep ) ) != errOK )
			return d_error;
	}
	memcpy( BLOCK_INDEX_ADD( arr->count ) , item , arr->item_size );
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
	memmove( target , next , move_size );
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
void * array_get( dyn_arr * arr , size_t index )
{
	if ( !arr || index >= arr->count ) return NULL;
	return ( void * )BLOCK_INDEX_ADD( index );
}

status array_set( dyn_arr * arr , size_t index , void * item )
{
	if ( !arr || index >= arr->count || !item ) return errArg;
	ASSERT( index < arr->count );
	if ( arr->allocated_pointer_keeper )
	{
		if ( index < arr->count )
		{
			DAC( ALC_PTR_BLOCK( index ) );
		}
	}
	memcpy( BLOCK_INDEX_ADD( index ) , item , arr->item_size );
	return errOK;
}
