/*
	80/5/22, hamidi:
	1.  errorStr and ErrorVal functions modified, so that a new value is generated for a new
		d_error, but without any d_error string instead of not generating a new value for it.
		ErrorVal also returns a proper string made for errors that have no string
	2.  errStrs became of type const char * instead of being of type struct
	Note:
	1.  if u change size of array errStrs, change size of buf in function errorStr
	2.  size of errStrs indicates how many errors have strings, that is there's no limitation
		for number of errors anymore
*/

#define Uses_basename
#define Uses_strlen
#define Uses_malloc
#define Uses_close
#define Uses_va_list
#define Uses_time_t
#define Uses_wchar_t
#define Uses_sprintf_s
#define Uses_sprintf_s
#define Uses_strcpy_s
#define Uses_errno
#define Uses_size_t

#include <general.dep>


static short _err = NEXT_GENERAL_ERROR_VALUE;
static LPCSTR errStrs[10000]={"errOK","errGeneral","errMemoryLow","errInvalidString","errCanceled","syntax error"};


//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

//-------------------------------------------------------------------------
const char EOS = '\0'; // 0
//const WCHAR WEOS = _WT('\0'); // 0
const char LF = '\n'; // line feed 10
const char CR = '\r'; // carriage return 13

const char _newLineFeed[] = { CR , LF , EOS };

const char _fileNameDelimiter[] = { '\\' , '/' , EOS };

const char _SPFD = '\\'; // Standard path file delimiter

const char _UTF8Sign[] = "\xEF\xBB\xBF";
const char _UTF16LSign[] = "\xFF\xFE"; // little-endian . Unicode in Microsoft terminology
const char _UTF16BSign[] = "\xFE\xFF"; // big-endian . Unicode in Microsoft terminology
const char _UTF32LSign[] = "\xFF\xFE\x00\x00"; // little-endian
const char _UTF32BSign[] = "\x00\x00\xFE\xFF"; // big-endian



_EXPORT status internalErrorVal(LPCSTR errStr)
{
	if (-_err < (int)COUNTOF(errStrs)) errStrs[-_err]=errStr;
	return _err--;
}

_EXPORT LPCSTR internalErrorStr(status errValue)
{
	if ( errValue > errOK )
	{
		//ASSERT( 0 );
		return "An error occur in mfc fxn"; // mohsen ageh error > 0 mi toneh beh M_mkShowMsg bereh pas chera barayeh amadanesh beh in fxn fekri nashodeh
	}
	if ( errValue <= errOK && errValue > _err )
	{
		if ( -errValue < (int)COUNTOF(errStrs))
		{
			return errStrs[-errValue];
		}
		else
		{
			static char buf[81];
			sprintf_s (buf, "Error #%d", -(int)errValue);
			return buf;
		}
	}
	return "Error value out of range";
}

char *newStr( LPCSTR str )
{
	if(!str)str="";
	if(!str[0])return "";
	size_t sz=strlen(str)+1;
	char *temp=NEWBUF(char,sz);
	if (temp) strcpy(temp,str);
	return temp;
}

//const char * __msg( char * msg_holder , size_t size_of_msg_holder , const char * msg , int line_number )
//{
//	snprintf( msg_holder , size_of_msg_holder , "%s: ln(%d)\n" , msg , line_number );
//	return msg_holder;
//}

const char * make_msg_appnd_sys_err( char * msg_holder , size_t size_of_msg_holder , const char * cst_msg )
{
	if ( errno != 0 )
	{
		snprintf( msg_holder , size_of_msg_holder , "%s (%d):%s" , cst_msg , errno , strerror(errno) );
	}
	else
	{
		snprintf( msg_holder , size_of_msg_holder , "%s" , cst_msg );
	}
	return msg_holder;
}

const char * __snprintf( char * msg_holder , size_t size_of_msg_holder , const char * format , ... )
{
	va_list args;

	va_start( args , format );
	vsnprintf( msg_holder , size_of_msg_holder , format , args );
	va_end( args );

	return msg_holder;
}

void _close_socket( int * socket_id )
{
	close( *socket_id );
	*socket_id = -1;
}

const char * read_file( const char * path , char * pInBuffer /*= NULL*/ )
{
	FILE * file = fopen( path , "r" );
	if ( file == NULL )
	{
		fprintf( stderr , "Expected file \"%s\" not found" , path );
		return NULL;
	}
	fseek( file , 0 , SEEK_END );
	long len = ftell( file );
	fseek( file , 0 , SEEK_SET );
	char * buffer = pInBuffer ? pInBuffer : malloc( (size_t)(len + 1) );

	if ( buffer == NULL )
	{
		fprintf( stderr , "Unable to allocate memory for file" );
		fclose( file );
		return NULL;
	}

	#pragma GCC diagnostic ignored "-Wunused-result"
	fread( buffer , 1 , (size_t)len , file );
	#pragma GCC diagnostic pop
	buffer[ len ] = EOS;

	fclose( file );
	file = NULL;

	return ( const char * )buffer;
}

const char * trim_trailing_zeros(char *s) {
    char *dot = strchr(s, '.');
    if (!dot) return s;

    char *end = s + strlen(s) - 1;
    while (end > dot && *end == '0') {
        *end = '\0';
        end--;
    }

    if (*end == '.') {
        *end = '\0';
    }
	return s;
}

const char * format_pps( char * buf , size_t buflen , ubigint pps , int number_of_float , const char * unit_name /*= "pps"*/ )
{
	const char * units[] = { "", "K", "M", "G", "T", "P" };
	double value = ( double )pps;
	int unit = 0;

	while ( value >= 1000.0 && unit < 5 )
	{
		value /= 1000.0;
		unit++;
	}
	char buf2[64];
	snprintf( buf2 , sizeof(buf2) , "%%.%df" , number_of_float );
	snprintf( buf , buflen , buf2 , value );
	snprintf( buf2 , sizeof(buf2) , "%s%s%s%s" , trim_trailing_zeros(buf) , ( strlen(units[unit]) > 0 || strlen(unit_name) > 0 ? " " : "" ) , units[unit] , unit_name);
	strncpy( buf , buf2 , buflen );

	return buf;
}

void format_clock_time( const struct timespec * ts , char * buffer , size_t buf_size )
{
	struct tm tm_info;
	localtime_r( &ts->tv_sec , &tm_info );  // Convert to local time

	// Format only the clock time part
	strftime( buffer , buf_size , "%H:%M:%S" , &tm_info );
}

void round_up_to_next_interval( struct timespec * now , int min_val , int interval , struct timespec * result )
{
	result->tv_sec = ( ( ( now->tv_sec + min_val ) / interval ) + 1 ) * interval;
	result->tv_nsec = 0;
}

#define MAX_PATH 4096

// Check if file exists
static int file_exists( const char * path )
{
	return access( path , F_OK ) == 0;
}

// Get current date string in YYYYMMDD_HHMMSS format
static void get_datetime_str( char * buffer , size_t size )
{
	time_t now = time( NULL );
	struct tm * t = localtime( &now );
	strftime( buffer , size , "%Y%m%d_%H%M%S" , t );
}

FILE * create_unique_file( const char * path , const char * filename /*=NULL(app+date)*/ )
{
	char final_path[ MAX_PATH ] = { 0 };
	char name_part[ 256 ] = { 0 };
	char datetime[ 64 ] = { 0 };

	// Determine base filename
	if ( !filename )
	{
		// Use executable name + date if filename is null
		char exe_path[ MAX_PATH ];
		ssize_t len = readlink( "/proc/self/exe" , exe_path , sizeof( exe_path ) - 1 );
		if ( len == -1 )
		{
			perror( "readlink" );
			return NULL;
		}
		exe_path[ len ] = '\0';
		snprintf( name_part , sizeof( name_part ) , "%s" , basename( exe_path ) );
		get_datetime_str( datetime , sizeof( datetime ) );
		snprintf( name_part + strlen( name_part ) , sizeof( name_part ) - strlen( name_part ) , "_%s.txt" , datetime );
	}
	else
	{
		snprintf( name_part , sizeof( name_part ) , "%s" , filename );
	}

	// Normalize path: check if trailing slash is needed
	if ( path && strlen( path ) > 0 )
	{
		size_t path_len = strlen( path );
		if ( path[ path_len - 1 ] == '/' || path[ path_len - 1 ] == '\\' )
		{
			snprintf( final_path , sizeof( final_path ) , "%s%s" , path , name_part );
		}
		else
		{
			snprintf( final_path , sizeof( final_path ) , "%s/%s" , path , name_part );
		}
	}
	else
	{
		snprintf( final_path , sizeof( final_path ) , "%s" , name_part );
	}

	// If file exists, add postfix number
	int counter = 1;
	while ( file_exists( final_path ) )
	{
		if ( path && strlen( path ) > 0 )
		{
			size_t path_len = strlen( path );
			if ( path[ path_len - 1 ] == '/' || path[ path_len - 1 ] == '\\' )
			{
				snprintf( final_path , sizeof( final_path ) , "%s%s_%d" , path , name_part , counter++ );
			}
			else
			{
				snprintf( final_path , sizeof( final_path ) , "%s/%s_%d" , path , name_part , counter++ );
			}
		}
		else
		{
			snprintf( final_path , sizeof( final_path ) , "%s_%d" , name_part , counter++ );
		}
	}

	FILE * file = fopen( final_path , "w" );
	if ( !file )
	{
		perror( "fopen" );
		return NULL;
	}

	return file;
}

void format_elapsed_time( time_t start , time_t end , char * buffer , size_t buf_size )
{
	if ( !buffer || buf_size == 0 ) return;

	time_t elapsed = ( start > end ) ? ( start - end ) : ( end - start );

	int days = ( int )( elapsed / LLONG( 86400 ) );
	int hours = ( int )( ( elapsed % LLONG( 86400 ) ) / LLONG( 3600 ) );
	int minutes = ( int )( ( elapsed % LLONG( 3600 ) ) / LLONG( 60 ) );
	int seconds = ( int )( elapsed % LLONG( 60 ) );

	int offset = 0;

	if ( days > 0 )
		offset += snprintf( buffer + offset , buf_size - (size_t)offset , "%dd " , days );
	if ( hours > 0 )
		offset += snprintf( buffer + offset , buf_size - (size_t)offset , "%dh " , hours );
	if ( minutes > 0 )
		offset += snprintf( buffer + offset , buf_size - (size_t)offset , "%dm " , minutes );
	if ( seconds > 0 || offset == 0 ) // always include at least seconds
		offset += snprintf( buffer + offset , buf_size - (size_t)offset , "%ds" , seconds );

	// Remove trailing space if any
	size_t len = strlen( buffer );
	if ( len > 0 && buffer[ len - 1 ] == ' ' )
		buffer[ len - 1 ] = '\0';
}
