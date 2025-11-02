#define Uses_LOCK_LINE
#define Uses_strncpy
#define Uses_MM_BREAK_IF
#define Uses_CALLOC_ONE
#define Uses_INIT_BREAKABLE_FXN
#define Uses_stat
#define Uses_snprintf
#define Uses_mmap
#define Uses_memset
#define Uses_open
#define Uses_errno
#define Uses_write
#define Uses_memmap
#define Uses_PagedStack
#include <general.dep>

/* ----------------- Config ----------------- */
#define MEMFILE_SIZE ((size_t)128 * 1024 * 1024) /* 64 MiB */
#define MEMFILE_HEADER_MAGIC 0x4D4D4654 /* "MMFT" */

#define METADATA_FILENAME "pg_stk_chain.meta"
#define HOTSPARE_NAME "pg_stk_hotspare"
#define TMPMETA_NAME "tmpmeta.XXXXXX"

#define MEMFILE_PAGEHEADER_SZ (sizeof(pg_stk_page_hdr_t))

/* ----------------- Utilities ----------------- */

/* safe write all */
_PRIVATE_FXN ssize_t pg_stk_write_all( int fd , const void_p buf , size_t sz )
{
	size_t off = 0;
	const char * p = buf;
	while ( off < sz )
	{
		ssize_t w = write( fd , p + off , sz - off );
		if ( w < 0 )
		{
			if ( errno == EINTR ) continue;
			return -1;
		}
		off += ( size_t )w;
	}
	return ( ssize_t )off;
}

/* atomic write+fsync+rename for metadata */
_PRIVATE_FXN int pg_stk_atomic_write_and_rename( const char * dir , const char * tmpname , const char * targetname , const void_p content , size_t len )
{
	char tmp[ MAX_PATH ];
	snprintf( tmp , sizeof( tmp ) , "%s/%s" , dir , tmpname );
	int fd = open( tmp , O_CREAT | O_WRONLY | O_TRUNC , 0644 );
	if ( fd < 0 ) return -1;
	if ( pg_stk_write_all( fd , content , len ) != ( ssize_t )len )
	{
		close( fd );
		unlink( tmp );
		return -1;
	}
	if ( fsync( fd ) != 0 )
	{
		close( fd ); unlink( tmp ); return -1;
	}
	if ( close( fd ) != 0 )
	{
		unlink( tmp ); return -1;
	}
	char tgt[ MAX_PATH ];
	snprintf( tgt , sizeof( tgt ) , "%s/%s" , dir , targetname );
	if ( rename( tmp , tgt ) != 0 )
	{
		unlink( tmp ); return -1;
	}
	/* sync dir to ensure rename persisted on some filesystems */
	int dfd = open( dir , O_DIRECTORY | O_RDONLY );
	if ( dfd >= 0 )
	{
		fsync( dfd );
		close( dfd );
	}
	return 0;
}

/* ----------------- Memfile helpers ----------------- */

/* create or open memfile path, ensure size, mmap it */
_PRIVATE_FXN status pg_stk_open_create( const char * path , size_t mem_size , int create_new , pg_stk_memfile_t ** out_mf )
{
	INIT_BREAKABLE_FXN();

	pg_stk_memfile_t * mf = CALLOC_ONE( mf );
	BREAK_IF( !mf , errMemoryLow , 0 );
	strncpy( mf->path , path , sizeof( mf->path ) - 1 );
	mf->memmap_size = mem_size;
	mf->decrease_time = 10 * 60; // 10 min
	//BREAK_IF( !pthread_mutex_init( &mf->lock , NULL ) , errResource , 1 );

	int flags = O_RDWR;
	if ( create_new ) flags |= O_CREAT;
	int fd = open( path , flags , 0644 );
	BREAK_IF( fd < 0 , errPath , 2 );
	
	/* ensure file size */
#ifdef __linux__
	/* try fallocate for speed if available (best-effort) */
	off_t cur = lseek( fd , 0 , SEEK_END );
	if ( ( size_t )cur < mem_size )
	{
		BREAK_IF( ftruncate( fd , ( __off_t )mem_size ) != 0 , errResource , 3 );
	}
#else
	#error // TOCHECK
	if ( ftruncate( fd , size ) != 0 )
	{
		close( fd ); free( mf ); return NULL;
	}
#endif

	void_p map = mmap( NULL , mem_size , PROT_READ | PROT_WRITE , MAP_SHARED , fd , 0 );
	BREAK_IF( map == MAP_FAILED , errResource , 3 );
	
	mf->fd = fd;
	mf->map = map;
	mf->hdr = ( pg_stk_page_hdr_t * )map; // also due time set in this part

	/* initialize header if empty */
	if ( mf->hdr->magic != MEMFILE_HEADER_MAGIC )
	{
		/* consider this file fresh: zero then write header */
		memset( mf->map , 0 , mf->memmap_size );
		mf->hdr->magic = MEMFILE_HEADER_MAGIC;
		//mf->hdr->LIFO_due = time(NULL); // add when be applicable and in minHeap . it is come from minHeap

		vstack_init( &mf->hdr->stack , ( ( uint8 * )mf->map ) + MEMFILE_PAGEHEADER_SZ , mem_size - MEMFILE_PAGEHEADER_SZ , true ); // create stack on the buf

		/* persist header */
		msync( mf->map , MEMFILE_PAGEHEADER_SZ , MS_SYNC );
		fsync( mf->fd );
	}
	else
	{
		vstack_init( &mf->hdr->stack , ( ( uint8 * )mf->map ) + MEMFILE_PAGEHEADER_SZ , mem_size - MEMFILE_PAGEHEADER_SZ , false ); // create stack on the buf
	}
	if ( out_mf ) *out_mf = mf;
	
	BEGIN_RET
	case 3:
	{
		close( fd );
	}
	case 2:
	{
		//pthread_mutex_destroy( &mf->lock );
	}
	case 1:
	{
		FREE( mf );
	}
	N_END_RET
}

/* close memfile */
_PRIVATE_FXN void pg_stk_close( pg_stk_memfile_t * mf )
{
	if ( !mf ) return;
	vstack_destroy( &mf->hdr->stack );
	msync( mf->map , mf->memmap_size , MS_SYNC );
	fsync( mf->fd );
	munmap( mf->map , mf->memmap_size );
	close( mf->fd );
	//pthread_mutex_destroy( &mf->lock );
	// FREE( mf );
}

_PRIVATE_FXN int compare_pg_stk_memfile( const void * a , const void * b )
{
	pg_stk_memfile_t * arg1 = *( pg_stk_memfile_t ** )a;
	pg_stk_memfile_t * arg2 = *( pg_stk_memfile_t ** )b;

	if ( arg1->hdr->due_time < arg2->hdr->due_time ) return 1; // less value means low priority
	if ( arg1->hdr->due_time > arg2->hdr->due_time ) return -1; // more value means higher order
	return 0;
}

/* write a record to memfile; returns seq number or 0 on failure */
_PRIVATE_FXN status pg_stk_append_record( page_stack_t * mm , pg_stk_memfile_t * mf , const void_p buf , size_t len )
{
	if ( !mf->nocked_up ) // if memfile not touched by any record then sort again based on due time
	{
		mf->nocked_up = true;
		mf->hdr->due_time = time(NULL);
		if ( mm->files.count > 1 )
		{
			qsort( mm->files.data , mm->files.count , sizeof( void * ) , compare_pg_stk_memfile );
		}
	}

	tchs touches;
	status err_ret = vstack_push( &mf->hdr->stack , buf , len , &touches );
	switch ( err_ret )
	{
		case errOK:
		{
			//msync( touches[ 0 ].addr , touches[ 0 ].sz , MS_SYNC );
			msync( touches[ 1 ].addr , touches[ 1 ].sz , MS_SYNC );
			//fsync( mf->fd );
			break;
		}
		case errOverflow:
		{
			if ( !mf->to_be_absolete )
			{
				msync( mf->map , mf->memmap_size , MS_SYNC );
				fsync( mf->fd );
			}
			break;
		}
	}
	return err_ret;
}

/* ----------------- Manager logic ----------------- */

_PRIVATE_FXN status pg_stk_create( page_stack_t * mm , const char * base_dir )
{
	INIT_BREAKABLE_FXN();
	MEMSET_ZERO_O( mm );
	strncpy( mm->base_dir , base_dir , sizeof( mm->base_dir ) - 1 );
	
	BREAK_STAT( mms_array_init( &mm->files , sizeof( pg_stk_memfile_t ) , 1 , GROW_STEP , 0 ) , 0 );

	//pthread_mutexattr_t attr;
	//pthread_mutexattr_init( &attr );
	//pthread_mutexattr_settype( &attr , PTHREAD_MUTEX_RECURSIVE ); // make it double locked
	pthread_mutex_init( &mm->ps_lock , NULL );

	mm->current = NULL;
	mm->hot_spare = NULL;
	/* ensure metadata file exists or create empty */
	
	BEGIN_SMPL
	N_END_RET
}

/* build file path helper */
_PRIVATE_FXN void pg_stk_fill_path( char * out , size_t outlen , const char * dir , const char * name )
{
	snprintf( out , outlen , "%s/%s" , dir , name );
}

/* persist metadata chain: list of filenames in order, one per line */
_PRIVATE_FXN int pg_stk_persist_chain( page_stack_t * mm )
{
	/* build content */
	size_t cap = 4096;
	char * buf = MALLOC( cap );
	size_t used = 0;
	for ( size_t i = 0; i < mm->files.count; i++ )
	{
		pg_stk_memfile_t * pfile = NULL;
		if ( mms_array_get_s( &mm->files , i , ( void ** )&pfile ) == errOK && !pfile->to_be_absolete )
		{
			const char * p = pfile->path;
			size_t need = strlen( p ) + 2;
			if ( used + need > cap )
			{
				cap *= 2;
				buf = REALLOC( buf , cap );
			}
			memcpy( buf + used , p , strlen( p ) );
			used += strlen( p );
			buf[ used++ ] = '\n';
		}
	}
	if ( used == 0 )
	{
		FREE( buf );
		return 0;
	}
	/* atomic write */
	int r = pg_stk_atomic_write_and_rename( mm->base_dir , TMPMETA_NAME , METADATA_FILENAME , buf , used );
	FREE( buf );
	return r;
}

/* load chain from metadata file (if present). For simplicity we'll ignore validation errors and treat as best-effort. */
_PRIVATE_FXN status pg_stk_load_chain( page_stack_t * mm )
{
	INIT_BREAKABLE_FXN();

	char meta_path[ MAX_PATH ];
	pg_stk_fill_path( meta_path , sizeof( meta_path ) , mm->base_dir , METADATA_FILENAME );
	FILE * f = fopen( meta_path , "r" );
	BREAK_IF( !f , errNotFound , 0 );
	char line[ MAX_PATH ];
	while ( fgets( line , sizeof( line ) , f ) )
	{
		/* trim newline */
		size_t L = strlen( line );
		if ( L && ( line[ L - 1 ] == '\n' || line[ L - 1 ] == '\r' ) ) line[ --L ] = '\0';
		pg_stk_memfile_t * mf = NULL;
		if ( pg_stk_open_create( line , MEMFILE_SIZE , 0 , &mf ) != errOK && !mf ) continue;
		if ( !mf->hdr->due_time ) mf->hdr->due_time = time(NULL);
		mf->nocked_up = true; // any record in it so it is not new

		pg_stk_memfile_t ** ppfile = NULL;
		if ( mms_array_get_one_available_unoccopied_item_holder( &mm->files , ( void *** )&ppfile ) == errOK )
		{
			*ppfile = mf;
		}
	}
	fclose( f );

	if ( mm->files.count > 1 )
	{
		qsort( mm->files.data , mm->files.count , sizeof( void * ) , compare_pg_stk_memfile );
	}
	mms_array_get_s( &mm->files , mm->files.count - 1 , ( void ** )&mm->current );
	
	BEGIN_RET
	case 1:
	{
		fclose( f );
	}
	N_END_RET
}

/* ensure there is a hot spare (pessimistically allocate) */
_PRIVATE_FXN status pg_stk_ensure_hot_spare( page_stack_t * mm )
{
	//pthread_mutex_lock( &mm->lock );
	if ( mm->hot_spare )
	{
		//pthread_mutex_unlock( &mm->lock );
		return errOK;
	}
	//size_t idx = mm->nfiles + 1;
	char path[ MAX_PATH ];
	char name[ 128 ];
	snprintf( name , sizeof( name ) , "%s/%s_%zu" , mm->base_dir , HOTSPARE_NAME , ( unsigned )time( NULL ) );
	/* choose deterministic path */
	pg_stk_fill_path( path , sizeof( path ) , mm->base_dir , HOTSPARE_NAME );
	/* create hot spare file path = base_dir/HOTSPARE_NAME */
	pg_stk_memfile_t * mf = NULL;
	status d_error = errOK;
	if ( ( d_error = pg_stk_open_create( path , MEMFILE_SIZE , 1 , &mf ) ) )
	{
		//pthread_mutex_unlock( &mm->lock );
		return d_error;
	}
	mm->hot_spare = mf;
	//pthread_mutex_unlock( &mm->lock );
	return d_error;
}

/* rotate in hot spare as active memfile; create new hot spare after rotation */
_PRIVATE_FXN status pg_stk_activate_hot_spare( page_stack_t * mm )
{
	INIT_BREAKABLE_FXN();
	//pthread_mutex_lock( &mm->lock );
	if ( !mm->hot_spare )
	{
		//pthread_mutex_unlock( &mm->lock );
		return errNotFound;
	}
	/* rename hot_spare file to canonical memfile_N name */
	char newpath[ MAX_PATH ];
	char newname[ 128 ];
	time_t tnow = time( NULL );
	snprintf( newname , sizeof( newname ) , "pg_stk_%zu_%zu.dat" , ( unsigned )tnow , mm->file_name_counter++ ); // add time to name
	pg_stk_fill_path( newpath , sizeof( newpath ) , mm->base_dir , newname );
	/* atomic rename */
	if ( !rename( mm->hot_spare->path , newpath ) )
	{
		#pragma GCC diagnostic ignored "-Wstringop-truncation"
		strncpy( mm->hot_spare->path , newpath , MAX_PATH - 1 );
		#pragma GCC diagnostic pop
	}
	
	// TODO . get free space and fill it . and does not make arr too long
	pg_stk_memfile_t ** ppfile = NULL;
	if ( ( d_error = mms_array_get_one_available_unoccopied_item_holder( &mm->files , ( void *** )&ppfile ) ) == errOK )
	{
		*ppfile = mm->hot_spare;

		( *ppfile )->hdr->due_time = /*tnow*/0; // first it initialized to zero then when one item add to it it set to now because when hot spare placed at index 0 then it means the end of heap arrived and no more delete is valid

		/* persist header */
		msync( ( *ppfile )->map , MEMFILE_PAGEHEADER_SZ , MS_SYNC );
		fsync( ( *ppfile )->fd );
	}
	else
	{
		return d_error;
	}
	if ( mm->files.count > 1 )
	{
		qsort( mm->files.data , mm->files.count , sizeof( void * ) , compare_pg_stk_memfile );
	}
	mm->current = mm->hot_spare;
	mm->hot_spare = NULL;
	/* persist chain metadata */
	pg_stk_persist_chain( mm );
	/* create new hot spare in background (pessimistically) */
	//pthread_mutex_unlock( &mm->lock );
	pg_stk_ensure_hot_spare( mm );

	BREAK( errOK , 0 );
	
	BEGIN_RET
	N_END_RET
}

////////////////////////////////////////

/* expose API: init manager (create base dir if needed), load existing chain, ensure hot spare */
_PUB_FXN status pg_stk_init( page_stack_t * mm , LPCSTR base_dir , void_p custom_data )
{
	INIT_BREAKABLE_FXN();
	/* create dir if not exists */
	struct stat st;
	if ( stat( base_dir , &st ) != 0 )
	{
		BREAK_IF( mkdir( base_dir , 0755 ) != 0 , errNotFound , 0 );
	}
	BREAK_STAT( pg_stk_create( mm , base_dir ) , 0 );
	mm->custom_data = custom_data;
	d_error = pg_stk_load_chain( mm );
	if ( d_error != errNotFound ) BREAK_STAT( d_error , 0 );
	BREAK_STAT( pg_stk_ensure_hot_spare( mm ) , 0 );
	/* If no current active file, activate the hot spare */
	if ( !mm->current )
	{
		pg_stk_activate_hot_spare( mm );
	}

	BEGIN_SMPL
	N_END_RET
}

/* store buffer into manager: returns seq (non-zero) on success, 0 on failure */
_PUB_FXN status pg_stk_store( page_stack_t * mm , const void_p buf , size_t len )
{
	if ( !mm || !buf || !len ) return errArg;
	LOCK_LINE( pthread_mutex_lock( &mm->ps_lock ) );
	pg_stk_memfile_t * cur = mm->current;
	status d_error = errOK;
	if ( !cur || cur->to_be_absolete )
	{
		if ( ( d_error = pg_stk_activate_hot_spare( mm ) ) == errOK ) // i added this line to make retry possible
		{
			pthread_mutex_unlock( &mm->ps_lock );
			pg_stk_store( mm , buf , len );
			return errRetry;
		}
		pthread_mutex_unlock( &mm->ps_lock );
		return d_error;
	}
	/* try append */
	if ( ( d_error = pg_stk_append_record( mm , cur , buf , len ) ) == errOK )
	{
		// GOOD
		mm->item_stored++;
		mm->item_stored_byte += len;
		pthread_mutex_unlock( &mm->ps_lock );
		return d_error;
	}
	/* not enough space: seal current, activate hot spare, then append to new current */
	//pg_stk_seal( cur );
	pg_stk_activate_hot_spare( mm );
	if ( !mm->current )
	{
		pthread_mutex_unlock( &mm->ps_lock );
		return errMemoryLow;
	}
	/* retry append again */
	if ( ( d_error = pg_stk_append_record( mm , mm->current , buf , len ) ) == errOK )
	{
		mm->item_stored++;
		mm->item_stored_byte += len;
	}
	pthread_mutex_unlock( &mm->ps_lock );
	return d_error;
}

/* pop latest record from manager chain (LIFO across files) - copies into user buffer */
// Call by packet mngr to retrieve ready packet
// can use callback to continue send data or check getter buffer that is ready to get data( with empty arg )
_PUB_FXN status pg_stk_try_to_pop_latest( page_stack_t * mm , ps_callback_data data_getter )
{
	INIT_BREAKABLE_FXN();

	LOCK_LINE( pthread_mutex_lock( &mm->ps_lock ) );
	
	void_p out_item;
	size_t out_sz;
	bool bcontinue_stack = true , bcontinue_heap = true;
	status ret_heap , ret_stack;
	pgstk_cmd ret_stack_cmd;
	pg_stk_memfile_t * pmemfile;

	do
	{
		ret_heap = mms_array_get_s( &mm->files , 0 , (void**)&pmemfile );
		switch ( ret_heap )
		{
			case errEmpty: BREAK( ret_heap , 0 );
			case errOK: { break; /*continue*/ }
			default: BREAK( errNotFound , 0 );
		}
		bcontinue_stack = true;

		if ( pmemfile->to_be_absolete )
		{
			pg_stk_persist_chain( mm );
			if ( !remove( pmemfile->path ) ) // remove file
			{
				pg_stk_close( pmemfile ); // close memmap
				if ( pmemfile == mm->current ) // if current is absolete then null
				{
					mm->current = NULL;
				}
				ret_heap = mms_array_delete( &mm->files , 0 );
			}
			else
			{
				ret_heap = errNotFound;
			}
			bcontinue_stack = false; // stack is empty so pop it and get next based on order
			if ( !mm->files.count )
			{
				pg_stk_activate_hot_spare( mm );
				BREAK( errEmpty , 0 );
			}
			bcontinue_heap = true; // after if ( !mm->files.count ) it means there is item in it
			continue;
		}

		do
		{
			ret_stack = vstack_peek( &pmemfile->hdr->stack , &out_item , &out_sz );
			switch ( ret_stack )
			{
				case errOK: // if there is packet in stack
				{
					ret_stack_cmd = data_getter( mm->custom_data , out_item , out_sz );
					switch( ret_stack_cmd )
					{
						case pgstk_not_send__stop_sending:
						{
							bcontinue_stack = bcontinue_heap = false;
							break;
						}
						case pgstk_not_send__continue_sending: // this segment has Head-of-line blocking
						{
							// try prev memmap and continue but not close current
							if ( pmemfile->hdr->due_time > pmemfile->decrease_time ) // if any time left decrease
							{
								pmemfile->hdr->due_time -= pmemfile->decrease_time; // it means it gain lower priority later
								if ( mm->files.count > 1 )
								{
									qsort( mm->files.data , mm->files.count , sizeof( void * ) , compare_pg_stk_memfile );
								}
							}
							pmemfile->decrease_time *= 2;

							bcontinue_stack = false;
							bcontinue_heap = true;
							break;
						}
						case pgstk_not_send__continue_sending_with_delay: // this segment has Head-of-line blocking
						{
							BREAK( errTooManyAttempt , 0 );
						}
						case pgstk_not_send__not_any_peer:
						{
							BREAK( errTooManyAttempt , 0 ); // wait for another time then retry sending
						}
						case pgstk_sended__continue_sending:
						{
							if ( mm->item_stored > 0 )
							{
								mm->item_stored--;
							}

							if ( mm->item_stored_byte >= out_sz )
							{
								mm->item_stored_byte -= out_sz;
							}
							else if ( mm->item_stored_byte > 0 )
							{
								mm->item_stored_byte = 0;
							}

							bool emptied = false;
							vstack_pop( &pmemfile->hdr->stack , NULL , NULL , &emptied ); // pop packet from stack
							bcontinue_stack = bcontinue_heap = true;
							if ( emptied )
							{
								bcontinue_stack = false;
								pmemfile->to_be_absolete = 1; // sended means it is full then be emptied . and this memmap is nocketup
							}
							break;
						}
						case pgstk_sended__stop_sending:
						{
							if ( mm->item_stored > 0 )
							{
								mm->item_stored--;
							}
							if ( mm->item_stored_byte >= out_sz )
							{
								mm->item_stored_byte -= out_sz;
							}
							else if ( mm->item_stored_byte > 0 )
							{
								mm->item_stored_byte = 0;
							}

							bool emptied = false;
							vstack_pop( &pmemfile->hdr->stack , NULL , NULL , &emptied ); // pop stack
							bcontinue_stack = false;
							bcontinue_heap = false;
							if ( emptied )
							{
								pmemfile->to_be_absolete = 1; // sended means it is full then be emptied . and this memmap is nocketup
							}
							break;
						}
					}
					break;
				}
				case errEmpty: // stack is empty
				{
					// try prev memmap and continue and closr current
					if ( pmemfile->nocked_up )
					{
						pmemfile->to_be_absolete = true;
					}
					else
					{
						if ( !pmemfile->hdr->due_time )
						{
							BREAK( errEmpty , 0 );
						}
					}
					bcontinue_stack = false; // stack is empty so pop it and get next based on order
					bcontinue_heap = ( mm->files.count > 0 ); // later poor guy most deal with broken heap
					if ( !mm->files.count ) BREAK( errEmpty , 0 );
					break;
				}
				default: // this place means vstack_peek failed but not empty so ignore this one and try another one
				{
					// try prev memmap and continue but not close current
					if ( pmemfile->hdr->due_time > pmemfile->decrease_time )
					{
						pmemfile->hdr->due_time -= pmemfile->decrease_time; // it means it gain lower priority later
						if ( mm->files.count > 1 )
						{
							qsort( mm->files.data , mm->files.count , sizeof( void * ) , compare_pg_stk_memfile );
						}
					}
					pmemfile->decrease_time *= 2;
					bcontinue_stack = false; // this memmap should proc later
					bcontinue_heap = true;  // but other memmap could be resumable
					break;
				}
			}
		}
		while( bcontinue_stack );
	}
	while( bcontinue_heap );
	
	// do not return without considering unlock mutex

	BEGIN_SMPL
	N_V_END_RET

	( pthread_mutex_unlock( &mm->ps_lock ) );
	return d_error;
}

/* shutdown manager, close files, persist metadata */
void pg_stk_shutdown( page_stack_t * mm )
{
	if ( !mm ) return;
	pthread_mutex_lock( &mm->ps_lock );
	pg_stk_persist_chain( mm );
	for ( size_t i = 0; i < mm->files.count; i++ )
	{
		pg_stk_memfile_t * pfile = NULL;
		if ( mms_array_get_s( &mm->files , i , ( void ** )&pfile ) == errOK )
		{
			pg_stk_close( pfile );
			if ( pfile == mm->current )
			{
				mm->current = NULL;
			}
		}
	}
	if ( mm->hot_spare ) pg_stk_close( mm->hot_spare );
	mms_array_free( &mm->files );
	( pthread_mutex_unlock( &mm->ps_lock ) );
	pthread_mutex_destroy( &mm->ps_lock );
	MEMSET_ZERO_O( mm );
}
