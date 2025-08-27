#define Uses_MEMSET_ZERO
#define Uses_pthread_mutex_init
#define Uses_dict_s_o
#include <general.dep>

status dict_s_o_init( dict_s_o_t * dd )
{
	MEMSET_ZERO_O( dd );
	return errOK;
}

// Expand bucket array dynamically if needed
status dict_s_o_expand( dict_s_o_t * d , size_t new_size )
{
	if ( new_size <= d->size ) return errOK;
	d->buckets = REALLOC_AR( d->buckets , new_size );
	if ( !d->buckets )
	{
		return errMemoryLow;
	}
	for ( size_t ibuck = d->size; ibuck < new_size; ibuck++ )
	{
		d->buckets[ ibuck ] = NULL;
	}
	d->size = new_size;
	return errOK;
}

status dict_s_o_put( dict_s_o_t * d , LPCSTR key , void_p value )
{
	ulong idx = hash8_fnv1a_avalanche( key );

	// Ensure buckets array is large enough
	if ( idx >= d->size )
	{
		if ( dict_s_o_expand( d , idx + 1 ) != errOK )
		{
			return errMemoryLow;
		}
	}

	entry_s_o_t * e = d->buckets[ idx ];
	while ( e )
	{
		if ( STR_SAME( e->key , key ) )
		{
			// Update existing
			e->value = value;
			return errOK;
		}
		e = e->next;
	}

	// Insert new
	entry_s_o_t * new_e = MALLOC_ONE( new_e );
	if ( !new_e )
	{
		return errMemoryLow;
	}
	new_e->key = strdup(key);
	new_e->value = value;
	new_e->next = d->buckets[ idx ];
	d->buckets[ idx ] = new_e;

	d->key_count++; // new key added

	return errOK;
}

void_p dict_s_o_get( dict_s_o_t * d , LPCSTR key )
{
	ulong idx = hash8_fnv1a_avalanche( key );

	if ( idx >= d->size ) return NULL;

	entry_s_o_t * e = d->buckets[ idx ];
	while ( e )
	{
		if ( STR_SAME( e->key , key ) )
			return e->value;
		e = e->next;
	}
	return NULL;
}

size_t dict_s_o_count( dict_s_o_t * d )
{
	return d->key_count;
}

void dict_s_o_free( dict_s_o_t * d )
{
	if ( !d ) return;

	for ( size_t ibuck = 0; ibuck < d->size; ibuck++ )
	{
		entry_s_o_t * e = d->buckets[ ibuck ];
		while ( e )
		{
			entry_s_o_t * tmp = e;
			e = e->next;
			DAC( tmp->key );
			DAC( tmp );
		}
	}

	DAC( d->buckets );
}
