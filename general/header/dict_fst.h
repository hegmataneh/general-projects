#pragma once

#if defined Uses_dict_fst || !defined __COMPILING

#ifndef INITIAL_BUCKET_COUNT
// a large-ish prime-like number of buckets; adjust to expected size
#define INITIAL_BUCKET_COUNT 131071u
#endif

typedef struct kv_entry
{
	uint64 hash;					// cached hash
	uint64 id;						// unique numeric key

	char * key;						// heap-allocated copy of key
	struct s_value
	{
		int int_val;
		void_p p_val;
	} v;

	pthread_spinlock_t entry_lock;	// per-entry exclusive lock
	struct kv_entry * next;			// chain link
} kv_entry_t;

typedef struct kv_bucket /*each bucket consist many key each one have value. each key value is static*/
{
	kv_entry_t * head;
	pthread_rwlock_t rwlock;		// readers-writer lock for the bucket
} kv_bucket_t;

typedef struct kv_table
{
	kv_bucket_t * buckets;
	size_t nbuckets;
	uint64 next_id;					// atomic counter for assigning unique IDs
	atomic_size_t nitems;
	pthread_mutex_t id_lock;
} kv_table_t;

// create dictionary with hit array size
status dict_fst_create( kv_table_t * tbl , size_t nbuckets /*count of array size dic use, =0 use default val*/ );
void dict_fst_destroy( kv_table_t * tbl );
status dict_fst_put( kv_table_t * t , const char * key , int ival , void_p pval ,
	_RET_VAL_P uint64 * key_hash /*=NULL*/ , _RET_VAL_P uint64 * uniq_id /*=NULL*/ , int * istat /*=NULL*/ );

status dict_fst_get_bykey( kv_table_t * t , const char * key , _RET_VAL_P int * int_out , _RET_VAL_P void_p * p_out );
status dict_fst_get_faster_by_hash_id( kv_table_t * t , uint64 key_hash , uint64 uniq_id , int * out , void_p * p_out );

status dict_fst_get_hash_id_bykey( kv_table_t * t , const char * key , _RET_VAL_P uint64 * key_hash /*=NULL*/ , _RET_VAL_P uint64 * uniq_id /*=NULL*/ );
status dict_forcibly_get_hash_id_bykey( kv_table_t * t , const char * key , int ival , void_p pval , _RET_VAL_P uint64 * key_hash /*=NULL*/ , _RET_VAL_P uint64 * uniq_id /*=NULL*/ );

status dict_fst_delete( kv_table_t * t , const char * key );
int dict_fst_lock_key( kv_table_t * t , const char * key );
status dict_fst_unlock_key( kv_table_t * t , const char * key );
status dict_fst_update_locked( kv_table_t * t , const char * key , int new_value );
status dict_fst_delete_locked( kv_table_t * t , const char * key );
size_t dict_fst_size( kv_table_t * t );

#endif

#ifdef DEMO_MAIN
#define N_THREADS 4
#define OPS_PER_THREAD 20000

typedef struct demo_arg
{
	kv_table_t * t;
	int id;
} demo_arg_t;

void * producer_thread( void * arg )
{
	demo_arg_t * a = arg;
	kv_table_t * t = a->t;
	int id = a->id;
	char keybuf[ 64 ];
	for ( int i = 0; i < OPS_PER_THREAD; ++i )
	{
		snprintf( keybuf , sizeof( keybuf ) , "k-%d-%d" , id , i ); // short keys
		ht_put( t , keybuf , id * 1000000 + i );
		// sometimes peek
		int v;
		if ( ht_get( t , keybuf , &v ) == 0 )
		{
			// ok
		}
		if ( ( i & 127 ) == 0 )
		{
			// update existing quickly
			ht_put( t , keybuf , v + 1 );
		}
		if ( ( i & 1023 ) == 0 )
		{
			// delete sometimes
			ht_delete( t , keybuf );
		}
	}
	return NULL;
}

int main( void )
{
	puts( "Starting demo hash table" );
	kv_table_t * t = ht_create( 16384 );
	pthread_t th[ N_THREADS ];
	demo_arg_t args[ N_THREADS ];

	for ( int i = 0; i < N_THREADS; ++i )
	{
		args[ i ].t = t;
		args[ i ].id = i;
		pthread_create( &th[ i ] , NULL , producer_thread , &args[ i ] );
	}
	for ( int i = 0; i < N_THREADS; ++i ) pthread_join( th[ i ] , NULL );

	printf( "Done. size=%zu\n" , ht_size( t ) );

	// quick test of lock/update/delete API:
	const char * k = "test-key";
	ht_put( t , k , 42 );
	if ( ht_lock_key( t , k ) == 0 )
	{
		ht_update_locked( t , k , 1234 );
		ht_unlock_key( t , k );
	}
	int outv;
	if ( ht_get( t , k , &outv ) == 0 ) printf( "value=%d\n" , outv );
	ht_delete( t , k );

	ht_destroy( t );
	return 0;
}
#endif
