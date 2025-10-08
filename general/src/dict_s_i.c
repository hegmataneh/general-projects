#define Uses_STRCMP
#define Uses_INIT_BREAKABLE_FXN
#define Uses_MEMSET_ZERO
#define Uses_pthread_mutex_init
#define Uses_dict_s_i
#include <general.dep>


status dict_s_i_init( dict_s_i_t * dd )
{
	MEMSET_ZERO_O( dd );
	return errOK;
}

// Expand bucket array dynamically if needed
status dict_s_i_expand( dict_s_i_t * d , size_t new_size )
{
	if ( new_size <= d->size ) return errOK;
	d->buckets = REALLOC_AR( d->buckets , new_size );
	if ( !d->buckets )
	{
		return errMemoryLow;
	}
	for ( size_t i = d->size; i < new_size; i++ )
	{
		d->buckets[ i ] = NULL;
	}
	d->size = new_size;
	return errOK;
}

status dict_s_i_put( dict_s_i_t * d , LPCSTR key , int value , int update_Value )
{
	ulong idx = ( ulong )hash8_fnv1a_avalanche( key );

	// Ensure buckets array is large enough
	if ( idx >= d->size )
	{
		if ( dict_s_i_expand( d , idx + 1 ) != errOK )
		{
			return errMemoryLow;
		}
	}

	entry_s_i_t * e = d->buckets[ idx ];
	while ( e )
	{
		if ( STRCMP( e->key , key ) == 0 )
		{
			// Update existing
			if ( update_Value )
			{
				e->value = value;
			}
			return errOK;
		}
		e = e->next;
	}

	// Insert new
	entry_s_i_t * new_e = MALLOC_ONE( new_e );
	if ( !new_e )
	{
		return errMemoryLow;
	}
	new_e->key = strdup( key );
	new_e->value = value;
	new_e->next = d->buckets[ idx ];
	d->buckets[ idx ] = new_e;

	d->key_count++; // new key added

	return errOK;
}

status dict_s_i_get( dict_s_i_t * d , LPCSTR key , int * value )
{
	ulong idx = ( ulong )hash8_fnv1a_avalanche( key );
	if ( idx >= d->size ) return errNotFound;

	entry_s_i_t * e = d->buckets[ idx ];
	while ( e )
	{
		if ( STRCMP( e->key , key ) == 0 )
		{
			*value = e->value;
			return errOK;
		}
		e = e->next;
	}
	return errNotFound;
}

size_t dict_s_i_count( dict_s_i_t * d )
{
	return d->key_count;
}

void dict_s_i_free( dict_s_i_t * d )
{
	if ( !d ) return;

	for ( size_t i = 0; i < d->size; i++ )
	{
		entry_s_i_t * e = d->buckets[ i ];
		while ( e )
		{
			entry_s_i_t * tmp = e;
			e = e->next;
			DAC( tmp->key );
			DAC( tmp );
		}
	}

	DAC( d->buckets );
}

status dict_s_i_try_put( dict_s_i_t * d , LPCSTR key , int value , int * exist )
{
	*exist = 1;
	int tmp;
	if ( dict_s_i_get( d , key , &tmp ) != errOK )
	{
		*exist = 0;
	}
	return dict_s_i_put( d , key , value , 1 );
}

status dict_s_i_get_keys( dict_s_i_t * d , _NEW_OUT_P LPCSTR ** strs , _RET_VAL_P int * count )
{
	INIT_BREAKABLE_FXN();
	*count = ( int )dict_s_i_count( d );
	N_MALLOC_AR( *strs , d->key_count , 0 );
	size_t pos = 0;

	for ( size_t i = 0; i < d->size; i++ )
	{
		entry_s_i_t * e = d->buckets[ i ];
		while ( e )
		{
			*( ( *strs ) + pos ) = ( LPCSTR )e->key;
			pos++;
			e = e->next;
		}
	}
	BEGIN_SMPL
	END_RET
}
