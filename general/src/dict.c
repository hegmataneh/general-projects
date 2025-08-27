#define Uses_INIT_BREAKABLE_FXN
#define Uses_MEMSET_ZERO
#define Uses_pthread_mutex_init
#define Uses_dict_s_s
#include <general.dep>


status dict_init( dict_s_s_t * dd )
{
	MEMSET_ZERO_O( dd );
	return errOK;
}

// Expand bucket array dynamically if needed
status dict_expand( dict_s_s_t * d , size_t new_size )
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

status dict_put( dict_s_s_t * d , LPCSTR key , LPCSTR value )
{
	ulong idx = ( ulong )hash8_fnv1a_avalanche( key );

	// Ensure buckets array is large enough
	if ( idx >= d->size )
	{
		if ( dict_expand( d , idx + 1 ) != errOK )
		{
			return errMemoryLow;
		}
	}

	entry_s_s_t * e = d->buckets[ idx ];
	while ( e )
	{
		if ( strcmp( e->key , key ) == 0 )
		{
			// Update existing
			DAC( e->value );
			e->value = strdup( value );
			return errOK;
		}
		e = e->next;
	}

	// Insert new
	entry_s_s_t * new_e = MALLOC_ONE( new_e );
	if ( !new_e )
	{
		return errMemoryLow;
	}
	new_e->key = strdup( key );
	new_e->value = strdup( value );
	new_e->next = d->buckets[ idx ];
	d->buckets[ idx ] = new_e;

	d->key_count++; // new key added

	return errOK;
}

LPCSTR dict_get( dict_s_s_t * d , LPCSTR key )
{
	ulong idx = ( ulong )hash8_fnv1a_avalanche( key );
	if ( idx >= d->size ) return NULL;

	entry_s_s_t * e = d->buckets[ idx ];
	while ( e )
	{
		if ( strcmp( e->key , key ) == 0 )
			return e->value;
		e = e->next;
	}
	return NULL;
}

size_t dict_count( dict_s_s_t * d )
{
	return d->key_count;
}

void dict_free( dict_s_s_t * d )
{
	if ( !d ) return;

	for ( size_t i = 0; i < d->size; i++ )
	{
		entry_s_s_t * e = d->buckets[ i ];
		while ( e )
		{
			entry_s_s_t * tmp = e;
			e = e->next;
			DAC( tmp->key );
			DAC( tmp->value );
			DAC( tmp );
		}
	}

	DAC( d->buckets );
}

status dict_get_keys( dict_s_s_t * d , _OUT LPCSTR ** strs , _OUT int * count )
{
	INIT_BREAKABLE_FXN();
	*count = ( int )dict_count( d );
	M_MALLOC_AR( *strs , d->key_count , 0 );
	size_t pos = 0;

	for ( size_t i = 0; i < d->size; i++ )
	{
		entry_s_s_t * e = d->buckets[ i ];
		while ( e )
		{
			*( (*strs) + pos ) = ( LPCSTR )e->key;
			pos++;
			e = e->next;
		}
	}
	BEGIN_SMPL
	END_RET
}
