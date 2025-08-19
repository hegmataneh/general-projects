#define Uses_strcmp
#define Uses_pthread_mutex_init
#define Uses_dict
#include <general.dep>

typedef struct entry
{
	LPSTR  key;
	LPSTR  value;
	struct entry * next;
} entry_t;

struct dict
{
	size_t capacity;
	entry_t ** buckets;
	pthread_mutex_t * locks;
};

// Simple string hash function (djb2)
static ulong hash( LPCSTR str )
{
	ulong h = 5381;
	uchar c;
	while ( ( c = (uchar)*str++ ) )
		h = ( ( h << 5 ) + h ) + c; // h * 33 + c
	return h;
}

status dict_init( dict_t ** dd , size_t capacity )
{
	(*dd) = malloc( sizeof( dict_t ) );
	if ( !(*dd) ) return errGeneral;

	(*dd)->capacity = capacity;
	(*dd)->buckets = calloc( capacity , sizeof( entry_t * ) );
	(*dd)->locks = malloc( sizeof( pthread_mutex_t ) * capacity );

	if ( !(*dd)->buckets || !(*dd)->locks )
	{
		DAC( (*dd)->buckets );
		DAC( (*dd)->locks );
		DAC( (*dd) );
		return errGeneral;
	}

	for ( size_t i = 0; i < capacity; i++ )
		pthread_mutex_init( &(*dd)->locks[ i ] , NULL );

	return errOK;
}

void dict_free( dict_t * d )
{
	if ( !d ) return;

	for ( size_t i = 0; i < d->capacity; i++ )
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
		pthread_mutex_destroy( &d->locks[ i ] );
	}

	DAC( d->buckets );
	DAC( d->locks );
	DAC( d );
}

status dict_put( dict_t * d , LPCSTR key , LPCSTR value )
{
	ulong h = hash( key ) % d->capacity;
	pthread_mutex_lock( &d->locks[ h ] );

	entry_t * e = d->buckets[ h ];
	while ( e )
	{
		if ( strcmp( e->key , key ) == 0 )
		{
			// Update existing
			DAC( e->value );
			e->value = strdup( value );
			pthread_mutex_unlock( &d->locks[ h ] );
			return errOK;
		}
		e = e->next;
	}

	// Insert new
	entry_t * new_e = malloc( sizeof( entry_t ) );
	if ( !new_e )
	{
		pthread_mutex_unlock( &d->locks[ h ] );
		return errGeneral;
	}
	new_e->key = strdup( key );
	new_e->value = strdup( value );
	new_e->next = d->buckets[ h ];
	d->buckets[ h ] = new_e;

	pthread_mutex_unlock( &d->locks[ h ] );
	return errOK;
}

LPCSTR dict_get( dict_t * d , LPCSTR key )
{
	ulong h = hash( key ) % d->capacity;
	pthread_mutex_lock( &d->locks[ h ] );

	entry_t * e = d->buckets[ h ];
	while ( e )
	{
		if ( strcmp( e->key , key ) == 0 )
		{
			LPCSTR val = e->value;
			pthread_mutex_unlock( &d->locks[ h ] );
			return val; // pointer valid until updated
		}
		e = e->next;
	}

	pthread_mutex_unlock( &d->locks[ h ] );
	return NULL;
}

