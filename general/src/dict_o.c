#define Uses__i2s
#define Uses_MEMSET_ZERO
#define Uses_pthread_mutex_init
#define Uses_dict_o
#include <general.dep>

status dict_o_init( dict_o_t * dd , bool bfree_values_addrs )
{
	MEMSET_ZERO_O( dd );
	dd->bfree_values_addrs = bfree_values_addrs;
	return errOK;
}

// Expand bucket array dynamically if needed
status dict_o_expand( dict_o_t * d , size_t new_size )
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

status dict_o_put( dict_o_t * d , int key , void_p value )
{
	_i2s i2s = { 0 };
	i2s.i = key;
	ulong idx = hash8_fnv1a_avalanche( i2s.ar );

	// Ensure buckets array is large enough
	if ( idx >= d->size )
	{
		if ( dict_o_expand( d , idx + 1 ) != errOK )
		{
			return errMemoryLow;
		}
	}

	entry_o_t * e = d->buckets[ idx ];
	while ( e )
	{
		if ( e->key == key )
		{
			// Update existing
			e->value = value;
			return errOK;
		}
		e = e->next;
	}

	// Insert new
	entry_o_t * new_e = MALLOC_ONE( new_e );
	if ( !new_e )
	{
		return errMemoryLow;
	}
	new_e->key = key;
	new_e->value = value;
	new_e->next = d->buckets[ idx ];
	d->buckets[ idx ] = new_e;

	d->key_count++; // new key added

	return errOK;
}

void_p dict_o_get( dict_o_t * d , int key )
{
	_i2s i2s = { 0 };
	i2s.i = key;
	ulong idx = hash8_fnv1a_avalanche( i2s.ar );

	if ( idx >= d->size ) return NULL;

	entry_o_t * e = d->buckets[ idx ];
	while ( e )
	{
		if ( e->key == key )
			return e->value;
		e = e->next;
	}
	return NULL;
}

size_t dict_o_count( dict_o_t * d )
{
	return d->key_count;
}

void dict_o_free( dict_o_t * d )
{
	if ( !d ) return;

	for ( size_t ibuck = 0; ibuck < d->size; ibuck++ )
	{
		entry_o_t * e = d->buckets[ ibuck ];
		while ( e )
		{
			entry_o_t * tmp = e;
			e = e->next;
			if ( tmp->value && d->bfree_values_addrs )
			{
				FREE( tmp->value );
			}
			DAC( tmp );
		}
	}

	DAC( d->buckets );
}
