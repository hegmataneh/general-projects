#define Uses_pthread_mutex_t
#define Uses_MEMSET_ZERO
#define Uses_cirbuf_infinite
#include <general.dep>


/* Allocate and initialize a segment with given capacities */
_PRIVATE_FXN status ci_sgm_create( ci_sgm_t ** psgm , size_t buf_capacity , size_t offsets_capacity )
{
	( *psgm ) = CALLOC_ONE( ( *psgm ) );
	if ( !( *psgm ) ) return errMemoryLow;

	( *psgm )->buf_capacity = buf_capacity;
	( *psgm )->buf = MALLOC_AR( ( *psgm )->buf , buf_capacity );
	if ( !( *psgm )->buf )
	{
		DAC( ( *psgm ) );
		return errMemoryLow;
	}

	( *psgm )->offsets_capacity = offsets_capacity;
	( *psgm )->offsets = MALLOC_AR( ( *psgm )->offsets , offsets_capacity );
	( *psgm )->sizes = MALLOC_AR( ( *psgm )->sizes , offsets_capacity );
	if ( !( *psgm )->offsets || !( *psgm )->sizes )
	{
		DAC( ( *psgm )->buf );
		DAC( ( *psgm )->offsets );
		DAC( ( *psgm )->sizes );
		DAC( ( *psgm ) );
		return errMemoryLow;
	}

	(*psgm)->queue_next = NULL;
	(*psgm)->queue_prev = NULL;
	//(*psgm)->buf_used = 0; // init by calloc
	//(*psgm)->itm_count = 0;
	( *psgm )->next = ( *psgm )->prev = ( *psgm );
	( *psgm )->active_lock = False;
	//( *psgm )->filled = False;
	( *psgm )->in_filled_queue = False;

	return errOK;
}

_PRIVATE_FXN void ci_sgm_free( ci_sgm_t * s )
{
	if ( !s ) return;
	DAC( s->buf );
	DAC( s->offsets );
	DAC( s->sizes );
	FREE( s );
}

/* Grow offsets arrays; returns 0 on success */
_PRIVATE_FXN status ci_sgm_grow_offsets( ci_sgm_t * s )
{
	size_t newcap = s->offsets_capacity * OFFSETS_GROW_FACTOR;
	if ( newcap == 0 ) newcap = DEFAULT_OFFSETS_CAP;
	size_t * no = REALLOC_AR( s->offsets , newcap );
	if ( !no ) return errMemoryLow;
	MEMSET_ZERO( no + s->offsets_capacity , newcap - s->offsets_capacity );

	size_t * nsz = REALLOC_AR( s->sizes , newcap );
	if ( !nsz )
	{
		/* rollback */
		s->offsets = REALLOC_AR( no , s->offsets_capacity );
		return errMemoryLow;
	}
	MEMSET_ZERO( nsz + s->offsets_capacity , newcap - s->offsets_capacity );

	s->offsets = no;
	s->sizes = nsz;
	s->offsets_capacity = newcap;
	return errOK;
}

/* Insert segment into ring after 'pos' (pos can be NULL to create single node) */
_PRIVATE_FXN void ci_sgm_insert_after( ci_sgmgr_t * mgr , ci_sgm_t * pos , ci_sgm_t * s )
{
	if ( !mgr->ring )
	{
		mgr->ring = s;
		s->next = s->prev = s;
	}
	else if ( !pos )
	{
		/* insert after ring pointer */
		ci_sgm_t * r = mgr->ring;
		s->next = r->next;
		s->prev = r;
		r->next->prev = s;
		r->next = s;
	}
	else
	{
		s->next = pos->next;
		s->prev = pos;
		pos->next->prev = s;
		pos->next = s;
	}
	mgr->segment_total++;
}

/* Remove segment s from ring. If ring becomes empty, set mgr->ring to NULL */
_PRIVATE_FXN void ci_sgm_remove_from_ring( ci_sgmgr_t * mgr , ci_sgm_t * s )
{
	if ( !mgr->ring || !s ) return;
	if ( s->next == s )
	{
		mgr->ring = NULL;
	}
	else
	{
		s->prev->next = s->next;
		s->next->prev = s->prev;
		if ( mgr->ring == s ) mgr->ring = s->next;
	}
	s->next = s->prev = s;
	if ( mgr->segment_total > 0 ) mgr->segment_total--;
}

/* --- Filled queue ops --- */

/* Push a segment to filled queue (assumes mgr lock is held) */
_PRIVATE_FXN void filled_queue_push( ci_sgmgr_t * mgr , ci_sgm_t * s )
{
	if ( s->in_filled_queue ) return;
	s->in_filled_queue = True;
	s->queue_next = NULL; // end of linek list /* next used as queue link */ // The filled queue is a single-linked FIFO queue, not a circular doubly-linked list. so this line set tail node end
	s->queue_prev = mgr->filled_tail;
	
	//s->filled = True;
	s->active_lock = False;
	//s->next = NULL; 
	if ( !mgr->filled_head )
	{
		mgr->filled_head = mgr->filled_tail = s;
	}
	else
	{
		mgr->filled_tail->queue_next = s;
		mgr->filled_tail = s;
	}
	mgr->filled_count++;
	pthread_cond_signal( &mgr->filled_cond );
}

/* Pop a filled segment from queue FIFO, return it (mgr lock must be held) or NULL if empty */
_PRIVATE_FXN ci_sgm_t * filled_queue_pop( ci_sgmgr_t * mgr )
{
	ci_sgm_t * s = mgr->filled_head;
	if ( !s ) return NULL;
	mgr->filled_head = s->queue_next;
	if ( !mgr->filled_head ) mgr->filled_tail = NULL;
	//s->next = s->prev = s; /* reset ring pointers to self (not re-inserted yet) */
	s->in_filled_queue = False;
	s->queue_next = NULL;
	mgr->filled_count--;
	return s;
}

/* Pop a filled segment from stack LIFO, return it (mgr lock must be held) or NULL if empty */
_PRIVATE_FXN ci_sgm_t * filled_queue_pop_tail( ci_sgmgr_t * mgr )
{
	ci_sgm_t * s = mgr->filled_tail;
	if ( !s ) return NULL;

	mgr->filled_tail = s->queue_prev;
	if ( mgr->filled_tail )
		mgr->filled_tail->queue_next = NULL;
	else
		mgr->filled_head = NULL;

	s->queue_next = s->queue_prev = NULL;
	s->in_filled_queue = False;
	return s;
}

/* ---------- Public Manager API ---------- */

/* Initialize manager. If allow_grow==True, manager will allocate new segments when needed. */
status segmgr_init( ci_sgmgr_t * mgr , size_t default_seg_capacity , size_t default_offsets_capacity , Boolean allow_grow )
{
	if ( !mgr ) return errArg;
	MEMSET_ZERO_O( mgr );
	pthread_mutex_init( &mgr->lock , NULL );
	pthread_cond_init( &mgr->filled_cond , NULL );
	mgr->default_seg_capacity = default_seg_capacity;
	mgr->default_offsets_capacity = default_offsets_capacity;
	mgr->allow_grow = allow_grow;
	mgr->ring = NULL;
	mgr->active = NULL;
	mgr->filled_head = mgr->filled_tail = NULL;
	mgr->filled_count = 0;
	mgr->segment_total = 0;
	mgr->total_items = 0;
	mgr->total_bytes = 0;

	return errOK;
}

/* Create and insert a new empty segment into the ring. Returns allocated segment or NULL. */
_PRIVATE_FXN ci_sgm_t * segmgr_new_segment_locked( ci_sgmgr_t * mgr )
{
	ci_sgm_t * new_sgm = NULL;
	status ret = ci_sgm_create( &new_sgm , mgr->default_seg_capacity , mgr->default_offsets_capacity );
	if ( ret != errOK ) return NULL;
	/* Insert after active if exists, else after ring pointer. */
	if ( mgr->active )
	{
		ci_sgm_insert_after( mgr , mgr->active , new_sgm );
	}
	else
	{
		ci_sgm_insert_after( mgr , mgr->ring , new_sgm );
		//mgr->ring = new_sgm; // if there is no active so ring must be null
	}
	return new_sgm;
}

/* Set a segment as active (assumes mgr lock held). */
_PRIVATE_FXN void segmgr_set_active_locked( ci_sgmgr_t * mgr , ci_sgm_t * s )
{
	if ( mgr->active ) mgr->active->active_lock = False;
	mgr->active = s; // maybe active set to NULL . because cannot allocate next one
	if ( s ) s->active_lock = True; // set it as active
}

/* Append raw item to active segment. Thread-safe.
 * Returns SM_OK on success, SM_NOMEM on allocation failure, SM_OVERFLOW if cannot fit and grow not allowed.
 *
 * Behavior:
 * - If no active segment, prefer any empty segment in ring (count==0).
 * - If none empty and allow_grow, create new segment.
 * - If item size > segment buffer capacity -> error (can't split items).*/
status segmgr_append( ci_sgmgr_t * mgr , const pass_p data , size_t len )
{
	if ( !mgr || !data || !len ) return errArg;
	pthread_mutex_lock( &mgr->lock );

	ci_sgm_t * psgm_active = NULL; // s
	/* First ensure active exists and can accommodate len (with offsets capacity). */
	psgm_active = mgr->active;

	/* If no active, try find an empty segment */
	if ( !psgm_active )
	{
		/* search ring for empty (count==0) segment . from ring to ring*/
		ci_sgm_t * mgr_ring_ft = mgr->ring;
		if ( mgr_ring_ft )
		{
			do
			{
				if ( mgr_ring_ft->itm_count == 0 && !mgr_ring_ft->in_filled_queue )
				{
					psgm_active = mgr_ring_ft;
					break;
				}
				mgr_ring_ft = mgr_ring_ft->next;
				WARNING( mgr_ring_ft ); // if there is circular buf so next never could possibly be null
			} while ( mgr_ring_ft && mgr_ring_ft != mgr->ring );
		}
		/* If still none, try create if allowed */
		if ( !psgm_active )
		{
			if ( !mgr->allow_grow )
			{
				pthread_mutex_unlock( &mgr->lock );
				return errOverflow;
			}
			psgm_active = segmgr_new_segment_locked( mgr ); // until this line no free segment found so create new one
			if ( !psgm_active )
			{
				pthread_mutex_unlock( &mgr->lock );
				return errMemoryLow;
			}
			/* If ring pointer was empty, ensure manager ring points to this new seg */
			//if ( !mgr->ring ) mgr->ring = psgm_active;
		}
		WARNING( psgm_active );
		segmgr_set_active_locked( mgr , psgm_active );
	}

	/* If item is larger than buffer capacity -> error (we don't split items) */
	WARNING( len <= mgr->default_seg_capacity );
	if ( len > psgm_active->buf_capacity )
	{
		segmgr_set_active_locked( mgr , NULL );
		psgm_active = NULL;
		//pthread_mutex_unlock( &mgr->lock );
	}

	/* If not enough free bytes in this segment, we need to move to next empty or create new */
	if ( psgm_active->buf_used + len > psgm_active->buf_capacity )
	{
		/* mark current active as filled and push to filled queue */
		filled_queue_push( mgr , psgm_active );

		/* find next empty segment */
		ci_sgm_t * next_empty = NULL;
		ci_sgm_t * pnext = psgm_active->next;
		WARNING( pnext && mgr->ring ); // there is active seg so how ring could be null
		if ( pnext && mgr->ring )
		{
			do
			{
				if ( pnext->itm_count == 0 && !pnext->in_filled_queue )
				{
					next_empty = pnext;
					break;
				}
				pnext = pnext->next;
				WARNING( pnext ); // circular???
			} while ( pnext != mgr->ring );
		}

		if ( !next_empty && mgr->allow_grow )
		{
			next_empty = segmgr_new_segment_locked( mgr );
			if ( !next_empty )
			{
				pthread_mutex_unlock( &mgr->lock );
				return errMemoryLow;
			}
		}

		if ( !next_empty )
		{
			/* no place to write */
			pthread_mutex_unlock( &mgr->lock );
			return errOverflow;
		}

		segmgr_set_active_locked( mgr , next_empty );
		psgm_active = mgr->active;
	}

	WARNING( psgm_active );

	/* Ensure offsets capacity */
	if ( psgm_active->itm_count + 1 > psgm_active->offsets_capacity )
	{
		status rc = ci_sgm_grow_offsets( psgm_active );
		if ( rc != errOK )
		{
			pthread_mutex_unlock( &mgr->lock );
			return rc;
		}
	}

	/* Append data into buffer */
	size_t off = psgm_active->buf_used;
	MEMCPY_OR( psgm_active->buf + off , data , len );
	psgm_active->offsets[ psgm_active->itm_count ] = off;
	psgm_active->sizes[ psgm_active->itm_count ] = len;
	psgm_active->itm_count++;
	psgm_active->buf_used += len;

	/* Update manager stats */
	mgr->total_items++;
	mgr->total_bytes += len;

	WARNING( psgm_active->buf_used <= psgm_active->buf_capacity );

	/* If segment becomes exactly full, mark filled and move active */
	if ( psgm_active->buf_used == psgm_active->buf_capacity )
	{
		filled_queue_push( mgr , psgm_active );

		/* Try to find next empty or create */
		ci_sgm_t * next_empty = NULL;
		ci_sgm_t * pnext = psgm_active->next;
		if ( pnext && mgr->ring )
		{
			do
			{
				if ( pnext->itm_count == 0 && !pnext->in_filled_queue )
				{
					next_empty = pnext;
					break;
				}
				pnext = pnext->next;
			} while ( pnext != mgr->ring );
		}
		if ( !next_empty && mgr->allow_grow )
		{
			next_empty = segmgr_new_segment_locked( mgr );
		}
		segmgr_set_active_locked( mgr , next_empty ); /* may set to NULL if none */
	}

	pthread_mutex_unlock( &mgr->lock );
	return errOK;
}

/* Pop a filled segment for flushing.
* If block==True, block until a filled segment is available.
* Returns pointer to a segment (caller owns it until calling ci_sgm_mark_empty()).
*
* Note: returned segment remains *in the ring*. Consumers should not modify next/prev links.
* After flush, consumer must call ci_sgm_mark_empty(mgr, seg) to mark it empty and reusable.*/
ci_sgm_t * segmgr_pop_filled_segment( ci_sgmgr_t * mgr , Boolean block , seg_trv trv )
{
	if ( !mgr ) return NULL;
	pthread_mutex_lock( &mgr->lock );
	while ( !mgr->filled_head )
	{
		if ( !block )
		{
			pthread_mutex_unlock( &mgr->lock );
			return NULL;
		}
		pthread_cond_wait( &mgr->filled_cond , &mgr->lock );
	}
	ci_sgm_t * s = NULL;

	switch ( trv )
	{
		case seg_trv_FIFO:
		{
			s = filled_queue_pop( mgr );
			break;
		}
		case seg_trv_LIFO:
		{
			s = filled_queue_pop_tail( mgr );
			break;
		}
	}
	/* popped segment is detached from queue but still belongs to ring (we kept next==prev==self) */
	/* Keep its ring pointers as-is; consumers can read s->buf, s->itm_count, offsets, sizes */
	pthread_mutex_unlock( &mgr->lock );
	return s;
}

/* After consumer flushes segment's items to persistent storage, call this to mark it empty and reusable.
* It resets the segment's internal counters and inserts it back into ring if necessary.
*
* This function is thread-safe.*/
status ci_sgm_mark_empty( ci_sgmgr_t * mgr , ci_sgm_t * s )
{
	if ( !mgr || !s ) return errArg;
	pthread_mutex_lock( &mgr->lock );

	/* Reset */
	mgr->total_items -= s->itm_count;
	/* subtract bytes */
	size_t bytes = s->buf_used;
	if (mgr->total_bytes >= bytes) mgr->total_bytes -= bytes;
	s->buf_used = 0;
	s->itm_count = 0;
	//s->filled = False;
	s->in_filled_queue = False;
	/* do not free offsets arrays; keep capacity to reuse */

	/* Keep segment in ring as-is; if no active, prefer this as active */
	if ( !mgr->active )
	{
		segmgr_set_active_locked( mgr , s );
	}
	pthread_mutex_unlock( &mgr->lock );
	return errOK;
}

/* Read item at index idx (0 <= idx < s->itm_count) from segment s.
* Returns pointer to internal buffer (valid while segment not modified) and size via out parameter.
* O(1).*/
//status ci_sgm_get_item( ci_sgm_t * s , size_t idx , const void ** out_ptr , size_t * out_len )
//{
//	if ( !s || !out_ptr || !out_len ) return errArg;
//	if ( idx >= s->itm_count ) return errNotFound;
//	*out_ptr = ( const void * )( s->buf + s->offsets[ idx ] );
//	*out_len = s->sizes[ idx ];
//	return errOK;
//}

/* Optional utility: iterate items sequentially in a segment with a callback */
status ci_sgm_iter_items( ci_sgm_t * s , seg_item_cb cb , pass_p ud )
{
	if ( !s || !cb ) return errArg;
	for ( size_t i = 0; i < s->itm_count; ++i )
	{
		buffer ptr = s->buf + s->offsets[ i ];
		size_t len = s->sizes[ i ];
		status rc = cb( ptr , len , ud );
		if ( rc != 0 ) return rc;
	}
	return errOK;
}

/* Destroy manager and free all segments. Caller must ensure no producers/consumers running. */
void segmgr_destroy( ci_sgmgr_t * mgr )
{
	if ( !mgr ) return;
	pthread_mutex_lock( &mgr->lock );
	/* free segments in ring */
	if ( mgr->ring )
	{
		ci_sgm_t * it = mgr->ring->next;
		while ( it != mgr->ring )
		{
			ci_sgm_t * next = it->next;
			ci_sgm_free( it );
			it = next;
		}
		ci_sgm_free( mgr->ring );
	}
	mgr->ring = mgr->active = NULL;
	/* free filled queue if any left (shouldn't be) */
	ci_sgm_t * q = mgr->filled_head;
	while ( q )
	{
		ci_sgm_t * n = q->next;
		/* q pointers were overridden for queue; safe to free? We already freed ring above. */
		/* But if ring was same segment, avoid double-free. */
		q = n;
	}
	pthread_mutex_unlock( &mgr->lock );
	pthread_mutex_destroy( &mgr->lock );
	pthread_cond_destroy( &mgr->filled_cond );
}
