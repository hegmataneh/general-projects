#define Uses_MEMSET_ZERO
#define Uses_pthread_mutex_t
#define Uses_id_dict
#include <general.dep>

status id_dict_init( id_dict_t ** pdict , size_t capacity)
{
	(*pdict) = MALLOC_ONE( *pdict );
	if ( !(*pdict) ) return errGeneral;
	MEMSET_ZERO_O( (*pdict) );

	(*pdict)->capacity = capacity;
	(*pdict)->buckets = calloc( capacity , sizeof( id_dict_entry_t * ) );
	(*pdict)->locks = malloc( sizeof( pthread_mutex_t ) * capacity );
	(*pdict)->counter = 0;

	if ( !(*pdict)->buckets || !(*pdict)->locks )
	{
		DAC( (*pdict)->buckets );
		DAC( (*pdict)->locks );
		DAC( (*pdict) );
		return errGeneral;
	}

	for ( size_t i = 0; i < capacity; i++ )
		pthread_mutex_init( &(*pdict)->locks[ i ] , NULL );
	pthread_mutex_init( &(*pdict)->counter_lock , NULL );

	return errOK;
}

void id_dict_free( id_dict_t * d )
{
	if ( !d ) return;

	for ( size_t i = 0; i < d->capacity; i++ )
	{
		id_dict_entry_t * e = d->buckets[ i ];
		while ( e )
		{
			id_dict_entry_t * tmp = e;
			e = e->next;
			DAC( tmp->key );
			DAC( tmp );
		}
		pthread_mutex_destroy( &d->locks[ i ] );
	}

	pthread_mutex_destroy( &d->counter_lock );
	DAC( d->buckets );
	DAC( d->locks );
	DAC( d );
}

long id_dict_put_or_get( id_dict_t * d , LPCSTR key )
{
	if ( !d || !key ) return -1;

	ulong h = ( ulong )hash8_fnv1a_avalanche( key ) % d->capacity;

	/* Lock bucket */
	pthread_mutex_lock( &d->locks[ h ] );

	id_dict_entry_t * e = d->buckets[ h ];
	while ( e )
	{
		if ( strcmp( e->key , key ) == 0 )
		{
			long val = e->value;
			pthread_mutex_unlock( &d->locks[ h ] );
			return val;
		}
		e = e->next;
	}

	/* Key not found → assign new ID */
	pthread_mutex_lock( &d->counter_lock );
	long new_id = d->counter++;
	pthread_mutex_unlock( &d->counter_lock );

	id_dict_entry_t * new_e = malloc( sizeof( id_dict_entry_t ) );
	if ( !new_e )
	{
		pthread_mutex_unlock( &d->locks[ h ] );
		return -1;
	}
	new_e->key = strdup( key );
	if ( !new_e->key )
	{
		DAC( new_e );
		pthread_mutex_unlock( &d->locks[ h ] );
		return -1;
	}
	new_e->value = new_id;
	new_e->next = d->buckets[ h ];
	d->buckets[ h ] = new_e;

	pthread_mutex_unlock( &d->locks[ h ] );
	return new_id;
}
