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
#define MEMFILE_SIZE ((size_t)64 * 1024 * 1024) /* 64 MiB */
#define MEMFILE_HEADER_MAGIC 0x4D4D4654 /* "MMFT" */
//#define MEMFILE_VERSION 1

#define METADATA_FILENAME "pg_stk_chain.meta"
#define HOTSPARE_NAME "pg_stk_hotspare"

#define MEMFILE_HEADER_SIZE (sizeof(pg_stk_page_hdr_t))
//#define REC_HEADER_SIZE (sizeof(pg_stk_rec_header_t))
//#define REC_FOOTER_SIZE (sizeof(uint32_t))
//#define REC_MAGIC 0x52454331 /* "REC1" */
//#define REC_VERSION 1

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
_PRIVATE_FXN status pg_stk_open_create( const char * path , size_t size , int create_new , pg_stk_memfile_t ** out_mf )
{
	INIT_BREAKABLE_FXN();

	pg_stk_memfile_t * mf = CALLOC_ONE( mf );
	BREAK_IF( !mf , errMemoryLow , 0 );
	strncpy( mf->path , path , sizeof( mf->path ) - 1 );
	mf->size = size;
	mf->decrease_time = 10 * 60;
	//BREAK_IF( !pthread_mutex_init( &mf->lock , NULL ) , errResource , 1 );

	int flags = O_RDWR;
	if ( create_new ) flags |= O_CREAT;
	int fd = open( path , flags , 0644 );
	BREAK_IF( fd < 0 , errPath , 2 );
	
	/* ensure file size */
#ifdef __linux__
	/* try fallocate for speed if available (best-effort) */
	off_t cur = lseek( fd , 0 , SEEK_END );
	if ( ( size_t )cur < size )
	{
		BREAK_IF( ftruncate( fd , ( __off_t )size ) != 0 , errResource , 3 );
	}
#else
	#error // TOCHECK
	if ( ftruncate( fd , size ) != 0 )
	{
		close( fd ); free( mf ); return NULL;
	}
#endif

	void_p map = mmap( NULL , size , PROT_READ | PROT_WRITE , MAP_SHARED , fd , 0 );
	BREAK_IF( map == MAP_FAILED , errResource , 3 );
	
	mf->fd = fd;
	mf->map = map;
	mf->hdr = ( pg_stk_page_hdr_t * )map;

	/* initialize header if empty */
	if ( mf->hdr->magic != MEMFILE_HEADER_MAGIC )
	{
		/* consider this file fresh: zero then write header */
		memset( mf->map , 0 , mf->size );
		mf->hdr->magic = MEMFILE_HEADER_MAGIC;
		//mf->hdr->version = MEMFILE_VERSION;
		//mf->hdr->write_offset = MEMFILE_HEADER_SIZE;
		//mf->hdr->seq_counter = 1;
		//mf->hdr->LIFO_due = time(NULL); // add when be applicable and in minHeap . it is come from minHeap

		vstack_init( &mf->hdr->stack , ( ( uint8 * )mf->map ) + MEMFILE_HEADER_SIZE , size - MEMFILE_HEADER_SIZE ); // create stack on the buf

		/* persist header */
		msync( mf->map , MEMFILE_HEADER_SIZE , MS_SYNC );
		fsync( mf->fd );
	}
	else
	{
		// TODO . for now
		pthread_mutex_init( &mf->hdr->stack.lock , NULL );
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
		free( mf );
	}
	N_END_RET
}

/* sync header to disk */
_PRIVATE_FXN int pg_stk_sync_header( pg_stk_memfile_t * mf )
{
	if ( !mf ) return -1;
	if ( msync( mf->map , MEMFILE_HEADER_SIZE , MS_SYNC ) != 0 ) return -1;
	if ( fsync( mf->fd ) != 0 ) return -1;
	return 0;
}

/* close memfile */
_PRIVATE_FXN void pg_stk_close( pg_stk_memfile_t * mf )
{
	if ( !mf ) return;
	munmap( mf->map , mf->size );
	close( mf->fd );
	vstack_destroy( &mf->hdr->stack );
	//pthread_mutex_destroy( &mf->lock );
	free( mf );
}

/* write a record to memfile; returns seq number or 0 on failure */
_PRIVATE_FXN status pg_stk_append_record( pg_stk_memfile_t * mf , const void_p buf , size_t len )
{
	return vstack_push( &mf->hdr->stack , buf , len );
}

/* pop last record: copies into buf (up to buf_len). returns size written or -1 on empty/error.
   Caller must free returned pointer if ret > 0 and pointer is returned. Here we copy to user buffer. */
//_PRIVATE_FXN ssize_t pg_stk_pop_last( pg_stk_memfile_t * mf , void * buf , size_t buf_len , uint64_t * out_seq )
//{
//	pthread_mutex_lock( &mf->lock );
//	uint64_t write_off = mf->hdr->write_offset;
//	if ( write_off <= MEMFILE_HEADER_SIZE )
//	{
//		pthread_mutex_unlock( &mf->lock );
//		return -1; /* empty */
//	}
//	size_t pos = ( size_t )write_off;
//	/* read footer */
//	if ( pos < REC_FOOTER_SIZE + REC_HEADER_SIZE )
//	{
//		pthread_mutex_unlock( &mf->lock );
//		return -1;
//	}
//	uint32_t footer_val;
//	memcpy( &footer_val , ( char * )mf->map + ( pos - REC_FOOTER_SIZE ) , REC_FOOTER_SIZE );
//	uint32_t data_len = footer_val;
//	if ( pos < REC_FOOTER_SIZE + data_len + REC_HEADER_SIZE )
//	{
//		pthread_mutex_unlock( &mf->lock );
//		return -1;
//	}
//	size_t rec_start = pos - REC_FOOTER_SIZE - data_len - REC_HEADER_SIZE;
//	pg_stk_rec_header_t rh;
//	memcpy( &rh , ( char * )mf->map + rec_start , REC_HEADER_SIZE );
//	if ( rh.rec_magic != REC_MAGIC || rh.rec_version != REC_VERSION || rh.data_len != data_len )
//	{
//		pthread_mutex_unlock( &mf->lock );
//		return -1;
//	}
//	/* copy data */
//	size_t tocopy = ( data_len < buf_len ) ? data_len : buf_len;
//	memcpy( buf , ( char * )mf->map + rec_start + REC_HEADER_SIZE , tocopy );
//	/* truncate file logically by moving write_offset back */
//	mf->hdr->write_offset = rec_start;
//	memfile_sync_header( mf );
//	if ( out_seq ) *out_seq = rh.seq;
//	pthread_mutex_unlock( &mf->lock );
//	return ( ssize_t )tocopy;
//}

/* ----------------- Manager logic ----------------- */

_PRIVATE_FXN status pg_stk_create( page_stack_t * mm , const char * base_dir )
{
	INIT_BREAKABLE_FXN();
	MEMSET_ZERO_O( mm );
	strncpy( mm->base_dir , base_dir , sizeof( mm->base_dir ) - 1 );
	
	BREAK_STAT( mms_array_init( &mm->files , sizeof( pg_stk_memfile_t ) , 1 , 10 , 0 ) , 0 );
	BREAK_STAT( mh_create( &mm->files_order , 1 , 10 , HEAP_MAX ) , 0 );

	pthread_mutex_init( &mm->lock , NULL );
	mm->current = NULL;
	mm->hot_spare = NULL;
	/* ensure metadata file exists or create empty */
	
	BEGIN_SMPL
	N_END_RET
}

/* build file path helper */
_PRIVATE_FXN void pg_stk_build_path( char * out , size_t outlen , const char * dir , const char * name )
{
	snprintf( out , outlen , "%s/%s" , dir , name );
}

/* persist metadata chain: list of filenames in order, one per line */
_PRIVATE_FXN int pg_stk_persist_chain( page_stack_t * mm )
{
	/* build content */
	size_t cap = 4096;
	char * buf = malloc( cap );
	size_t used = 0;
	for ( size_t i = 0; i < mm->files.count; i++ )
	{
		pg_stk_memfile_t * pfile = NULL;
		if ( mms_array_get_s( &mm->files , i , ( void ** )&pfile ) == errOK )
		{
			const char * p = pfile->path;
			size_t need = strlen( p ) + 2;
			if ( used + need > cap )
			{
				cap *= 2;
				buf = realloc( buf , cap );
			}
			memcpy( buf + used , p , strlen( p ) );
			used += strlen( p );
			buf[ used++ ] = '\n';
		}
	}
	if ( used == 0 )
	{
		free( buf );
		return 0;
	}
	/* atomic write */
	int r = pg_stk_atomic_write_and_rename( mm->base_dir , "tmpmeta.XXXXXX" , METADATA_FILENAME , buf , used );
	free( buf );
	return r;
}

/* load chain from metadata file (if present). For simplicity we'll ignore validation errors and treat as best-effort. */
_PRIVATE_FXN status pg_stk_load_chain( page_stack_t * mm )
{
	INIT_BREAKABLE_FXN();

	char meta_path[ MAX_PATH ];
	pg_stk_build_path( meta_path , sizeof( meta_path ) , mm->base_dir , METADATA_FILENAME );
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
		
		// TODO . add time to it
		
		pg_stk_memfile_t ** ppfile = NULL;
		if ( mms_array_get_one_available_unoccopied_item_holder( &mm->files , ( void *** )&ppfile ) == errOK )
		{
			*ppfile = mf;

			mh_HeapNode * porder_node = NULL;
			BREAK_STAT( mh_insert( &mm->files_order , time(NULL) , mf , &porder_node ) , 0 );
			mf->hdr->LIFO_due = ( time_t * )&porder_node->key;
		}
	}
	fclose( f );
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
	pthread_mutex_lock( &mm->lock );
	if ( mm->hot_spare )
	{
		pthread_mutex_unlock( &mm->lock );
		return errOK;
	}
	//size_t idx = mm->nfiles + 1;
	char path[ MAX_PATH ];
	char name[ 128 ];
	snprintf( name , sizeof( name ) , "%s/%s_%zu" , mm->base_dir , HOTSPARE_NAME , ( unsigned )time( NULL ) );
	/* choose deterministic path */
	pg_stk_build_path( path , sizeof( path ) , mm->base_dir , HOTSPARE_NAME );
	/* create hot spare file path = base_dir/HOTSPARE_NAME */
	pg_stk_memfile_t * mf = NULL;
	status d_error = errOK;
	if ( ( d_error = pg_stk_open_create( path , MEMFILE_SIZE , 1 , &mf ) ) )
	{
		pthread_mutex_unlock( &mm->lock );
		return d_error;
	}
	mm->hot_spare = mf;
	pthread_mutex_unlock( &mm->lock );
	return d_error;
}

/* rotate in hot spare as active memfile; create new hot spare after rotation */
_PRIVATE_FXN status pg_stk_activate_hot_spare( page_stack_t * mm )
{
	INIT_BREAKABLE_FXN();
	pthread_mutex_lock( &mm->lock );
	if ( !mm->hot_spare )
	{
		pthread_mutex_unlock( &mm->lock );
		return errNotFound;
	}
	/* rename hot_spare file to canonical memfile_N name */
	char newpath[ MAX_PATH ];
	char newname[ 128 ];
	snprintf( newname , sizeof( newname ) , "pg_stk_%zu.dat" , ( unsigned )time( NULL ) ); // add time to name
	pg_stk_build_path( newpath , sizeof( newpath ) , mm->base_dir , newname );
	/* atomic rename */
	if ( !rename( mm->hot_spare->path , newpath ) )
	{
		strncpy( mm->hot_spare->path , newpath , sizeof( mm->hot_spare->path ) - 1 );
	}
	
	// TODO . get free space and fill it . and does not make arr too long
	pg_stk_memfile_t ** ppfile = NULL;
	if ( ( d_error = mms_array_get_one_available_unoccopied_item_holder( &mm->files , ( void *** )&ppfile ) ) == errOK )
	{
		*ppfile = mm->hot_spare;

		mh_HeapNode * porder_node = NULL;
		BREAK_STAT( mh_insert( &mm->files_order , time( NULL ) , *ppfile , &porder_node ) , 0 );
		
		(*ppfile)->hdr->LIFO_due = ( time_t * )&porder_node->key;
		*mm->hot_spare->hdr->LIFO_due = time( NULL );
	}
	else
	{
		return d_error;
	}
	mm->current = mm->hot_spare;
	mm->hot_spare = NULL;
	/* persist chain metadata */
	pg_stk_persist_chain( mm );
	/* create new hot spare in background (pessimistically) */
	pthread_mutex_unlock( &mm->lock );
	pg_stk_ensure_hot_spare( mm );
	
	BEGIN_RET
	N_END_RET
}

////////////////////////////////////////

/* expose API: init manager (create base dir if needed), load existing chain, ensure hot spare */
status pg_stk_init( page_stack_t * mm , LPCSTR base_dir , void_p custom_data )
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
status pg_stk_store( page_stack_t * mm , const void_p buf , size_t len )
{
	if ( !mm || !buf || !len ) return errArg;
	pthread_mutex_lock( &mm->lock );
	pg_stk_memfile_t * cur = mm->current;
	status d_error = errOK;
	if ( !cur )
	{
		pthread_mutex_unlock( &mm->lock );
		if ( ( d_error = pg_stk_activate_hot_spare( mm ) ) == errOK ) // i added this line to make retry possible
		{
			pg_stk_store( mm , buf , len );
			return errRetry;
		}
		return d_error;
	}
	/* try append */
	if ( ( d_error = pg_stk_append_record( cur , buf , len ) ) == errOK )
	{
		// GOOD
		pthread_mutex_unlock( &mm->lock );
		return d_error;
	}
	/* not enough space: seal current, activate hot spare, then append to new current */
	//pg_stk_seal( cur );
	pg_stk_activate_hot_spare( mm );
	pg_stk_memfile_t * newcur = mm->current;
	if ( !newcur )
	{
		pthread_mutex_unlock( &mm->lock );
		return errMemoryLow;
	}
	/* try append again */
	d_error = pg_stk_append_record( newcur , buf , len );
	pthread_mutex_unlock( &mm->lock );
	return d_error;
}

/* pop latest record from manager chain (LIFO across files) - copies into user buffer */
// Call by packet mngr to retrieve ready packet
// can use callback to continue send data or check getter buffer that is ready to get data( with empty arg )
status pg_stk_try_to_pop_latest( page_stack_t * mm , ps_callback_data data_getter )
{
	INIT_BREAKABLE_FXN();

	pthread_mutex_lock( &mm->lock );
	
	mh_HeapNode * pnode = NULL;
	void_p out_item;
	size_t out_sz;
	bool bcontinue_stack = true , bcontinue_heap = true;
	//bool bdelayed = false;
	status ret_heap , ret_stack;
	pgstk_cmd ret_stack_cmd;

	do
	{
		ret_heap = mh_min( &mm->files_order , &pnode ); // it means max. min or max define at init time
		switch ( ret_heap )
		{
			case errEmpty: BREAK( ret_heap , 0 );
			case errOK: { break; } // continue
			default: BREAK( errNotFound , 0 );
		}
		pg_stk_memfile_t * pmemfile = ( ( pg_stk_memfile_t * )pnode->data_key );

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
							bcontinue_stack = false;
							bcontinue_heap = true;
							break;
						}
						case pgstk_not_send__continue_sending_with_delay: // this segment has Head-of-line blocking
						{
							BREAK( errTooManyAttempt , 0 );
						}
						case pgstk_sended__continue_sending:
						{
							vstack_pop( &pmemfile->hdr->stack , NULL , NULL ); // pop stack
							bcontinue_stack = bcontinue_heap = true;
							break;
						}
						case pgstk_sended__stop_sending:
						{
							vstack_pop( &pmemfile->hdr->stack , NULL , NULL ); // pop stack
							bcontinue_stack = false;
							bcontinue_heap = false;
							break;
						}
					}
					break;
				}
				case errEmpty: // stack is empty
				{
					// try prev memmap and continue and closr current
					ret_heap = mh_extract_min( &mm->files_order , NULL /*because we use memory from another list and this heap just sort them*/ , 1 /*at least one page most be kept*/ );
					bcontinue_stack = false; // stack is empty so pop it and get next based on order
					bcontinue_heap = ( ret_heap == errOK ); // later poor guy most deal with broken heap
					if ( ret_heap == errEmpty ) BREAK( ret_heap , 0 );
					break;
				}
				default:
				{
					// try prev memmap and continue but not close current
					if ( pmemfile->hdr->LIFO_due )
					{
						*pmemfile->hdr->LIFO_due -= pmemfile->decrease_time; // it means it gain lower priority later
						mh_heap_refresh( &mm->files_order , pmemfile );
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

	pthread_mutex_unlock( &mm->lock );
	return d_error;
}

/* shutdown manager, close files, persist metadata */
void pg_stk_shutdown( page_stack_t * mm )
{
	if ( !mm ) return;
	pthread_mutex_lock( &mm->lock );
	pg_stk_persist_chain( mm );
	for ( size_t i = 0; i < mm->files.count; i++ )
	{
		pg_stk_memfile_t * pfile = NULL;
		if ( mms_array_get_s( &mm->files , i , ( void ** )&pfile ) == errOK )
		{
			pg_stk_close( pfile );
		}
	}
	if ( mm->hot_spare ) pg_stk_close( mm->hot_spare );
	mms_array_free( &mm->files );
	pthread_mutex_unlock( &mm->lock );
	pthread_mutex_destroy( &mm->lock );
}
