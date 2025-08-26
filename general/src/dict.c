#define Uses_INIT_BREAKABLE_FXN
#define Uses_MEMSET_ZERO
#define Uses_pthread_mutex_init
#define Uses_dict
#include <general.dep>


// Simple string hash function (djb2)
static ulong hash( LPCSTR str )
{
	ulong h = 5381;
	uchar c;
	while ( ( c = (uchar)*str++ ) )
		h = ( ( h << 5 ) + h ) + c; // h * 33 + c
	return h;
}

status dict_init( dict_t * dd )
{
	MEMSET_ZERO_O( dd );
	return errOK;
}

// Expand bucket array dynamically if needed
status dict_expand( dict_t * d , size_t new_size )
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

status dict_put( dict_t * d , LPCSTR key , LPCSTR value )
{
	unsigned long idx = hash(key);

	// Ensure buckets array is large enough
	if ( idx >= d->size )
	{
		if ( dict_expand( d , idx + 1 ) != errOK )
		{
			return errMemoryLow;
		}
	}

	entry_t * e = d->buckets[ idx ];
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
	entry_t * new_e = MALLOC_ONE( new_e );
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

LPCSTR dict_get( dict_t * d , LPCSTR key )
{
	unsigned long idx = hash( key );
	if ( idx >= d->size ) return NULL;

	entry_t * e = d->buckets[ idx ];
	while ( e )
	{
		if ( strcmp( e->key , key ) == 0 )
			return e->value;
		e = e->next;
	}
	return NULL;
}

size_t dict_count( dict_t * d )
{
	return d->key_count;
}

void dict_free( dict_t * d )
{
	if ( !d ) return;

	for ( size_t i = 0; i < d->size; i++ )
	{
		entry_t * e = d->buckets[ i ];
		while ( e )
		{
			entry_t * tmp = e;
			e = e->next;
			DAC( tmp->key );
			DAC( tmp->value );
			DAC( tmp );
		}
	}

	DAC( d->buckets );
}

status dict_get_keys( dict_t * d , _OUT LPCSTR ** strs , _OUT int * count )
{
	INIT_BREAKABLE_FXN();
	*count = ( int )dict_count( d );
	M_MALLOC_AR( *strs , d->key_count , 0 );
	size_t pos = 0;

	for ( size_t i = 0; i < d->size; i++ )
	{
		entry_t * e = d->buckets[ i ];
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
