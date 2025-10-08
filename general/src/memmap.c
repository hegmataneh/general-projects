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
#include <general.dep>

/* ----------------- Config ----------------- */
#define MEMFILE_SIZE ((size_t)64 * 1024 * 1024) /* 64 MiB */
#define MEMFILE_HEADER_MAGIC 0x4D4D4654 /* "MMFT" */
#define MEMFILE_VERSION 1

#define METADATA_FILENAME "mm_chain.meta"
#define HOTSPARE_NAME "memfile_hotspare"

#define MEMFILE_HEADER_SIZE (sizeof(memfile_header_t))
#define REC_HEADER_SIZE (sizeof(rec_header_t))
#define REC_FOOTER_SIZE (sizeof(uint32_t))
#define REC_MAGIC 0x52454331 /* "REC1" */
#define REC_VERSION 1

/* ----------------- Utilities ----------------- */
//_PRIVATE_FXN void perror_exit( const char * msg )
//{
//	perror( msg );
//	exit( EXIT_FAILURE );
//}

/* safe write all */
_PRIVATE_FXN ssize_t write_all( int fd , const void * buf , size_t sz )
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
_PRIVATE_FXN int atomic_write_and_rename( const char * dir , const char * tmpname , const char * targetname , const char * content , size_t len )
{
	char tmp[ MAX_PATH ];
	snprintf( tmp , sizeof( tmp ) , "%s/%s" , dir , tmpname );
	int fd = open( tmp , O_CREAT | O_WRONLY | O_TRUNC , 0644 );
	if ( fd < 0 ) return -1;
	if ( write_all( fd , content , len ) != ( ssize_t )len )
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
_PRIVATE_FXN status memfile_open_create( const char * path , size_t size , int create_new , memfile_t ** out_mf )
{
	INIT_BREAKABLE_FXN();

	memfile_t * mf = CALLOC_ONE( mf );
	BREAK_IF( !mf , errMemoryLow , 0 );
	strncpy( mf->path , path , sizeof( mf->path ) - 1 );
	mf->size = size;
	BREAK_IF( !pthread_mutex_init( &mf->lock , NULL ) , errResource , 1 );

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

	void * map = mmap( NULL , size , PROT_READ | PROT_WRITE , MAP_SHARED , fd , 0 );
	BREAK_IF( map == MAP_FAILED , errResource , 3 );
	
	mf->fd = fd;
	mf->map = map;
	mf->hdr = ( memfile_header_t * )map;

	/* initialize header if empty */
	if ( mf->hdr->magic != MEMFILE_HEADER_MAGIC )
	{
		/* consider this file fresh: zero then write header */
		memset( mf->map , 0 , mf->size );
		mf->hdr->magic = MEMFILE_HEADER_MAGIC;
		mf->hdr->version = MEMFILE_VERSION;
		mf->hdr->write_offset = MEMFILE_HEADER_SIZE;
		mf->hdr->seq_counter = 1;
		/* persist header */
		msync( mf->map , MEMFILE_HEADER_SIZE , MS_SYNC );
		fsync( mf->fd );
	}
	
	BEGIN_RET
	case 3:
	{
		close( fd );
	}
	case 2:
	{
		pthread_mutex_destroy( &mf->lock );
	}
	case 1:
	{
		free( mf );
	}
	N_END_RET
}

/* sync header to disk */
_PRIVATE_FXN int memfile_sync_header( memfile_t * mf )
{
	if ( !mf ) return -1;
	if ( msync( mf->map , MEMFILE_HEADER_SIZE , MS_SYNC ) != 0 ) return -1;
	if ( fsync( mf->fd ) != 0 ) return -1;
	return 0;
}

/* close memfile */
_PRIVATE_FXN void memfile_close( memfile_t * mf )
{
	if ( !mf ) return;
	munmap( mf->map , mf->size );
	close( mf->fd );
	pthread_mutex_destroy( &mf->lock );
	free( mf );
}

/* recover incomplete records at tail. returns 0 ok, -1 on error */
_PRIVATE_FXN int memfile_recover( memfile_t * mf )
{
	pthread_mutex_lock( &mf->lock );
	uint64_t write_off = mf->hdr->write_offset;
	if ( write_off < MEMFILE_HEADER_SIZE ) write_off = MEMFILE_HEADER_SIZE;

	/* scan backwards to find last valid record footer */
	if ( write_off == MEMFILE_HEADER_SIZE )
	{
		pthread_mutex_unlock( &mf->lock );
		return 0; /* empty */
	}

	/* Start from write_off: the last byte written should be the last footer end */
	size_t pos = ( size_t )write_off;
	while ( pos > MEMFILE_HEADER_SIZE )
	{
		if ( pos < REC_FOOTER_SIZE + REC_HEADER_SIZE )
		{ /* cannot contain a record */
			pos = MEMFILE_HEADER_SIZE;
			break;
		}
		/* read footer at pos - REC_FOOTER_SIZE */
		uint32_t footer_val;
		memcpy( &footer_val , ( char * )mf->map + ( pos - REC_FOOTER_SIZE ) , REC_FOOTER_SIZE );
		if ( footer_val == 0xFFFFFFFF )
		{
			/* likely unwritten area; move back a bit to search */
			/* reduce pos by 1KB or to header */
			size_t newpos = ( pos > 1024 ) ? pos - 1024 : MEMFILE_HEADER_SIZE;
			pos = newpos;
			continue;
		}
		uint32_t data_len = footer_val;
		/* compute start of record header */
		if ( pos < REC_FOOTER_SIZE + data_len + REC_HEADER_SIZE )
		{
			/* inconsistent, truncate everything up to header */
			pos = MEMFILE_HEADER_SIZE;
			break;
		}
		size_t rec_start = pos - REC_FOOTER_SIZE - data_len - REC_HEADER_SIZE;
		rec_header_t rh;
		memcpy( &rh , ( char * )mf->map + rec_start , REC_HEADER_SIZE );
		if ( rh.rec_magic == REC_MAGIC && rh.rec_version == REC_VERSION && rh.data_len == data_len )
		{
			/* found valid last record; truncate write_offset to end of this record */
			write_off = rec_start + REC_HEADER_SIZE + data_len + REC_FOOTER_SIZE;
			mf->hdr->write_offset = write_off;
			memfile_sync_header( mf );
			pthread_mutex_unlock( &mf->lock );
			return 0;
		}
		else
		{
			/* corrupt footer: it may be partial; step back slightly and continue search */
			pos -= 1;
		}
	}

	/* nothing good found -> reset to header */
	mf->hdr->write_offset = MEMFILE_HEADER_SIZE;
	memfile_sync_header( mf );
	pthread_mutex_unlock( &mf->lock );
	return 0;
}

/* write a record to memfile; returns seq number or 0 on failure */
_PRIVATE_FXN uint64_t memfile_append_record( memfile_t * mf , const void * buf , uint32_t len )
{
	pthread_mutex_lock( &mf->lock );
	uint64_t off = mf->hdr->write_offset;
	size_t need = REC_HEADER_SIZE + len + REC_FOOTER_SIZE;
	if ( off + need > mf->size )
	{
		pthread_mutex_unlock( &mf->lock );
		return 0; /* not enough space */
	}
	/* prepare header */
	rec_header_t rh;
	rh.rec_magic = REC_MAGIC;
	rh.rec_version = REC_VERSION;
	rh.seq = mf->hdr->seq_counter++;
	rh.data_len = len;
	/* write header */
	void * ph = ( char * )mf->map + off;
	memcpy( ph , &rh , REC_HEADER_SIZE );
	/* write data */
	memcpy( ( char * )mf->map + off + REC_HEADER_SIZE , buf , len );
	/* msync header+data (we may msync whole record) */
	size_t rec_total = REC_HEADER_SIZE + len;
	if ( msync( ( char * )mf->map + off , rec_total , MS_SYNC ) != 0 )
	{
		/* try to rollback seq counter if desired (not necessary) */
		pthread_mutex_unlock( &mf->lock );
		return 0;
	}
	/* write footer last */
	uint32_t footer = len;
	memcpy( ( char * )mf->map + off + REC_HEADER_SIZE + len , &footer , REC_FOOTER_SIZE );
	/* msync footer to make record durable */
	if ( msync( ( char * )mf->map + off + REC_HEADER_SIZE + len , REC_FOOTER_SIZE , MS_SYNC ) != 0 )
	{
		pthread_mutex_unlock( &mf->lock );
		return 0;
	}
	/* update header write_offset and persist it */
	mf->hdr->write_offset = off + need;
	memfile_sync_header( mf );

	uint64_t seq = rh.seq;
	pthread_mutex_unlock( &mf->lock );
	return seq;
}

/* pop last record: copies into buf (up to buf_len). returns size written or -1 on empty/error.
   Caller must free returned pointer if ret > 0 and pointer is returned. Here we copy to user buffer. */
_PRIVATE_FXN ssize_t memfile_pop_last( memfile_t * mf , void * buf , size_t buf_len , uint64_t * out_seq )
{
	pthread_mutex_lock( &mf->lock );
	uint64_t write_off = mf->hdr->write_offset;
	if ( write_off <= MEMFILE_HEADER_SIZE )
	{
		pthread_mutex_unlock( &mf->lock );
		return -1; /* empty */
	}
	size_t pos = ( size_t )write_off;
	/* read footer */
	if ( pos < REC_FOOTER_SIZE + REC_HEADER_SIZE )
	{
		pthread_mutex_unlock( &mf->lock );
		return -1;
	}
	uint32_t footer_val;
	memcpy( &footer_val , ( char * )mf->map + ( pos - REC_FOOTER_SIZE ) , REC_FOOTER_SIZE );
	uint32_t data_len = footer_val;
	if ( pos < REC_FOOTER_SIZE + data_len + REC_HEADER_SIZE )
	{
		pthread_mutex_unlock( &mf->lock );
		return -1;
	}
	size_t rec_start = pos - REC_FOOTER_SIZE - data_len - REC_HEADER_SIZE;
	rec_header_t rh;
	memcpy( &rh , ( char * )mf->map + rec_start , REC_HEADER_SIZE );
	if ( rh.rec_magic != REC_MAGIC || rh.rec_version != REC_VERSION || rh.data_len != data_len )
	{
		pthread_mutex_unlock( &mf->lock );
		return -1;
	}
	/* copy data */
	size_t tocopy = ( data_len < buf_len ) ? data_len : buf_len;
	memcpy( buf , ( char * )mf->map + rec_start + REC_HEADER_SIZE , tocopy );
	/* truncate file logically by moving write_offset back */
	mf->hdr->write_offset = rec_start;
	memfile_sync_header( mf );
	if ( out_seq ) *out_seq = rh.seq;
	pthread_mutex_unlock( &mf->lock );
	return ( ssize_t )tocopy;
}

/* seal a memfile (no further writes) */
_PRIVATE_FXN void memfile_seal( memfile_t * mf )
{
	pthread_mutex_lock( &mf->lock );
	mf->active = 2;
	memfile_sync_header( mf );
	pthread_mutex_unlock( &mf->lock );
}

/* ----------------- Manager logic ----------------- */

_PRIVATE_FXN status mm_create( mm_manager_t * mm , const char * base_dir )
{
	INIT_BREAKABLE_FXN();
	MEMSET_ZERO_O( mm );
	strncpy( mm->base_dir , base_dir , sizeof( mm->base_dir ) - 1 );
	
	BREAK_STAT( mms_array_init( &mm->files , sizeof( memfile_t ) , 1 , 10 , 0 ) , 0 );
	pthread_mutex_init( &mm->lock , NULL );
	mm->current = NULL;
	mm->hot_spare = NULL;
	/* ensure metadata file exists or create empty */
	
	BEGIN_SMPL
	N_END_RET
}

/* build file path helper */
_PRIVATE_FXN void build_path( char * out , size_t outlen , const char * dir , const char * name )
{
	snprintf( out , outlen , "%s/%s" , dir , name );
}

/* persist metadata chain: list of filenames in order, one per line */
_PRIVATE_FXN int mm_persist_chain( mm_manager_t * mm )
{
	/* build content */
	size_t cap = 4096;
	char * buf = malloc( cap );
	size_t used = 0;
	for ( size_t i = 0; i < mm->files.count; i++ )
	{
		memfile_t * pfile = NULL;
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
	int r = atomic_write_and_rename( mm->base_dir , "tmpmeta.XXXXXX" , METADATA_FILENAME , buf , used );
	free( buf );
	return r;
}

/* load chain from metadata file (if present). For simplicity we'll ignore validation errors and treat as best-effort. */
_PRIVATE_FXN status mm_load_chain( mm_manager_t * mm )
{
	INIT_BREAKABLE_FXN();

	char meta_path[ MAX_PATH ];
	build_path( meta_path , sizeof( meta_path ) , mm->base_dir , METADATA_FILENAME );
	FILE * f = fopen( meta_path , "r" );
	BREAK_IF( !f , errNotFound , 0 );
	char line[ MAX_PATH ];
	while ( fgets( line , sizeof( line ) , f ) )
	{
		/* trim newline */
		size_t L = strlen( line );
		if ( L && ( line[ L - 1 ] == '\n' || line[ L - 1 ] == '\r' ) ) line[ --L ] = '\0';
		memfile_t * mf = NULL;
		if ( memfile_open_create( line , MEMFILE_SIZE , 0 , &mf ) != errOK && !mf ) continue;
		/* recover incomplete tail */
		memfile_recover( mf );
		
		memfile_t ** ppfile = NULL;
		if ( mms_array_get_one_available_unoccopied_item_holder( &mm->files , ( void *** )&ppfile ) == errOK )
		{
			*ppfile = mf;
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

/* create a new memfile on disk with an index number */
//_PRIVATE_FXN memfile_t * mm_create_new_file( mm_manager_t * mm , size_t idx )
//{
//	char path[ MAX_PATH ];
//	char name[ 128 ];
//	snprintf( name , sizeof( name ) , "memfile_%zu.dat" , idx );
//	build_path( path , sizeof( path ) , mm->base_dir , name );
//	memfile_t * mf = NULL;
//	memfile_open_create( path , MEMFILE_SIZE , 1 , &mf );
//	if ( !mf ) return NULL;
//	return mf;
//}

/* ensure there is a hot spare (pessimistically allocate) */
_PRIVATE_FXN status mm_ensure_hot_spare( mm_manager_t * mm )
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
	build_path( path , sizeof( path ) , mm->base_dir , HOTSPARE_NAME );
	/* create hot spare file path = base_dir/HOTSPARE_NAME */
	memfile_t * mf = NULL;
	
	status d_error;
	if ( ( d_error = memfile_open_create( path , MEMFILE_SIZE , 1 , &mf ) ) )
	{
		pthread_mutex_unlock( &mm->lock );
		return d_error;
	}
	mf->active = 0; /* reserved */
	mm->hot_spare = mf;
	pthread_mutex_unlock( &mm->lock );
	return errOK;
}

/* rotate in hot spare as active memfile; create new hot spare after rotation */
_PRIVATE_FXN status mm_activate_hot_spare( mm_manager_t * mm )
{
	pthread_mutex_lock( &mm->lock );
	if ( !mm->hot_spare )
	{
		pthread_mutex_unlock( &mm->lock );
		return errNotFound;
	}
	/* rename hot_spare file to canonical memfile_N name */
	size_t newidx = mm->files.count + 1;
	char newpath[ MAX_PATH ];
	char newname[ 128 ];
	snprintf( newname , sizeof( newname ) , "memfile_%zu.dat" , newidx );
	build_path( newpath , sizeof( newpath ) , mm->base_dir , newname );
	/* atomic rename */
	if ( rename( mm->hot_spare->path , newpath ) != 0 )
	{
		/* fallback: keep as-is */
		/* but to ensure chain persists, we'll still add it */
		char oldpath[ MAX_PATH ];
		strncpy( oldpath , mm->hot_spare->path , sizeof( oldpath ) );
		mm->hot_spare->path[ 0 ] = '\0';
		build_path( mm->hot_spare->path , sizeof( mm->hot_spare->path ) , mm->base_dir , newname );
	}
	else
	{
		strncpy( mm->hot_spare->path , newpath , sizeof( mm->hot_spare->path ) - 1 );
	}
	mm->hot_spare->active = 1;
	
	memfile_t ** ppfile = NULL;
	status d_error;
	if ( ( d_error = mms_array_get_one_available_unoccopied_item_holder( &mm->files , ( void *** )&ppfile ) ) == errOK )
	{
		*ppfile = mm->hot_spare;
	}
	else
	{
		return d_error;
	}
	mm->current = mm->hot_spare;
	mm->hot_spare = NULL;
	/* persist chain metadata */
	mm_persist_chain( mm );
	/* create new hot spare in background (pessimistically) */
	pthread_mutex_unlock( &mm->lock );
	mm_ensure_hot_spare( mm );
	return errOK;
}

////////////////////////////////////////

/* expose API: init manager (create base dir if needed), load existing chain, ensure hot spare */
status mm_init( mm_manager_t * mm , LPCSTR base_dir )
{
	INIT_BREAKABLE_FXN();
	/* create dir if not exists */
	struct stat st;
	if ( stat( base_dir , &st ) != 0 )
	{
		BREAK_IF( mkdir( base_dir , 0755 ) != 0 , errNotFound , 0 );
	}
	BREAK_STAT( mm_create( mm , base_dir ) , 0 );
	BREAK_STAT( mm_load_chain( mm ) , 0 );
	BREAK_STAT( mm_ensure_hot_spare( mm ) , 0 );
	/* If no current active file, activate the hot spare */
	if ( !mm->current )
	{
		mm_activate_hot_spare( mm );
	}
	BEGIN_SMPL
	N_END_RET
}

/* store buffer into manager: returns seq (non-zero) on success, 0 on failure */
//status mm_store( mm_manager_t * mm , const void * buf , uint32_t len , uint64_t * pseq )
//{
//	if ( !mm || !buf || len == 0 ) return 0;
//	pthread_mutex_lock( &mm->lock );
//	memfile_t * cur = mm->current;
//	if ( !cur )
//	{
//		pthread_mutex_unlock( &mm->lock );
//		if ( mm_activate_hot_spare( mm ) == errOK ) // i added this line to make retry possible
//		{
//			return errRetry;
//		}
//		return errCorrupted;
//	}
//	/* try append */
//	uint64_t seq = memfile_append_record( cur , buf , len );
//	if ( seq != 0 )
//	{
//		pthread_mutex_unlock( &mm->lock );
//		return seq;
//	}
//	/* not enough space: seal current, activate hot spare, then append to new current */
//	memfile_seal( cur );
//	mm_activate_hot_spare( mm );
//	memfile_t * newcur = mm->current;
//	if ( !newcur )
//	{
//		pthread_mutex_unlock( &mm->lock );
//		return 0;
//	}
//	/* try append again */
//	seq = memfile_append_record( newcur , buf , len );
//	pthread_mutex_unlock( &mm->lock );
//	*pseq = seq;
//	return errOK;
//}

/* pop latest record from manager chain (LIFO across files) - copies into user buffer */
//ssize_t mm_pop_latest( mm_manager_t * mm , void * buf , size_t buf_len , uint64_t * out_seq )
//{
//	if ( !mm ) return -1;
//	pthread_mutex_lock( &mm->lock );
//	/* try current file first */
//	for ( ssize_t i = ( ssize_t )mm->nfiles - 1; i >= 0; --i )
//	{
//		memfile_t * mf = mm->files[ i ];
//		ssize_t r = memfile_pop_last( mf , buf , buf_len , out_seq );
//		if ( r >= 0 )
//		{
//			/* if file became empty and sealed, we can remove from chain optionally */
//			if ( mf->hdr->write_offset == MEMFILE_HEADER_SIZE && mf->active == 2 )
//			{
//				/* optionally remove file - here we keep on disk but reduce nfiles by moving index */
//				/* We won't delete from disk for safety; for production you can compact. */
//			}
//			pthread_mutex_unlock( &mm->lock );
//			return r;
//		}
//	}
//	pthread_mutex_unlock( &mm->lock );
//	return -1; /* nothing */
//}

/* shutdown manager, close files, persist metadata */
void mm_shutdown( mm_manager_t * mm )
{
	if ( !mm ) return;
	pthread_mutex_lock( &mm->lock );
	mm_persist_chain( mm );
	for ( size_t i = 0; i < mm->files.count; i++ )
	{
		memfile_t * pfile = NULL;
		if ( mms_array_get_s( &mm->files , i , ( void ** )&pfile ) == errOK )
		{
			memfile_close( pfile );
		}
	}
	if ( mm->hot_spare ) memfile_close( mm->hot_spare );
	mms_array_free( &mm->files );
	pthread_mutex_unlock( &mm->lock );
	pthread_mutex_destroy( &mm->lock );
}
