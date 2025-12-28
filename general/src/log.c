#define Uses_va_start
#define Uses_time
#define Uses_FILE
#define Uses_LOG

#include <general.dep>

#ifdef _DEBUG

static FILE * log_file = NULL;  // Global file pointer
static const char * level_str[] = { "DEBUG", "INFO", "WARN", "ERROR" };

// Initialize the logger (creates file or appends if exists)
status log_init( const char * filename , bool bnew )
{
	
#ifdef ENABLE_LOCK_LOGGING
	int flags = O_RDWR;
	flags |= O_CREAT;
	int fd = open( "./log_lock.txt" , flags , 0644 );
	if ( fd >= 0 )
	{
		__log_lock = mmap( NULL , ( ( size_t )128 * 1024 * 1024 ) , PROT_READ | PROT_WRITE , MAP_SHARED , fd , 0 );

		off_t cur = lseek( fd , 0 , SEEK_END );
		if ( ( size_t )cur < ( ( size_t )128 * 1024 * 1024 ) )
		{
			ftruncate( fd , ( __off_t )( ( size_t )128 * 1024 * 1024 ) );
		}

		memset( __log_lock , 0 , ( ( size_t )128 * 1024 * 1024 ) );
	}
#endif


	log_file = fopen( filename , bnew ? "w" : "a" );
	if ( !log_file )
	{
		return errOpen;
	}

	// Optional: write session header
	time_t now = time( NULL );
	fprintf( log_file , "\n--- Log started at %s" , ctime( &now ) );
	fflush( log_file );
	return errOK;
}

// Write a formatted log message with level and timestamp
// this fxn put \n at the end
void log_write( LogLevel level , const char * fmt , ... )
{
	if ( !log_file )
		return; // Logging not initialized

	time_t now = time( NULL );
	struct tm * t = localtime( &now );
	char timebuf[ 20 ];
	strftime( timebuf , sizeof( timebuf ) , "%Y-%m-%d %H:%M:%S" , t );

	fprintf( log_file , "[%s] [%s] " , timebuf , level_str[ level ] );

	va_list args;
	va_start( args , fmt );
	vfprintf( log_file , fmt , args );
	va_end( args );

	fprintf( log_file , "\n" );
	fflush( log_file );  // ensure log written immediately
}

// Close log file
void log_close( void )
{
	if ( log_file )
	{
		fprintf( log_file , "--- Log closed ---\n\n" );
		fclose( log_file );
		log_file = NULL;
	}
}


#ifdef ENABLE_LOCK_LOGGING
void * __log_lock = NULL;
int __log_lock_pos = 0;
#endif


#endif
