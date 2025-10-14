#define Uses_FREE_PTR
#define Uses_STRDUP
#define Uses_dict_fst
#include <general.dep>

status dict_fst_create( kv_table_t * tbl , size_t nbuckets )
{
	if ( nbuckets == 0 ) nbuckets = INITIAL_BUCKET_COUNT;

	tbl->buckets = CALLOC( nbuckets , sizeof( kv_bucket_t ) );
	if ( !tbl->buckets )
	{
		return errMemoryLow;
	}
	tbl->next_id = 1; // start IDs from 1
	pthread_mutex_init(&tbl->id_lock, NULL);   // init lock
	tbl->nbuckets = nbuckets;
	atomic_init( &tbl->nitems , 0 );

	for ( size_t i = 0; i < nbuckets; ++i )
	{
		tbl->buckets[ i ].head = NULL;
		pthread_rwlockattr_t a;
		pthread_rwlockattr_init( &a );
		// set prefer writer? keep default
		pthread_rwlock_init( &tbl->buckets[ i ].rwlock , &a );
		pthread_rwlockattr_destroy( &a );
	}
	return errOK;
}

_PRIVATE_FXN inline kv_bucket_t * dict_fst_get_bucket( kv_table_t * t , uint64 hash )
{
	return &t->buckets[ hash % t->nbuckets ];
}

void dict_fst_destroy( kv_table_t * tbl )
{
	if ( !tbl ) return;
	for ( size_t i = 0; i < tbl->nbuckets; ++i )
	{
		kv_bucket_t * b = &tbl->buckets[ i ];
		pthread_rwlock_wrlock( &b->rwlock );
		kv_entry_t * e = b->head;
		while ( e )
		{
			kv_entry_t * nx = e->next;
			pthread_spin_destroy( &e->entry_lock );
			FREE_PTR( e->key );
			FREE_PTR( e );
			e = nx;
		}
		b->head = NULL;
		pthread_rwlock_unlock( &b->rwlock );
		pthread_rwlock_destroy( &b->rwlock );
	}
	pthread_mutex_destroy(&tbl->id_lock);   // init lock

	FREE_PTR( tbl->buckets );
}

/*
 Insert or update a key.
 */
status dict_fst_put( kv_table_t * t , const char * key , int ival , void_p pval ,
	_RET_VAL_P uint64 * key_hash /*=NULL*/ , _RET_VAL_P uint64 * uniq_id /*=NULL*/ , int * istat /*=NULL*/ )
{
	uint64 h = hash64_fnv1a_avalanche( key );
	if ( key_hash ) *key_hash = h;
	kv_bucket_t * b = dict_fst_get_bucket( t , h );

	// allocate new entry first (optimistic): reduces time bucket is locked
	// TODO . later if we have repelase  least used item is good idea
	kv_entry_t * newe = MALLOC( sizeof( *newe ) );
	if ( !newe ) return errMemoryLow;
	newe->key = STRDUP( key );
	if ( !newe->key )
	{
		FREE_PTR( newe ); return errMemoryLow;
	}
	newe->hash = h;
	newe->v.int_val = ival;
	newe->v.p_val = pval;
	pthread_spin_init( &newe->entry_lock , PTHREAD_PROCESS_PRIVATE );
	newe->next = NULL;

	// Acquire write lock for bucket for update/insert
	pthread_rwlock_wrlock( &b->rwlock );

	kv_entry_t * cur = b->head;
	while ( cur )
	{
		if ( cur->hash == h && strcmp( cur->key , key ) == 0 )
		{
			// found existing: update it
			pthread_spin_lock( &cur->entry_lock );
			cur->v.p_val = pval; // pointer assigned first . because it is more error prone
			cur->v.int_val = ival;
			pthread_spin_unlock( &cur->entry_lock );

			pthread_rwlock_unlock( &b->rwlock );
			// free new entry we allocated but didn't use
			pthread_spin_destroy( &newe->entry_lock );
			FREE_PTR( newe->key );
			FREE_PTR( newe );
			if ( istat ) *istat = 1;
			return errOK;
		}
		cur = cur->next;
	}

	// assign new ID
	pthread_mutex_lock( &t->id_lock );
	uint64 new_id = t->next_id++;
	pthread_mutex_unlock( &t->id_lock );

	// not found => insert at head
	newe->next = b->head;
	b->head = newe;
	newe->id = new_id;
	if ( uniq_id ) *uniq_id = newe->id;

	atomic_fetch_add_explicit( &t->nitems , 1 , memory_order_relaxed );

	pthread_rwlock_unlock( &b->rwlock );
	if ( istat ) *istat = 0;
	return errOK;
}

/*
 Lookup (peek) value by key.
 Returns 0 and sets *out if found, -1 if not found.
 This is safe for concurrent readers (acquires bucket read lock).
*/
status dict_fst_get_bykey( kv_table_t * t , _IN const char * key , _RET_VAL_P int * int_out , _RET_VAL_P void_p * p_out )
{
	uint64 h = hash64_fnv1a_avalanche( key );
	kv_bucket_t * b = dict_fst_get_bucket( t , h );

	pthread_rwlock_rdlock( &b->rwlock );
	kv_entry_t * cur = b->head;
	while ( cur )
	{
		if ( cur->hash == h && strcmp( cur->key , key ) == 0 )
		{
			// read value atomically while holding entry lock? value is int; read is atomic on most platforms
			// but to be safe, grab entry spinlock for a short time
			pthread_spin_lock( &cur->entry_lock );
			int v = cur->v.int_val;
			void_p p = cur->v.p_val;
			pthread_spin_unlock( &cur->entry_lock );

			pthread_rwlock_unlock( &b->rwlock );
			if ( int_out ) *int_out = v;
			if ( p_out ) *p_out = p;
			return errOK;
		}
		cur = cur->next;
	}
	pthread_rwlock_unlock( &b->rwlock );
	return errNotFound;
}

/*
 Lookup (peek) value by key.
 Returns 0 and sets *out if found, -1 if not found.
 This is safe for concurrent readers (acquires bucket read lock).
*/
status dict_fst_get_faster_by_hash_id( kv_table_t * t , uint64 key_hash , uint64 uniq_id , int * int_out , void_p * p_out )
{
	kv_bucket_t * b = dict_fst_get_bucket( t , key_hash );

	pthread_rwlock_rdlock( &b->rwlock );
	kv_entry_t * cur = b->head;
	while ( cur )
	{
		if ( cur->hash == key_hash && cur->id == uniq_id )
		{
			// read value atomically while holding entry lock? value is int; read is atomic on most platforms
			// but to be safe, grab entry spinlock for a short time
			pthread_spin_lock( &cur->entry_lock );
			int v = cur->v.int_val;
			void_p p = cur->v.p_val;
			pthread_spin_unlock( &cur->entry_lock );

			pthread_rwlock_unlock( &b->rwlock );
			if ( p_out ) *p_out = p; // pointer must be first assigned
			if ( int_out ) *int_out = v;
			return errOK;
		}
		cur = cur->next;
	}
	pthread_rwlock_unlock( &b->rwlock );
	return errNotFound;
}

status dict_fst_get_hash_id_bykey( kv_table_t * t , const char * key , _RET_VAL_P uint64 * key_hash /*=NULL*/ , _RET_VAL_P uint64 * uniq_id /*=NULL*/ )
{
	uint64 h = hash64_fnv1a_avalanche( key );
	kv_bucket_t * b = dict_fst_get_bucket( t , h );

	pthread_rwlock_rdlock( &b->rwlock );
	kv_entry_t * cur = b->head;
	while ( cur )
	{
		if ( cur->hash == h && strcmp( cur->key , key ) == 0 )
		{
			// read value atomically while holding entry lock? value is int; read is atomic on most platforms
			// but to be safe, grab entry spinlock for a short time
			pthread_spin_lock( &cur->entry_lock );
			uint64 tmp_hash = cur->hash;
			uint64 tmp_id = cur->id;
			pthread_spin_unlock( &cur->entry_lock );

			pthread_rwlock_unlock( &b->rwlock );
			if ( key_hash ) *key_hash = tmp_hash;
			if ( uniq_id ) *uniq_id = tmp_id;
			return errOK;
		}
		cur = cur->next;
	}
	pthread_rwlock_unlock( &b->rwlock );
	return errNotFound;
}

status dict_forcibly_get_hash_id_bykey( kv_table_t * t , const char * key , int ival , void_p pval , _RET_VAL_P uint64 * key_hash /*=NULL*/ , _RET_VAL_P uint64 * uniq_id /*=NULL*/ )
{
	status ret = dict_fst_get_hash_id_bykey( t , key , key_hash , uniq_id );
	if ( ret == errOK ) return ret;
	dict_fst_put( t , key , ival , pval , NULL , NULL , NULL );
	return dict_fst_get_hash_id_bykey( t , key , key_hash , uniq_id );
}

/*
 Delete key. Returns 0 if deleted, -1 if not found.
*/
status dict_fst_delete( kv_table_t * t , const char * key )
{
	uint64 h = hash64_fnv1a_avalanche( key );
	kv_bucket_t * b = dict_fst_get_bucket( t , h );

	pthread_rwlock_wrlock( &b->rwlock );

	kv_entry_t * prev = NULL;
	kv_entry_t * cur = b->head;
	while ( cur )
	{
		if ( cur->hash == h && strcmp( cur->key , key ) == 0 )
		{
			// lock the entry to ensure no-one else is modifying
			pthread_spin_lock( &cur->entry_lock );

			// unlink
			if ( prev ) prev->next = cur->next;
			else b->head = cur->next;

			pthread_spin_unlock( &cur->entry_lock );
			pthread_spin_destroy( &cur->entry_lock );
			FREE_PTR( cur->key );
			FREE_PTR( cur );
			atomic_fetch_sub_explicit( &t->nitems , 1 , memory_order_relaxed );

			pthread_rwlock_unlock( &b->rwlock );
			return errOK;
		}
		prev = cur;
		cur = cur->next;
	}

	pthread_rwlock_unlock( &b->rwlock );
	return errNotFound;
}

/*
 Lock a key for exclusive update/delete by caller.
 Returns 0 and locks the key; caller must call dict_fst_unlock_key() when done.
 If key not found returns -1.
 Note: while you hold the entry lock you should only call:
   - dict_fst_update_locked(t, key, newval)
   - dict_fst_delete_locked(t, key)  (this will remove the entry and release its resources)
   - dict_fst_unlock_key(t, key)
 The locking order is: bucket rwlock (read or write) then entry spinlock.
*/
int dict_fst_lock_key( kv_table_t * t , const char * key )
{
	uint64 h = hash64_fnv1a_avalanche( key );
	kv_bucket_t * b = dict_fst_get_bucket( t , h );

	// Acquire bucket read lock first (consistent ordering)
	pthread_rwlock_rdlock( &b->rwlock );

	kv_entry_t * cur = b->head;
	while ( cur )
	{
		if ( cur->hash == h && strcmp( cur->key , key ) == 0 )
		{
			// lock the entry while still holding bucket lock (prevents it from being removed under us)
			pthread_spin_lock( &cur->entry_lock );
			// now safe to release bucket lock
			pthread_rwlock_unlock( &b->rwlock );
			return 0;
		}
		cur = cur->next;
	}

	pthread_rwlock_unlock( &b->rwlock );
	return -1;
}

/*
 Unlock previously locked key.
*/
status dict_fst_unlock_key( kv_table_t * t , const char * key )
{
	uint64 h = hash64_fnv1a_avalanche( key );
	kv_bucket_t * b = dict_fst_get_bucket( t , h );

	// We need to find the entry and unlock it. Acquire bucket read lock to find it.
	pthread_rwlock_rdlock( &b->rwlock );
	kv_entry_t * cur = b->head;
	while ( cur )
	{
		if ( cur->hash == h && strcmp( cur->key , key ) == 0 )
		{
			pthread_spin_unlock( &cur->entry_lock );
			pthread_rwlock_unlock( &b->rwlock );
			return errOK;
		}
		cur = cur->next;
	}
	pthread_rwlock_unlock( &b->rwlock );
	return errNotFound;
}

/*
 Update value while holding the lock acquired by dict_fst_lock_key.
 Returns 0 on success, -1 if not found.
*/
status dict_fst_update_locked( kv_table_t * t , const char * key , int new_value )
{
	// Assume caller holds the entry spinlock, but we still need to find entry
	uint64 h = hash64_fnv1a_avalanche( key );
	kv_bucket_t * b = dict_fst_get_bucket( t , h );

	// bucket read lock to find entry
	pthread_rwlock_rdlock( &b->rwlock );
	kv_entry_t * cur = b->head;
	while ( cur )
	{
		if ( cur->hash == h && strcmp( cur->key , key ) == 0 )
		{
			// We assume caller holds cur->entry_lock; set value
			cur->v.int_val = new_value;
			pthread_rwlock_unlock( &b->rwlock );
			return errOK;
		}
		cur = cur->next;
	}
	pthread_rwlock_unlock( &b->rwlock );
	return errNotFound;
}

/*
 Delete an entry while holding its lock (dict_fst_lock_key must have been called).
 This will remove it from chain and free memory.
 Returns 0 on success, -1 if not found.
*/
status dict_fst_delete_locked( kv_table_t * t , const char * key )
{
	uint64 h = hash64_fnv1a_avalanche( key );
	kv_bucket_t * b = dict_fst_get_bucket( t , h );

	// To delete safely we need bucket write lock and entry locked.
	// Acquire write lock (consistent order: bucket then entry; but entry is already locked by caller)
	pthread_rwlock_wrlock( &b->rwlock );

	kv_entry_t * prev = NULL;
	kv_entry_t * cur = b->head;
	while ( cur )
	{
		if ( cur->hash == h && strcmp( cur->key , key ) == 0 )
		{
			// entry should already be locked by caller
			// unlink
			if ( prev ) prev->next = cur->next;
			else b->head = cur->next;

			pthread_spin_unlock( &cur->entry_lock );
			pthread_spin_destroy( &cur->entry_lock );
			FREE_PTR( cur->key );
			FREE_PTR( cur );
			atomic_fetch_sub_explicit( &t->nitems , 1 , memory_order_relaxed );

			pthread_rwlock_unlock( &b->rwlock );
			return errOK;
		}
		prev = cur;
		cur = cur->next;
	}

	pthread_rwlock_unlock( &b->rwlock );
	return errNotFound;
}

/* Utility: count of items (approx, atomic) */
size_t dict_fst_size( kv_table_t * t )
{
	return ( size_t )atomic_load_explicit( &t->nitems , memory_order_relaxed );
}
