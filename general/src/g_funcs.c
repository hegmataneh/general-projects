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

#define Uses_flock
#define Uses_floor
#define Uses_tcp_info
#define Uses_gettimeofday
#define Uses_INTERNAL_ERR
#define Uses_DETAILED_ERR
#define Uses_fcntl
#define Uses_pthread_mutex_timedlock
#define Uses_sem_t
#define Uses_TCP_KEEPIDLE
#define Uses_bool
#define Uses_sem_t
#define Uses_pollfd
#define Uses_STRICMP
#define Uses_STRCMP
#define Uses_WARNING
#define Uses_isprint
#define Uses_INT_MAX
#define Uses_send
#define Uses_basename
#define Uses_strlen
#define Uses_MEMSET_ZERO
#define Uses_close
#define Uses_va_list
#define Uses_time_t
#define Uses_wchar_t
#define Uses_sprintf_s
#define Uses_strcpy_s
#define Uses_errno
#define Uses_size_t
#define Uses_strcasestr

#include <general.dep>

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

const sockfd invalid_fd = -1;

const unsigned char MSB_MARKERS[8] = {
	0x3C,  /* M0 */
	0xC3,  /* M1 */
	0x5A,  /* M2 */
	0xA5,  /* M3 */
	0x96,  /* M4 */
	0x69,  /* M5 */
	0xF0,  /* M6 */
	0x0F   /* M7 */
};

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

//_EXPORT status internalErrorVal(LPCSTR errStr)
//{
//	if (-__local_err_val < (int)COUNTOF(__local_err_strs)) __local_err_strs[-__local_err_val]=errStr;
//	return __local_err_val--;
//}

_EXPORT IMMORTAL_LPCSTR internalErrorStr( status errValue , bool simple_text )
{
	if ( errValue > errOK )
	{
		//WARNING( 0 );
		return "An error occur in os kernel fxn"; // mohsen ageh error > 0 mi toneh beh M_mkShowMsg bereh pas chera barayeh amadanesh beh in fxn fekri nashodeh
	}
	if ( errValue <= errOK && errValue > NEXT_GENERAL_ERROR_VALUE )
	{
		if ( -errValue < ( int )COUNTOF( __local_err_strs ) )
		{
			return simple_text ? __local_err_strs[ -errValue ].simple_text : __local_err_strs[ -errValue ].local_sign_text;
		}
		else
		{
			static char buf[ 81 ];
			sprintf_s( buf , "Error #%d" , -( int )errValue );
			return buf;
		}
	}
	return "Error value out of range";
}


//_WEAK_ATTR void M_showMsg( LPCSTR msg )
//{
//
//}

LPSTR newStr( LPCSTR str )
{
	if(!str)str="";
	if(!str[0])return "";
	size_t sz=STRLEN(str)+1;
	LPSTR temp=MALLOC_AR(temp,sz);
	if (temp) strcpy(temp,str);
	return temp;
}

IMMORTAL_LPCSTR systemErrorStr( int prcID /*just for future use*/ )
{
	return errno ? ( LPCSTR )strerror( errno ) : "";
}

LPCSTR __FUNCTION_shrtn( LPCSTR str ) // just shorten __FUNCTION__
{
	int len = ( int )strlen( str );
	return ( LPCSTR )( ( ( LPSTR )str ) + (len > 10 ? len - 10 : 0) );
}

LPCSTR __FILE_shrtn( LPCSTR src_str ) // just shorten __FUNCTION__
{
	LPCSTR str = get_filename( src_str );
	int len = ( int )strlen( str );
	return ( LPCSTR )( ( ( LPSTR )str ) + ( len > 10 ? len - 10 : 0 ) );
}

IMMORTAL_LPCSTR __conditional_internalErrorStr( status err , LPCSTR ifnotstr , bool simple_text )
{
	return ( ( ifnotstr && ifnotstr[0] ) ? "" : internalErrorStr( err , simple_text ) );
}

//LPCSTR __msg( LPSTR  msg_holder , size_t size_of_msg_holder , LPCSTR msg , int line_number )
//{
//	snprintf( msg_holder , size_of_msg_holder , "%s: ln(%d)\n" , msg , line_number );
//	return msg_holder;
//}

//LPCSTR make_msg_appnd_sys_err( LPSTR  msg_holder , size_t size_of_msg_holder , LPCSTR cst_msg )
//{
//	if ( errno != 0 )
//	{
//		snprintf( msg_holder , size_of_msg_holder , "%s (%d):%s" , cst_msg , errno , strerror(errno) );
//	}
//	else
//	{
//		snprintf( msg_holder , size_of_msg_holder , "%s" , cst_msg );
//	}
//	return msg_holder;
//}

LPCSTR __snprintf( LPSTR  msg_holder , size_t size_of_msg_holder , LPCSTR format , ... )
{
	va_list args;

	va_start( args , format );
	vsnprintf( msg_holder , size_of_msg_holder , format , args );
	va_end( args );

	return msg_holder;
}

void _close_socket( sockfd * socket_id , Brief_Err * imortalErrStr )
{
	if ( *socket_id > invalid_fd )
	{
		KERNEL_CALL_NORET( shutdown( *socket_id , SHUT_RDWR ) == -1 , "shutdown()" , imortalErrStr , true ); // Use shutdown() before close() to send a clean FIN.
		KERNEL_CALL_NORET( close( *socket_id ) == -1 , "close()" , imortalErrStr , true );
		*socket_id = invalid_fd;
	}
}

LPCSTR read_file( LPCSTR path , LPSTR  pInBuffer /*= NULL*/ , Brief_Err * imortalErrStr )
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
	LPSTR buf = pInBuffer ? pInBuffer : ( LPSTR )MALLOC( (size_t)(len + 1) );

	if ( buf == NULL )
	{
		fprintf( stderr , "Unable to allocate memory for file" );
		KERNEL_CALL_NORET( fclose( file ) == EOF , "fclose()" , imortalErrStr , true );
		return NULL;
	}

	#pragma GCC diagnostic ignored "-Wunused-result"
	fread( buf , 1 , (size_t)len , file );
	#pragma GCC diagnostic pop
	buf[ len ] = EOS;

	KERNEL_CALL_NORET( fclose( file ) == EOF , "fclose()" , imortalErrStr , true );
	file = NULL;

	return ( LPCSTR )buf;
}

LPCSTR trim_trailing_zeros(LPSTR s) {
    LPSTR dot = strchr(s, '.');
    if (!dot) return s;

    LPSTR end = s + STRLEN(s) - 1;
    while (end > dot && *end == '0') {
        *end = '\0';
        end--;
    }

    if (*end == '.') {
        *end = '\0';
    }
	return s;
}

// Helper: absolute value for double
_PRIVATE_FXN double dabs( double x )
{
	return x < 0 ? -x : x;
}

int last_valid_digit_14( double x )
{
	// If x is zero, no digits exist.
	if ( x == 0.0 ) return 0;

	// 1) Normalize: ensure positive
	x = dabs( x );

	// 2) Keep only 14 digits of precision:
	//    round(x to 14 decimal digits)
	//    scale = 10^14
	const double scale = 1e14;
	double rounded = floor( x * scale + 0.5 ) / scale;

	// 3) Extract fractional part after rounding
	double frac = rounded - floor( rounded );

	// 4) Convert the fractional part into integer digits
	long long digits = ( long long )( frac * scale + 0.0000000001 );

	// 5) Find the last non-zero digit
	while ( digits > 0 && ( digits % 10 ) == 0 )
	{
		digits /= 10;
	}

	if ( digits == 0 )
		return 0;   // no non-zero digits inside 14-digit precision

	return ( int )( digits % 10 );
}

LPCSTR format_pps_double( LPSTR  buf , size_t buflen , double pps , int number_of_float , LPCSTR unit_name /*= "pps"*/ , LPCSTR prefix_string )
{
	LPCSTR units[] = { "", "K", "M", "G", "T", "P" };
	double fvalue = pps;
	int unit = 0;

	while ( fvalue >= 1000.0 && unit < 5 )
	{
		fvalue /= 1000.0;
		unit++;
	}

	int last14 = last_valid_digit_14( fvalue );
	char buf14[14];
	snprintf( buf14 , sizeof(buf14) , " %d" , last14 );

	char buf2[ DEFAULT_MFS_BUF_SZ ];
	snprintf( buf2 , sizeof( buf2 ) , "%%.%df" , number_of_float ); // make format
	snprintf( buf , buflen , buf2 , fvalue );
	snprintf( buf2 , sizeof( buf2 ) , "%s%s%s%s%s%s" , 
		prefix_string ? prefix_string : "" ,
		trim_trailing_zeros( buf ) ,
		""/*( STRLEN(units[unit]) > 0 || STRLEN(unit_name) > 0 ? " " : "" )*/ ,
		units[unit] ,
		unit_name ,
		( unit >= 1 && last14 != 0 ) ? buf14 : "" );
	strncpy( buf , buf2 , buflen );

	return buf;
}

void format_clock_time( const struct timespec * ts , LPSTR  buffer , size_t buf_size )
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
static int file_exists( LPCSTR path )
{
	return access( path , F_OK ) == 0;
}

// Get current date string in YYYYMMDD_HHMMSS format
static void get_datetime_str( LPSTR  buffer , size_t size )
{
	time_t now = time( NULL );
	struct tm * t = localtime( &now );
	strftime( buffer , size , "%Y%m%d_%H%M%S" , t );
}

FILE * create_unique_file( LPCSTR path , LPCSTR filename /*=NULL(app+date)*/ , Brief_Err * imortalErrStr )
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
			STORE_BRIEF_ERR( imortalErrStr , "readlink()\n" , true );
			return NULL;
		}
		exe_path[ len ] = '\0';
		snprintf( name_part , sizeof( name_part ) , "%s" , basename( exe_path ) );
		get_datetime_str( datetime , sizeof( datetime ) );
		snprintf( name_part + STRLEN( name_part ) , sizeof( name_part ) - STRLEN( name_part ) , "_%s.txt" , datetime );
	}
	else
	{
		snprintf( name_part , sizeof( name_part ) , "%s" , filename );
	}

	// Normalize path: check if trailing slash is needed
	if ( path && STRLEN( path ) > 0 )
	{
		size_t path_len = STRLEN( path );
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
		if ( path && STRLEN( path ) > 0 )
		{
			size_t path_len = STRLEN( path );
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
		STORE_BRIEF_ERR( imortalErrStr , "fopen()\n" , true );
		return NULL;
	}

	return file;
}

void format_elapsed_time( time_t start , time_t end , LPSTR  buffer , size_t buf_size )
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
	size_t len = STRLEN( buffer );
	if ( len > 0 && buffer[ len - 1 ] == ' ' )
		buffer[ len - 1 ] = '\0';
}

//// difference in nanoseconds
long long timespec_diff_nsec( struct timespec * start , struct timespec * end )
{
	return ( long long )( end->tv_sec - start->tv_sec ) * 1000000000LL
		+ ( end->tv_nsec - start->tv_nsec );
}

// difference in milliseconds
double timespec_diff_ms( struct timespec * start , struct timespec * end )
{
	return ( double )( end->tv_sec - start->tv_sec ) * 1000.0
		+ ( double )( end->tv_nsec - start->tv_nsec ) / 1000000.0;
}

//// difference in seconds
//double timespec_diff_sec( struct timespec * start , struct timespec * end )
//{
//	return ( end->tv_sec - start->tv_sec )
//		+ ( end->tv_nsec - start->tv_nsec ) / 1000000000.0;
//}
//
//struct timespec timespec_sub( struct timespec start , struct timespec end )
//{
//	struct timespec temp;
//	temp.tv_sec = end.tv_sec - start.tv_sec;
//	temp.tv_nsec = end.tv_nsec - start.tv_nsec;
//	if ( temp.tv_nsec < 0 )
//	{
//		temp.tv_sec -= 1;
//		temp.tv_nsec += 1000000000;
//	}
//	return temp;
//}
//
//struct timespec timeval_diff_timespec( struct timeval start , struct timeval end )
//{
//	struct timespec temp;
//
//	// convert to seconds + nanoseconds
//	temp.tv_sec = end.tv_sec - start.tv_sec;
//	temp.tv_nsec = ( end.tv_usec - start.tv_usec ) * 1000; // usec → nsec
//
//	if ( temp.tv_nsec < 0 )
//	{
//		temp.tv_sec -= 1;
//		temp.tv_nsec += 1000000000;
//	}
//
//	return temp;
//}

long long timeval_diff_nsec( struct timeval * start , struct timeval * end )
{
	long long res = ( long long )( end->tv_sec - start->tv_sec ) * 1000000000LL
		+ ( long long )( end->tv_usec - start->tv_usec ) * 1000LL;
	if ( res < 0 ) res = 0;
	return res;
}

double timeval_diff_ms( struct timeval * start , struct timeval * end )
{
	return ( double )timeval_diff_nsec( start , end ) / 1000000.0;
}

void format_elapsed_time_with_millis( struct timeval start , struct timeval end , LPSTR  buffer , size_t buf_size , int type /*=0*/ )
{
	if ( !buffer || buf_size == 0 ) return;

	// Calculate elapsed time
	time_t sec = end.tv_sec - start.tv_sec;
	suseconds_t usec = end.tv_usec - start.tv_usec;

	if ( usec < 0 )
	{
		sec -= 1;
		usec += 1000000;
	}

	if ( type == 0 )
	{
		// Break down time
		int millis = (int)(usec / (suseconds_t)(1000));
		int days = (int)(sec / LLONG(86400));
		int hours = (int)( sec % LLONG(86400) / LLONG(3600));
		int minutes = (int)( sec % LLONG(3600) / LLONG(60));
		int seconds = (int)(sec % LLONG(60));

		int written = 0;

		if ( days > 0 )
			written += snprintf( buffer + written , buf_size - (size_t)written , "%02d" , days );

		if ( hours > 0 || written > 0 )
			written += snprintf( buffer + written , buf_size - (size_t)written , "%s%02d" , ( written ? ":" : "" ) , hours );

		if ( minutes > 0 || written > 0 )
			written += snprintf( buffer + written , buf_size - (size_t)written , "%s%02d" , ( written ? ":" : "" ) , minutes );

		if ( seconds > 0 || written > 0 )
			written += snprintf( buffer + written , buf_size - (size_t)written , "%s%02d" , ( written ? ":" : "" ) , seconds );

		// Always include milliseconds
		snprintf( buffer + written , buf_size - (size_t)written , "%s%03d" , ( written ? ":" : "" ) , millis );
	}
	else if ( type == 1 )
	{
		int millis = (int)(usec / 1000);

		// Format as SS:MMM (2-digit seconds, 3-digit millis)
		snprintf( buffer , buf_size , "%ld.%03d" , sec , millis );
	}
}


//----- stristr ------------------------------------------------------

IN_GENERAL LPCSTR stristr( LPCSTR sStr , LPCSTR sSubstr ) // Written By Mohsen
{
	return ( LPCSTR )strcasestr( sStr , sSubstr );
}

IN_GENERAL LPCSTR strrstr( LPCSTR sStr , LPCSTR sSubStr ) // Written By Mohsen
{
	LPCSTR lpLastSubStr = NULL;
	while ( ( sStr = strstr( sStr , sSubStr ) ) )
	{
		lpLastSubStr = sStr;
		sStr++;
	}
	return lpLastSubStr;
}

IN_GENERAL LPCSTR strristr( LPCSTR sStr , LPCSTR sSubStr ) // Written By Mohsen
{
	LPCSTR lpLastSubStr = NULL;
	while ( ( sStr = stristr( sStr , sSubStr ) ) )
	{
		lpLastSubStr = sStr;
		sStr++;
	}
	return lpLastSubStr;
}

LPCSTR strrstr_try( LPCSTR haystack , LPCSTR needle )
{
	LPCSTR res = strrstr( haystack , needle );
	return res ? res : haystack;
}

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

IN_GENERAL LPCSTR strstrs( LPCSTR sSrc , int * const pISubStr , int countstrs , LPCSTR * const sSubstrs ) // Written By Mohsen
{
	LPCSTR pOutStr = NULL , pTempOutStr;
	for ( int nstrs = 0 ; nstrs < countstrs ; nstrs++ )
	{
		if ( ( pTempOutStr = ( LPCSTR )strstr( ( LPSTR )sSrc , sSubstrs[ nstrs ] ) ) )
		{
			if ( !pOutStr || pTempOutStr < pOutStr )
			{
				pOutStr = pTempOutStr;
				if ( pISubStr ) *pISubStr = nstrs;
			}
			//return pOutStr;
		}
	}
	return pOutStr;
}

IN_GENERAL LPCSTR stristrs( LPCSTR sSrc , int * const pISubStr , int countstrs , LPCSTR * const sSubstrs ) // Written By Mohsen
{
	LPCSTR pOutStr = NULL , pTempOutStr;
	for ( int nstrs = 0 ; nstrs < countstrs ; nstrs++ )
	{
		if ( ( pTempOutStr = ( LPCSTR )stristr( ( LPSTR )sSrc , sSubstrs[ nstrs ] ) ) )
		{
			if ( !pOutStr || pTempOutStr < pOutStr )
			{
				pOutStr = pTempOutStr;
				if ( pISubStr ) *pISubStr = nstrs;
			}
			//return pOutStr;
		}
	}
	return pOutStr;
}

//IN_GENERAL LPCSTR strstrs( LPCSTR sSrc , int * const pISubStr , int count , ... ) // Written By Mohsen
//{
//	GcMshCollection< LPCSTR > sSubstrs;
//	copyVarArg_assign( sSubstrs , &count );
//	return strstrs( sSrc , pISubStr , count , sSubstrs.getArray() );
//}
//
//IN_GENERAL LPCSTR stristrs( LPCSTR sSrc , int * const pISubStr , int count , ... ) // Written By Mohsen
//{
//	GcMshCollection< LPCSTR > sSubstrs;
//	copyVarArg_assign( sSubstrs , &count );
//	return stristrs( sSrc , pISubStr , count , sSubstrs.getArray() );
//}
//
//IN_GENERAL LPCSTR strstrs( LPCSTR sSrc , int count , ... ) // Written By Mohsen
//{
//	int iSubStr;
//	GcMshCollection< LPCSTR > sSubstrs;
//	copyVarArg_assign( sSubstrs , &count );
//	return strstrs( sSrc , &iSubStr , count , sSubstrs.getArray() );
//}
//
//IN_GENERAL LPCSTR stristrs( LPCSTR sSrc , int count , ... ) // Written By Mohsen
//{
//	int iSubStr;
//	GcMshCollection< LPCSTR > sSubstrs;
//	copyVarArg_assign( sSubstrs , &count );
//	return stristrs( sSrc , &iSubStr , count , sSubstrs.getArray() );
//}
//
//IN_GENERAL LPCSTR strstrs( LPCSTR sSrc , LPCSTR doubleStrBuf , int * const pISubStr ) // Written By Mohsen
//{
//	GcDoubleString ds( doubleStrBuf );
//	GcMshCollection< GcString > sSubstrs;
//	ds.get( sSubstrs );
//	GcMshCollection< LPCSTR > pstrs;
//	appendCollection_assign( pstrs , sSubstrs );
//	return strstrs( sSrc , pISubStr , pstrs.getCount() , pstrs.getArray() );
//}
//
//IN_GENERAL LPCSTR stristrs( LPCSTR sSrc , LPCSTR doubleStrBuf , int * const pISubStr ) // Written By Mohsen
//{
//	GcDoubleString ds( doubleStrBuf );
//	GcMshCollection< GcString > sSubstrs;
//	ds.get( sSubstrs );
//	GcMshCollection< LPCSTR > pstrs;
//	appendCollection_assign( pstrs , sSubstrs );
//	return stristrs( sSrc , pISubStr , pstrs.getCount() , pstrs.getArray() );
//}

IN_GENERAL void convertChr( LPCSTR str , LPCSTR from , LPCSTR to ) // Written By Mohsen
{
	size_t t1 = STRLEN( from );
	WARNING( t1 == STRLEN( to ) );
	LPSTR  lpC;
	while ( ( lpC = strpbrk( ( LPSTR )str , from ) ) )
	{
		*lpC = to[ ( LPSTR  )MEMCHR( from , *lpC , t1 ) - from ];
	}
}

IN_GENERAL void replaceChr( char fromChar , char toChar , LPCSTR str , size_t sz ) // Written By Mohsen
{
	if ( sz == -1 ) sz = STRLEN( str );
	LPSTR  lpHead;
	while ( ( lpHead = ( LPSTR  )MEMCHR( str , fromChar , sz ) ) )
	{
		*lpHead = toChar;
	}
}

//IN_GENERAL void_p removeChr( void_p const str /*in out*/ , char chr , int sz /*in*/ , int * const pSz /*out*/ ) // 1389/11/19
//{
//	WARNING( pSz );
//	LPSTR  basestr = ( LPSTR  )str;
//	LPSTR  lpLastChr = basestr;
//	while ( ( lpLastChr = ( LPSTR  )MEMCHR( lpLastChr , chr , basestr + sz - lpLastChr ) ) )
//	{
//		MEMMOVE( lpLastChr , lpLastChr + 1 , basestr + sz - lpLastChr - 1 );
//		sz--;
//	}
//	if ( pSz ) *pSz = sz;
//	return sz ? basestr : NULL;
//}
//
//IN_GENERAL void_p removeiChr( void_p const str /*in out*/ , char chr , int sz /*in*/ , int * const pSz /*out*/ ) // 1389/11/19
//{
//	WARNING( pSz );
//	LPSTR  basestr = ( LPSTR  )str;
//	LPSTR  lpLastChr = basestr;
//	while ( ( lpLastChr = ( LPSTR  )memichr( lpLastChr , chr , basestr + sz - lpLastChr ) ) )
//	{
//		MEMMOVE( lpLastChr , lpLastChr + 1 , basestr + sz - lpLastChr - 1 );
//		sz--;
//	}
//	if ( pSz ) *pSz = sz;
//	return sz ? basestr : NULL;
//}
//
//IN_GENERAL void_p removeChrs( void_p const sMem /*in out*/ , size_t memSz /*in*/ , size_t * const pSz /*out*/ , void_p const chrs /*in*/ , size_t chrsCount /*in*/ ) // 1390/06/03
//{
//	WARNING( pSz );
//	LPSTR  basestr = ( LPSTR  )sMem;
//	LPSTR  lpLastChr = basestr;
//	while ( ( lpLastChr = ( LPSTR  )memchrs( lpLastChr , basestr + memSz - lpLastChr , chrs , chrsCount ) ) )
//	{
//		MEMMOVE( lpLastChr , lpLastChr + 1 , basestr + memSz - lpLastChr - 1 );
//		memSz--;
//	}
//	if ( pSz ) *pSz = memSz;
//	return memSz ? basestr : NULL;
//}
//
//IN_GENERAL void_p removeiChrs( void_p const sMem /*in out*/ , size_t memSz /*in*/ , size_t * const pSz /*out*/ , void_p const chrs /*in*/ , size_t chrsCount /*in*/ ) // 1390/06/03
//{
//	WARNING( pSz );
//	LPSTR  basestr = ( LPSTR  )sMem;
//	LPSTR  lpLastChr = basestr;
//	while ( ( lpLastChr = ( LPSTR  )memichrs( lpLastChr , basestr + memSz - lpLastChr , chrs , chrsCount ) ) )
//	{
//		MEMMOVE( lpLastChr , lpLastChr + 1 , basestr + memSz - lpLastChr - 1 );
//		memSz--;
//	}
//	if ( pSz ) *pSz = memSz;
//	return memSz ? basestr : NULL;
//}
//
//IN_GENERAL void_p serializeChrs( void_p const sMem /*in out*/ , size_t memSz /*in*/ , size_t * const pSz /*out*/ , void_p const chrs /*in*/ , size_t chrsCount /*in*/ ) // 1390/06/03
//{
//	WARNING( pSz );
//	LPSTR  basestr = ( LPSTR  )sMem;
//	LPSTR  lpLastMem = basestr;
//	LPSTR  lpChr = basestr;
//	while ( ( lpChr = ( LPSTR  )memchrs( lpLastMem , memSz , chrs , chrsCount ) ) )
//	{
//		if ( lpChr == lpLastMem )
//		{
//			lpLastMem++;
//			memSz--;
//			continue;
//		}
//		MEMMOVE( lpLastMem , lpChr , lpLastMem + memSz - lpChr );
//		memSz = lpLastMem + memSz - lpChr - 1;
//		lpLastMem++;
//	}
//	memSz = lpLastMem - basestr;
//	if ( pSz ) *pSz = memSz;
//	return memSz ? sMem : NULL;
//}
//
//IN_GENERAL void_p serializeiChrs( void_p const sMem /*in out*/ , size_t memSz /*in*/ , size_t * const pSz /*out*/ , void_p const chrs /*in*/ , size_t chrsCount /*in*/ ) // 1390/06/03
//{
//	WARNING( pSz );
//	LPSTR  basestr = ( LPSTR  )sMem;
//	LPSTR  lpLastMem = basestr;
//	LPSTR  lpChr = basestr;
//	while ( ( lpChr = ( LPSTR  )memichrs( lpLastMem , memSz , chrs , chrsCount ) ) )
//	{
//		if ( lpChr == lpLastMem )
//		{
//			lpLastMem++;
//			memSz--;
//			continue;
//		}
//		MEMMOVE( lpLastMem , lpChr , lpLastMem + memSz - lpChr );
//		memSz = lpLastMem + memSz - lpChr - 1;
//		lpLastMem++;
//	}
//	memSz = lpLastMem - basestr;
//	if ( pSz ) *pSz = memSz;
//	return memSz ? sMem : NULL;
//}

// ---- chr --------------------------------------------------------------------------------

IN_GENERAL LPCSTR strichr( LPCSTR str , char c )
{
	char car[] = { c , EOS };
	return stristr( str , car );
}

IN_GENERAL LPCSTR strnchr( LPCSTR str , char c , int n )
{
	do
	{
		str = strchr( str , c );
	} while ( str && n-- > 0 && str++ );
	return str;
}

IN_GENERAL LPCSTR strinchr( LPCSTR str , char c , int n )
{
	do
	{
		str = strichr( str , c );
	} while ( str && n-- > 0 && str++ );
	return str;
}

//
//IN_GENERAL LPCSTR strrichr( LPCSTR str , char c )
//{
//	char car[] = { c , EOS };
//	return rstristr( str , car );
//}
//
//IN_GENERAL LPCSTR strrnchr( LPCSTR inStr , char c , int n )
//{
//	LPSTR str = ( LPSTR )inStr;
//	LPSTR srcs = str;
//	CHAR eos = EOS;
//	LPSTR ceos = NULL;
//	do
//	{
//		if ( ( str = strrchr( srcs , c ) ) && n >= 0 )
//		{
//			if ( eos != EOS )
//			{
//				::swap( ceos[ 0 ] , eos );
//				WARNING( eos == EOS );
//			}
//			::swap( str[ 0 ] , eos );
//			ceos = str;
//		}
//	} while ( str && n-- > 0 );
//	if ( eos != EOS )
//	{
//		::swap( ceos[ 0 ] , eos );
//		WARNING( eos == EOS );
//	}
//	return str;
//}
//
//IN_GENERAL LPCSTR strrinchr( LPCSTR inStr , char c , int n )
//{
//	LPSTR str = ( LPSTR )inStr;
//	LPSTR srcs = str;
//	CHAR eos = EOS;
//	LPSTR ceos = NULL;
//	do
//	{
//		if ( ( str = ( LPSTR )strrichr( srcs , c ) ) && n >= 0 )
//		{
//			if ( eos != EOS )
//			{
//				::swap( ceos[ 0 ] , eos );
//				WARNING( eos == EOS );
//			}
//			::swap( str[ 0 ] , eos );
//			ceos = str;
//		}
//	} while ( str && n-- > 0 );
//	if ( eos != EOS )
//	{
//		::swap( ceos[ 0 ] , eos );
//		WARNING( eos == EOS );
//	}
//	return str;
//}
//
// ~~~~~ chr --------------------------------------------------------------------------------
// ---- chrs --------------------------------------------------------------------------------
//
//IN_GENERAL LPCSTR strichrs( LPCSTR str , LPCSTR chrs , int * const pCI ) // case insensitive cmp . Written By Mohsen
//{
//	LPCSTR pOutStr = NULL , pTempOutStr;
//	for ( ; chrs[ 0 ] != EOS ; chrs++ )
//	{
//		if ( ( pTempOutStr = ( LPCSTR )strichr( str , chrs[ 0 ] ) ) )
//		{
//			if ( !pOutStr || pTempOutStr < pOutStr )
//			{
//				pOutStr = pTempOutStr;
//			}
//		}
//	}
//	if ( pCI && pOutStr )
//	{
//		*pCI = strchr( chrs , pOutStr[ 0 ] ) - chrs;
//	}
//	return pOutStr;
//}
//IN_GENERAL LPCSTR strnchrs( LPCSTR str , int n , LPCSTR chrs , int * const pCI ) // n(th) chr . Written By Mohsen
//{
//	LPCSTR pOutStr = NULL , pTempOutStr;
//	for ( ; chrs[ 0 ] != EOS ; chrs++ )
//	{
//		if ( ( pTempOutStr = ( LPCSTR )strnchr( str , chrs[ 0 ] , n ) ) )
//		{
//			if ( !pOutStr || pTempOutStr < pOutStr )
//			{
//				pOutStr = pTempOutStr;
//			}
//		}
//	}
//	if ( pCI && pOutStr )
//	{
//		*pCI = strchr( chrs , pOutStr[ 0 ] ) - chrs;
//	}
//	return pOutStr;
//}
//IN_GENERAL LPCSTR strinchrs( LPCSTR str , int n , LPCSTR chrs , int * const pCI ) // n(th) chr . case insensitive cmp . Written By Mohsen
//{
//	LPCSTR pOutStr = NULL , pTempOutStr;
//	for ( ; chrs[ 0 ] != EOS ; chrs++ )
//	{
//		if ( ( pTempOutStr = ( LPCSTR )strinchr( str , chrs[ 0 ] , n ) ) )
//		{
//			if ( !pOutStr || pTempOutStr < pOutStr )
//			{
//				pOutStr = pTempOutStr;
//			}
//		}
//	}
//	if ( pCI && pOutStr )
//	{
//		*pCI = strchr( chrs , pOutStr[ 0 ] ) - chrs;
//	}
//	return pOutStr;
//}
//
//
//
//IN_GENERAL LPCSTR strrichrs( LPCSTR str , LPCSTR chrs , int * const pCI ) // reverse search . case insensitive cmp . Written By Mohsen
//{
//	LPCSTR pOutStr = NULL , pTempOutStr;
//	for ( ; chrs[ 0 ] != EOS ; chrs++ )
//	{
//		if ( ( pTempOutStr = ( LPCSTR )strrichr( str , chrs[ 0 ] ) ) )
//		{
//			if ( !pOutStr || pTempOutStr > pOutStr )
//			{
//				pOutStr = pTempOutStr;
//			}
//		}
//	}
//	if ( pCI && pOutStr )
//	{
//		*pCI = strchr( chrs , pOutStr[ 0 ] ) - chrs;
//	}
//	return pOutStr;
//}
//IN_GENERAL LPCSTR strrnchrs( LPCSTR str , int n , LPCSTR chrs , int * const pCI ) // Written By Mohsen
//{
//	LPCSTR pOutStr = NULL , pTempOutStr;
//	for ( ; chrs[ 0 ] != EOS ; chrs++ )
//	{
//		if ( ( pTempOutStr = ( LPCSTR )strrnchr( str , chrs[ 0 ] , n ) ) )
//		{
//			if ( !pOutStr || pTempOutStr > pOutStr )
//			{
//				pOutStr = pTempOutStr;
//			}
//		}
//	}
//	if ( pCI && pOutStr )
//	{
//		*pCI = strchr( chrs , pOutStr[ 0 ] ) - chrs;
//	}
//	return pOutStr;
//}
//IN_GENERAL LPCSTR strrinchrs( LPCSTR str , int n , LPCSTR chrs , int * const pCI ) // Written By Mohsen
//{
//	LPCSTR pOutStr = NULL , pTempOutStr;
//	for ( ; chrs[ 0 ] != EOS ; chrs++ )
//	{
//		if ( ( pTempOutStr = ( LPCSTR )strrinchr( str , chrs[ 0 ] , n ) ) )
//		{
//			if ( !pOutStr || pTempOutStr > pOutStr )
//			{
//				pOutStr = pTempOutStr;
//			}
//		}
//	}
//	if ( pCI && pOutStr )
//	{
//		*pCI = strchr( chrs , pOutStr[ 0 ] ) - chrs;
//	}
//	return pOutStr;
//}
//
//
//
//IN_GENERAL LPCSTR strchrs( LPCSTR str , LPCSTR chrs , int * const pCI ) // same as strpbrk . Written By Mohsen
//{
//	LPCSTR lpRet = strpbrk( str , chrs );
//	if ( pCI && lpRet )
//	{
//		*pCI = strchr( chrs , lpRet[ 0 ] ) - chrs;
//	}
//	return lpRet;
//}

IN_GENERAL LPCSTR strrchrs( LPCSTR str , LPCSTR chrs , int * const pCI ) // Written By Mohsen
{
	LPCSTR pOutStr = NULL , pTempOutStr;
	for ( ; chrs[ 0 ] != EOS ; chrs++ )
	{
		if ( ( pTempOutStr = ( LPCSTR )strrchr( str , chrs[ 0 ] ) ) )
		{
			if ( !pOutStr || pTempOutStr > pOutStr )
			{
				pOutStr = pTempOutStr;
			}
		}
	}
	if ( pCI && pOutStr )
	{
		*pCI = ( int )( strchr( chrs , pOutStr[ 0 ] ) - chrs );
	}
	return pOutStr;
}

LPCSTR strihead( LPCSTR str , LPCSTR head )
{
	return stristr( str , head ) == str ? str : NULL;
}

//status sendall( sockfd socketfd , buffer buf , size_t * len ) // as beej book says
//{
//	size_t total = 0; // how many bytes we've snt
//	size_t byteleft = *len; // how many we have left to send
//	ssize_t send_ret;
//
//	while ( total < *len )
//	{
//		send_ret = send( socketfd , buf + total , byteleft , MSG_NOSIGNAL );
//		if ( send_ret == EPIPE )
//		{
//			return errPeerClosed;
//		}
//		if ( send_ret == -1 ) { break; }
//		total += ( size_t )send_ret;
//		byteleft -= ( size_t )send_ret;
//	}
//	*len  = total; // return number actually sent
//	return byteleft == 0 ? errOK : errCanceled; // return -1 on failure, 0 on success
//}

status tcp_send_all( int fd , const void * buf , size_t len , int flags , int pool_timeout_onsend_ms , int timeout_onack_ms ,
	int retry_count_on_timeout/*0 no retry*/ , Brief_Err * imortalErrStr , DtsErrBuf detailErrBuf )
{
	const unsigned char * p = ( const unsigned char * )buf;
	size_t remaining = len;
	size_t total_sent = 0;

	/* Add MSG_NOSIGNAL if available to avoid SIGPIPE. */
#ifdef MSG_NOSIGNAL
	int send_flags = flags | MSG_NOSIGNAL; // Prevents SIGPIPE if peer closed connection.
#else
	int send_flags = flags;
#endif

	while ( remaining > 0 )
	{
		ssize_t n = send( fd , p , remaining , send_flags ); // send() copies data only up to the amount of space available in the TCP buffer at that layer.
		if ( n > 0 )
		{
			/* Successfully sent n bytes; advance pointer and continue. */
			p += n;
			remaining -= ( size_t )n; // never happened that n greater that ordered size
			total_sent += ( size_t )n;
			continue;
		}

		if ( n == 0 )
		{
			/*
			 * send() returning 0 on a TCP socket is unusual. Treat as error:
			 * peer likely closed or OS-level oddity. Set errno to ECONNRESET.
			 */
			if ( detailErrBuf )
			{
				IGNORE_RESULT char * p = strerror_r( errno , *detailErrBuf , sizeof(*detailErrBuf) );
			}
			errno = ECONNRESET;
			STORE_BRIEF_ERR( imortalErrStr , "PeerClosed\n" , true );
			return errPeerClosed;
		}

		/* n < 0: error occurred */
		int saved_errno = errno;

		if ( saved_errno == EINTR )
		{
			/* Interrupted by signal; retry immediately. */
			continue;
		}

		if ( saved_errno == EAGAIN || saved_errno == EWOULDBLOCK )
		{
			/* Socket does not have storage to store my data so wait for it to become writable using poll(). */
			struct pollfd pfd;
			pfd.fd = fd;
			pfd.events = POLLOUT;
			pfd.revents = 0;

			int poll_timeout = pool_timeout_onsend_ms; /* in milliseconds; negative => infinite */
			int poll_ret;

			if ( poll_timeout < 0 )
			{
				/* Wait indefinitely. */
				poll_ret = poll( &pfd , 1 , -1 );
			}
			else
			{
				poll_ret = poll( &pfd , 1 , poll_timeout );
			}

			if ( poll_ret > 0 )
			{
				/* Check for errors on socket */
				if ( pfd.revents & ( POLLERR | POLLHUP | POLLNVAL ) )
				{
					/* Peer closed or error on socket. We surface as ECONNRESET. */
					errno = ECONNRESET;
					STORE_BRIEF_ERR( imortalErrStr , "PeerClosed\n" , true );
					return errPeerClosed;
				}
				/* Writable: loop will retry send() */
				continue;
			}
			else if ( poll_ret == 0 )
			{
				/* Timeout */
				if ( retry_count_on_timeout > 0 && remaining > 0 )
				{
					return tcp_send_all( fd , p , remaining , flags , pool_timeout_onsend_ms , timeout_onack_ms , retry_count_on_timeout - 1 , imortalErrStr , detailErrBuf );
				}
				STORE_BRIEF_ERR( imortalErrStr , "Timeout\n" , true );
				errno = ETIMEDOUT;
				return errTimeout;
			}
			else
			{
				/* poll() itself failed */
				STORE_BRIEF_ERR( imortalErrStr , "poll()\n" , true );
				if ( detailErrBuf )
				{
					IGNORE_RESULT char * p = strerror_r( errno , *detailErrBuf , sizeof(*detailErrBuf) );
				}
				return errSocket; /* errno set by poll */
			}
		}

		if ( saved_errno == EPIPE || saved_errno == ECONNRESET )
		{
			STORE_BRIEF_ERR( imortalErrStr , "PeerClosed\n" , true );
			return errPeerClosed;
		}

		/* For other errno values (EPIPE, ECONNRESET, etc.) return error. */
		/* If EPIPE occurs, MSG_NOSIGNAL prevents SIGPIPE; caller gets EPIPE. */
		errno = saved_errno;
		return errGeneral;
	}
	if ( total_sent >= len )
	{
	#ifdef ENABLE_WAIT_FOR_PACKET_ACK
		if ( wait_for_ack( fd , total_sent , timeout_onack_ms ) == errOK )
		{
			return errOK;
		}
		return errACK;
	#endif
		return errOK;
	}
	STORE_BRIEF_ERR( imortalErrStr , "send != request\n" , true );
	return errGeneral; /* should equal len */
}

status string_to_int( LPCSTR str , int * out )
{
	LPSTR  endptr;
	long val;

	errno = 0;  // reset errno before call
	val = strtol( str , &endptr , 10 ); // base 10

	// Check for conversion errors
	if ( errno == ERANGE || val > INT_MAX || val < INT_MIN )
	{
		return errGeneral; // overflow/underflow
	}

	if ( endptr == str )
	{
		return errGeneral; // no digits were found
	}

	// Check for any extra non-digit characters
	while ( *endptr != '\0' )
	{
		if ( *endptr != ' ' && *endptr != '\t' && *endptr != '\n' )
		{
			return errGeneral; // invalid character in string
		}
		endptr++;
	}

	*out = ( int )val;
	return errOK; // success
}

void buff_fill_seq( buffer buf , size_t size )
{
	if ( !buf || size == 0 ) return;

	int num = 1;
	for ( size_t i = 0; i < size; i++ )
	{
		buf[ i ] = ( uchar )( '0' + num );   // store as character '1'..'9'
		num++;
		if ( num > 9 ) num = 1; // wrap back to 1
	}
}

void dump_buffer( const buffer buff , size_t size )
{
	const unsigned char * buf = ( const unsigned char * )buff;
	size_t i;

	for ( i = 0; i < size; i += 16 )
	{
		printf( "%08zx  " , i );

		// Print hex part
		for ( size_t j = 0; j < 16; j++ )
		{
			if ( i + j < size )
				printf( "%02x " , buf[ i + j ] );
			else
				printf( "   " );
		}

		printf( " " );

		// Print ASCII part
		for ( size_t j = 0; j < 16 && i + j < size; j++ )
		{
			unsigned char c = buf[ i + j ];
			printf( "%c" , isprint( c ) ? c : '.' );
		}

		printf( "\n" );
	}
}

/*
 * Dump binary buffer into a readable ASCII string.
 * Printable ASCII chars => copy directly.
 * Non-printable chars   => write as \xHH.
 *
 * out_size must include space for the terminating '\0'.
 * Returns number of bytes written (not including '\0').
 */
size_t dump_buffer_ascii( buffer in, size_t in_len, char *out, size_t out_size )
{
    size_t oi = 0;  // output index

    for (size_t i = 0; i < in_len; i++) {
        unsigned char c = in[i];

        if (isprint(c)) {
            // Writes 1 byte if space available
            if (oi + 1 >= out_size)
                break;
            out[oi++] = (char)c;
        } else {
            // Non-printable → "\xHH" (4 chars)
            // Check space for 4 chars
            if (oi + 4 >= out_size)
                break;

            // Write hex escape
            out[oi++] = '\\';
            out[oi++] = 'x';
            out[oi++] = "0123456789ABCDEF"[c >> 4];
            out[oi++] = "0123456789ABCDEF"[c & 0x0F];
        }
    }

    // Null terminate
    if (out_size > 0)
        out[oi < out_size ? oi : out_size - 1] = '\0';

    return oi;
}

int peerTcpClosed( sockfd socketfd )
{
	char c;
	ssize_t n = recv( socketfd , &c , 1 , MSG_PEEK | MSG_DONTWAIT );
	if ( n == 0 )
	{
		return 1;
	}
	return 0;
}

int strsstr( LPCSTR * strs , int strs_count , LPCSTR target )
{
	for ( int i = 0; i < strs_count; i++ )
	{
		if ( STRCMP( target , strs[ i ] ) == 0 )
		{
			return i;  // found at index i
		}
	}
	return -1;  // not found
}

int strsistr( LPCSTR * strs , int strs_count , LPCSTR target )
{
	for ( int i = 0; i < strs_count; i++ )
	{
		if ( STRICMP( target , strs[ i ] ) == 0 )
		{
			return i;  // found at index i
		}
	}
	return -1;  // not found
}

// Option A: 64-bit FNV-1a accumulator + SplitMix64-style avalanche
uint8 hash8_fnv1a_avalanche( const char * s )
{
	uint64_t x = 1469598103934665603ULL;          // FNV-1a offset
	const uint64_t prime = 1099511628211ULL;      // FNV-1a prime
	for ( const unsigned char * p = ( const unsigned char * )s; *p; ++p )
	{
		x ^= *p;
		x *= prime;
	}
	// Strong 64-bit avalanche (SplitMix64 finalizer)
	x ^= x >> 30;
	x *= 0xbf58476d1ce4e5b9ULL;
	x ^= x >> 27;
	x *= 0x94d049bb133111ebULL;
	x ^= x >> 31;
	return ( uint8 )x; // take any byte after avalanche
}

uint8 hash8_fnv1a_avalanche_l( long l )
{
	char ar[9] = {0};
	*( ( long * ) ar ) = l;
	return hash8_fnv1a_avalanche( ar );
}

uint64 hash64_fnv1a_avalanche( const char * s )
{
	const uint64 FNV_OFFSET_BASIS = 14695981039346656037ULL;
	const uint64 FNV_PRIME = 1099511628211ULL;
	uint64 h = FNV_OFFSET_BASIS;
	const unsigned char * p = ( const unsigned char * )s;
	while ( *p )
	{
		h ^= ( uint64 )( *p++ );
		h *= FNV_PRIME;
	}
	// final avalanche (optional)
	h ^= ( h >> 33 );
	h *= 0xff51afd7ed558ccdULL;
	h ^= ( h >> 33 );
	return h;
}

// Simple string hash function (djb2)
ulong hash( LPCSTR str )
{
	ulong h = 5381;
	uchar c;
	while ( ( c = ( uchar )*str++ ) )
		h = ( ( h << 5 ) + h ) + c; // h * 33 + c
	return h;
}

const char * get_filename( const char * path )
{
	const char * slash = strrchrs( path , "/\\" , NULL );  // find last '/'
	if ( slash )
		return slash + 1;  // return part after last '/'
	return path;  // no '/' found, whole string is filename
}

status sem_wait_with_timeout( sem_t * sem , long timeout_sec , volatile bool * app_closed )
{
	if ( !sem || timeout_sec < 0 )
		return errArg;

	struct timespec start , now;
	if ( clock_gettime( CLOCK_MONOTONIC , &start ) != 0 )
		return errSystem;

	long elapsed = 0;

	while ( elapsed < timeout_sec )
	{
		if ( app_closed && *app_closed )
			return errShutdown;

		// Set up 1-second absolute timeout from current time
		struct timespec ts;
		if ( clock_gettime( CLOCK_REALTIME , &ts ) != 0 )
			return errSystem;
		ts.tv_sec += 1;

		int res = sem_timedwait( sem , &ts );

		if ( res == 0 )
			return errOK;

		if ( errno == ETIMEDOUT )
		{
			// Continue if still within overall timeout
			if ( clock_gettime( CLOCK_MONOTONIC , &now ) != 0 )
				return errSystem;

			elapsed = ( long )( now.tv_sec - start.tv_sec );
			continue;
		}
		else if ( errno == EINTR )
		{
			// Interrupted, check again
			continue;
		}
		else
		{
			int val = 0;
			if ( sem_getvalue( sem , &val ) < 0 || val < 1 ) return errSystem;
			return errGeneral;
		}
	}

	return errTimeout;
}

int timeval_compare( const struct timeval * a , const struct timeval * b )
{
	if ( a->tv_sec < b->tv_sec )
		return -1; // a low prio than b
	else if ( a->tv_sec > b->tv_sec )
		return 1;
	else if ( a->tv_usec < b->tv_usec )
		return -1;
	else if ( a->tv_usec > b->tv_usec )
		return 1;
	else
		return 0;
}

/*be careful about order of input items*/
int regression_slope_int( const uint64 * y , size_t n )
{
	double sum_x = 0 , sum_y = 0 , sum_xy = 0 , sum_x2 = 0;
	double slope;

	for ( size_t i = 0; i < n; i++ )
	{
		double x = (double)(i + 1); // x = 1..n
		sum_x += (double)(x);
		sum_y += (double)(y[ i ]);
		sum_xy += x * (double)(y[ i ]);
		sum_x2 += (double)(x * x);
	}

	slope = ( (double)(n) * sum_xy - sum_x * sum_y ) / ( (double)(n) * sum_x2 - sum_x * sum_x );

	// Return slope as integer (rounded)
	return ( int )( slope + ( slope >= 0 ? 0.5 : -0.5 ) );
}

void enable_keepalive_chaotic( int sock , Brief_Err * imortalErrStr )
{
	srand( ( unsigned )time( NULL ) ^ ( unsigned )sock );

	int yes = 1;
	KERNEL_CALL_NORET( setsockopt( sock , SOL_SOCKET , SO_KEEPALIVE , &yes , sizeof( yes ) ) == -1 , "setsockopt()" , imortalErrStr , true );

	// Prime base values with random jitter ±3s
	int idle = 29 + ( rand() % 7 - 3 );  // 26–32 s
	int interval = 7 + ( rand() % 3 - 1 );  // 6–8 s
	int count = 5;                      // prime and stable

	if ( idle < 10 ) idle = 10;
	if ( interval < 3 ) interval = 3;

	KERNEL_CALL_NORET( setsockopt( sock , IPPROTO_TCP , TCP_KEEPIDLE , &idle , sizeof( idle ) ) == -1 , "setsockopt()" , imortalErrStr , true );
	KERNEL_CALL_NORET( setsockopt( sock , IPPROTO_TCP , TCP_KEEPINTVL , &interval , sizeof( interval ) ) == -1 , "setsockopt()" , imortalErrStr , true );
	KERNEL_CALL_NORET( setsockopt( sock , IPPROTO_TCP , TCP_KEEPCNT , &count , sizeof( count ) ) == -1 , "setsockopt()" , imortalErrStr , true );
}

void enable_keepalive( sockfd sock , Brief_Err * imortalErrStr )
{
	int yes = 1;
	KERNEL_CALL_NORET( setsockopt( sock , SOL_SOCKET , SO_KEEPALIVE , &yes , sizeof( yes ) ) == -1 , "setsockopt()" , imortalErrStr , true );

	int idle = 30;  // seconds of inactivity before keepalive probes start
	int interval = 10;  // seconds between probes
	int count = 5;   // number of failed probes before marking dead

	KERNEL_CALL_NORET( setsockopt( sock , IPPROTO_TCP , TCP_KEEPIDLE , &idle , sizeof( idle ) ) == -1 , "setsockopt()" , imortalErrStr , true );
	KERNEL_CALL_NORET( setsockopt( sock , IPPROTO_TCP , TCP_KEEPINTVL , &interval , sizeof( interval ) ) == -1 , "setsockopt()" , imortalErrStr , true );
	KERNEL_CALL_NORET( setsockopt( sock , IPPROTO_TCP , TCP_KEEPCNT , &count , sizeof( count ) ) == -1 , "setsockopt()" , imortalErrStr , true );
}

// Returns:
//  1 = socket looks connected (no EOF detected, no immediate error)
//  0 = socket closed by peer (recv returned 0) or error that indicates closure
// -1 = indeterminate (poll error) but errno will be set
int peek_socket_connection( int fd , int timeout_ms , Brief_Err * imortalErrStr )
{
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN | POLLERR | POLLHUP;
	pfd.revents = 0;

	int rv = poll( &pfd , 1 , timeout_ms );
	if ( rv < 0 )
	{
		STORE_BRIEF_ERR( imortalErrStr , "poll()\n" , true );
		return -1; // errno set
	}
	if ( rv == 0 )
	{
		// no event within timeout -> assume still connected (no proof of closure)
		return 1;
	}

	// If socket is readable, peek a byte: recv() == 0 => orderly shutdown.
	if ( pfd.revents & POLLIN )
	{
		char buf;
		ssize_t n = recv( fd , &buf , 1 , MSG_PEEK | MSG_DONTWAIT );
		if ( n == 0 ) return 0;         // connection closed by peer
		if ( n > 0 ) return 1;          // data available, socket alive
		// n < 0 -> error
		if ( errno == EAGAIN || errno == EWOULDBLOCK ) return 1;
		return 0;
	}

	// HUP or ERR means closure or error
	if ( pfd.revents & ( POLLHUP | POLLERR | POLLNVAL ) ) return 0;
	return 1;
}

status pthread_mutex_timedlock_rel( pthread_mutex_t * mutex , long ms )
{
	struct timespec ts;
	clock_gettime( CLOCK_REALTIME , &ts );

	ts.tv_sec += ms / 1000;
	ts.tv_nsec += ( ms % 1000 ) * 1000000;
	if ( ts.tv_nsec >= 1000000000 )
	{
		ts.tv_sec++;
		ts.tv_nsec -= 1000000000;
	}
	if ( pthread_mutex_timedlock( mutex , &ts ) == ETIMEDOUT )
	{
		return errTimeout;
	}
	return errOK;
}

long parse_and_extract_file_name_value( LPCSTR filename , LPCSTR ignore_part )
{
	const char * start = strstr( filename , ignore_part );  // find the pattern
	long number = 0;
	if ( start )
	{
		start += strlen( ignore_part );  // move to start of number
		number = strtol( start , NULL , 10 );  // parse as long
	}
	return number;
}

status connect_with_timeout( const char * ip , int port , int timeout_sec , sockfd * conn_sock ,
	Brief_Err * imortalErrStr , DtsErrBuf detailErrBuf )
{
	int sockfd;
	struct sockaddr_in addr;
	int flags , result , valopt = 0;
	fd_set wset;
	struct timeval tv;
	socklen_t lon;

	// Create socket
	sockfd = socket( AF_INET , SOCK_STREAM , 0 );
	if ( sockfd < 0 )
	{
		STORE_BRIEF_ERR( imortalErrStr , "socket()\n" , true );
		if ( detailErrBuf )
		{
			IGNORE_RESULT char * p = strerror_r( errno , *detailErrBuf , sizeof(*detailErrBuf) );
		}
		return errSocket;
	}

	//// Make socket non-blocking
	flags = fcntl( sockfd , F_GETFL , 0 );
	fcntl( sockfd , F_SETFL , flags | O_NONBLOCK );

	// Set address
	memset( &addr , 0 , sizeof( addr ) );
	addr.sin_family = AF_INET;
	addr.sin_port = htons( (uint16_t)port );
	//inet_pton( AF_INET , ip , &addr.sin_addr );
	addr.sin_addr.s_addr = inet_addr( ip );

	// Start connecting (non-blocking)
	result = connect( sockfd , ( struct sockaddr * )&addr , sizeof( addr ) );
	if ( result < 0 )
	{
		if ( errno == EINPROGRESS )
		{
			// Connection in progress
			FD_ZERO( &wset );
			FD_SET( sockfd , &wset );
			tv.tv_sec = timeout_sec;
			tv.tv_usec = 0;

			// Wait for socket to become writable (success) or error
			result = select( sockfd + 1 , NULL , &wset , NULL , &tv );
			if ( result > 0 )
			{
				lon = sizeof( int );
				getsockopt( sockfd , SOL_SOCKET , SO_ERROR , ( void * )( &valopt ) , &lon );
				if ( valopt )
				{
					if ( detailErrBuf )
					{
						IGNORE_RESULT char * p = strerror_r( valopt , *detailErrBuf , sizeof(*detailErrBuf) );
					}
					errno = valopt;
					STORE_BRIEF_ERR( imortalErrStr , "connect()\n" , true );
					close( sockfd );
					return errNoConnection;
				}
			}
			else if ( result == 0 )
			{
				STORE_BRIEF_ERR( imortalErrStr , "timeout\n", true );
				close( sockfd );
				errno = ETIMEDOUT;
				return errTimeout;
			}
			else
			{
				STORE_BRIEF_ERR( imortalErrStr , "select()\n" , true );
				if ( detailErrBuf )
				{
					IGNORE_RESULT char * p = strerror_r( errno , *detailErrBuf , sizeof(*detailErrBuf) );
				}
				close( sockfd );
				return errSelect;
			}
		}
		else
		{
			STORE_BRIEF_ERR( imortalErrStr , "connect()\n" , true );
			if ( detailErrBuf )
			{
				IGNORE_RESULT char * p = strerror_r( errno , *detailErrBuf , sizeof(*detailErrBuf) );
			}
			close( sockfd );
			return errConnect;
		}
	}

	//// Restore blocking mode
	fcntl( sockfd , F_SETFL , flags );
	*conn_sock = sockfd;
	return errOK;  // Success
}

// General function to handle socket creation, binding, listening, and accepting with timeout
status create_server_socket_with_timeout( const char * ip_address , int port , int timeout_sec , sockfd * client_fd ,
	Brief_Err * imortalErrStr , DtsErrBuf detailErrBuf )
{
	int server_fd;
	fd_set read_fds;
	struct timeval timeout;

	// Create socket
	if ( ( server_fd = socket( AF_INET , SOCK_STREAM , 0 ) ) == -1 )
	{
		STORE_BRIEF_ERR( imortalErrStr , "socket()\n" , true );
		if ( detailErrBuf )
		{
			IGNORE_RESULT char * p = strerror_r( errno , *detailErrBuf , sizeof(*detailErrBuf) );
		}
		return errSocket;
	}
	int prevflags = fcntl( server_fd , F_GETFL , 0 );
	if ( prevflags == -1 )
	{
		STORE_BRIEF_ERR( imortalErrStr , "fcntl(F_GETFL) failed.\n" , true );
		if ( detailErrBuf )
		{
			IGNORE_RESULT char * p = strerror_r( errno , *detailErrBuf , sizeof(*detailErrBuf) );
		}
		_close_socket( &server_fd , NULL );
		return errsockopt;
	}
	if ( fcntl( server_fd , F_SETFL , prevflags | O_NONBLOCK ) == -1 )
	{
		STORE_BRIEF_ERR( imortalErrStr , "fcntl(O_NONBLOCK) failed.\n" , true );
		if ( detailErrBuf )
		{
			IGNORE_RESULT char * p = strerror_r( errno , *detailErrBuf , sizeof(*detailErrBuf) );
		}
		_close_socket( &server_fd , NULL );
		return errsockopt;
	}
	int opt = 1;
	if ( setsockopt( server_fd, SOL_SOCKET , SO_REUSEADDR , &opt , sizeof( opt ) ) < 0 )
	{
		STORE_BRIEF_ERR( imortalErrStr , "setsockopt(SO_REUSEADDR)\n" , true );
		if ( detailErrBuf )
		{
			IGNORE_RESULT char * p = strerror_r( errno , *detailErrBuf , sizeof(*detailErrBuf) );
		}
		_close_socket( &server_fd , NULL );
		return errsockopt;
	}
	if ( setsockopt( server_fd , SOL_SOCKET , SO_REUSEPORT , &opt , sizeof( opt ) ) < 0 ) // Let server restart immediately without waiting TIME_WAIT.
	{
		STORE_BRIEF_ERR( imortalErrStr , "setsockopt(SO_REUSEPORT)\n" , true );
		if ( detailErrBuf )
		{
			IGNORE_RESULT char * p = strerror_r( errno , *detailErrBuf , sizeof(*detailErrBuf) );
		}
		_close_socket( &server_fd , NULL );
		return errsockopt;
	}
	if ( setsockopt( server_fd , IPPROTO_TCP , TCP_QUICKACK , &opt , sizeof( opt ) ) < 0 ) // Force immediate ACK. Decreases feedback latency. Disable delayed ACK at receiver
	{
		STORE_BRIEF_ERR( imortalErrStr , "setsockopt(TCP_QUICKACK)\n" , true );
		if ( detailErrBuf )
		{
			IGNORE_RESULT char * p = strerror_r( errno , *detailErrBuf , sizeof(*detailErrBuf) );
		}
		_close_socket( &server_fd , NULL );
		return errsockopt;
	}

	struct sockaddr_in server_addr;
	socklen_t addrlen = sizeof( server_addr );
	memset( &server_addr , 0 , sizeof( server_addr ) );
	server_addr.sin_family = AF_INET;
	if ( strcmp( ip_address , "INADDR_ANY" ) == 0 )
	{
		server_addr.sin_addr.s_addr = INADDR_ANY;
	}
	else
	{
		server_addr.sin_addr.s_addr = inet_addr( ip_address ); // Specify the IP address to bind to
	}
	server_addr.sin_port = htons( (uint16_t)port );

	// Bind the socket
	if ( bind( server_fd , ( struct sockaddr * )&server_addr , addrlen ) == -1 )
	{
		if ( errno == EADDRINUSE )
		{
			STORE_BRIEF_ERR( imortalErrStr , "bind(ADDRINUSE)-multiple instance\n" , true );
		}
		else
		{
			STORE_BRIEF_ERR( imortalErrStr , "bind()\n" , true );
			if ( detailErrBuf )
			{
				IGNORE_RESULT char * p = strerror_r( errno , *detailErrBuf , sizeof(*detailErrBuf) );
			}
		}
		close( server_fd );
		return errBind;
	}
	// Prepare for listen
	if ( listen( server_fd , 5 ) == -1 )
	{
		STORE_BRIEF_ERR( imortalErrStr , "listen()\n" , true );
		if ( detailErrBuf )
		{
			IGNORE_RESULT char * p = strerror_r( errno , *detailErrBuf , sizeof(*detailErrBuf) );
		}
		close( server_fd );
		return errListen;
	}

	// Set up timeout value for select()
	timeout.tv_sec = timeout_sec;
	timeout.tv_usec = 0;

	// Set up the file descriptor set
	FD_ZERO( &read_fds );
	FD_SET( server_fd , &read_fds );

	// Wait for incoming connection with timeout using select()
	int activity = select( server_fd + 1 , &read_fds , NULL , NULL , &timeout );

	if ( activity == -1 )
	{
		STORE_BRIEF_ERR( imortalErrStr , "select()\n" , true );
		if ( detailErrBuf )
		{
			IGNORE_RESULT char * p = strerror_r( errno , *detailErrBuf , sizeof(*detailErrBuf) );
		}
		close( server_fd );
		return errSelect;
	}
	else if ( activity == 0 )
	{
		STORE_BRIEF_ERR( imortalErrStr , "Timeout reached.\n" , true );
		close( server_fd );
		return errTimeout;
	}

	// Accept the connection if it's ready
	if ( FD_ISSET( server_fd , &read_fds ) )
	{
		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof( client_addr );

		*client_fd = accept( server_fd , ( struct sockaddr * )&client_addr , &client_len );
		if ( (*client_fd) < 0 )
		{
			STORE_BRIEF_ERR( imortalErrStr , "accept()\n" , true );
			if ( detailErrBuf )
			{
				IGNORE_RESULT char * p = strerror_r( errno , *detailErrBuf , sizeof(*detailErrBuf) );
			}
			close( server_fd );
			return errAccept;
		}

		// Connection accepted
		close( server_fd );
		return errOK;
	}

	close( server_fd );
	STORE_BRIEF_ERR( imortalErrStr , "NoConnection.\n" , true );
	return errNoConnection;  // Timeout or error occurred
}

// Wait until all sent bytes are ACKed by peer.
status wait_for_ack( int sock , size_t sent_bytes /*Forward compatibility*/ , int timeout_ms )
{
	struct tcp_info info;
	socklen_t len = sizeof( info );

	// Get start time for timeout
	struct timeval start , now;
	gettimeofday( &start , NULL );

	for (;;)
	{
		// query TCP_INFO
		if ( getsockopt( sock , SOL_TCP , TCP_INFO , &info , &len ) < 0 )
			return errsockopt;

		// For older kernels:
		// tcpi_unacked = # of unacknowledged packets
		if ( info.tcpi_unacked == 0 )
			return errOK; // all data ACKed

		// Check timeout
		gettimeofday( &now , NULL );
		int elapsed_ms = (int)(( now.tv_sec - start.tv_sec ) * 1000 +
			( now.tv_usec - start.tv_usec ) / 1000);

		if ( elapsed_ms >= timeout_ms )
			break;  // timed out

		// small sleep to avoid busy looping
		usleep( 100 );
	}
	return errTimeout;  // timed out
}

bool is_double_zero( double value )
{
	return value < EPSILON_ZERO && value > -EPSILON_ZERO;
}

IMMORTAL_LPCSTR signal_to_string( int sig )
{
	const char * s = strsignal( sig );
	static char buf[10];
	sprintf( buf , "Unknown signal %d" , sig );
	return ( s != NULL ) ? s : "Unknown signal";
}

void prevent_duplicate_program_execution()
{
	char exe_path[ PATH_MAX ];
	char dir_path[ PATH_MAX ];
	char lock_path[ PATH_MAX ];

	/* Get absolute path of executable */
	ssize_t len = readlink( "/proc/self/exe" , exe_path , sizeof( exe_path ) - 1 );
	if ( len < 0 )
	{
		perror( "readlink" );
		return;
	}
	exe_path[ len ] = '\0';

	/* dirname() and basename() may modify input */
	strncpy( dir_path , exe_path , sizeof( dir_path ) );
	char * dir = dirname( dir_path );
	char * base = basename( exe_path );

	/* Build lock file path: same dir, same name, different extension */
	snprintf( lock_path , sizeof( lock_path ) , "%s/%s.lock" , dir , base );

	int fd = open( lock_path , O_RDWR | O_CREAT , 0644 );
	if ( fd < 0 )
	{
		perror( "open lock file" );
		sleep( 2 );
		_exit( 0 );
		return;
	}

	if ( flock( fd , LOCK_EX | LOCK_NB ) < 0 )
	{
		if ( errno == EWOULDBLOCK )
		{
			fprintf( stderr , "\n\nAnother instance already running (%s)\n" , lock_path );
			sleep( 2 );
			_exit( 0 );
		}
		else
		{
			perror( "flock" );
			sleep( 2 );
			_exit( 0 );
		}
		return;
	}
}

/*
 * Convert a fixed-size binary buffer into a printable, null-terminated string.
 *
 * in        : input buffer (can contain '\0')
 * in_len    : number of bytes to read from input
 * out       : output buffer
 * out_len   : size of output buffer
 *
 * Notes:
 * - Output is always null-terminated (if out_len > 0)
 * - At most out_len - 1 characters are written
 * - Non-printable characters are replaced with '.'
 */
void buffer_to_printable_string( const unsigned char *in , size_t in_len , char *out , size_t out_len )
{
    size_t i, max;

    if (!out || !out_len)
        return;

    /* Leave space for null terminator */
    max = (in_len < out_len - 1) ? in_len : out_len - 1;

    for (i = 0; i < max; i++) {
        /* isprint() excludes control chars and '\0' */
        if (isprint(in[i])) {
            out[i] = (char)in[i];
        } else {
            out[i] = '.';   /* replacement for non-printable */
        }
    }

    out[i] = '\0';
}
