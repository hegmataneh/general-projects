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
#define Uses_sprintf_s
#define Uses_strcpy_s
#define Uses_errno
#define Uses_size_t
#define Uses_strcasestr

#include <general.dep>


static short _err = NEXT_GENERAL_ERROR_VALUE;
static LPCSTR errStrs[64]={"errOK","errGeneral","errMemoryLow","errInvalidString","errCanceled","syntax error","invalid argument","timed out",\
	"peer closed","OutofRanje","MaximumExceeded","NoPeer","NotFound"};


//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

//-------------------------------------------------------------------------
const char EOS = '\0'; // 0
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


//_WEAK_ATTR void M_showMsg( LPCSTR msg )
//{
//
//}

LPSTR newStr( LPCSTR str )
{
	if(!str)str="";
	if(!str[0])return "";
	size_t sz=strlen(str)+1;
	LPSTR temp=MALLOC_AR(temp,sz);
	if (temp) strcpy(temp,str);
	return temp;
}

//LPCSTR __msg( LPSTR  msg_holder , size_t size_of_msg_holder , LPCSTR msg , int line_number )
//{
//	snprintf( msg_holder , size_of_msg_holder , "%s: ln(%d)\n" , msg , line_number );
//	return msg_holder;
//}

LPCSTR make_msg_appnd_sys_err( LPSTR  msg_holder , size_t size_of_msg_holder , LPCSTR cst_msg )
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

LPCSTR __snprintf( LPSTR  msg_holder , size_t size_of_msg_holder , LPCSTR format , ... )
{
	va_list args;

	va_start( args , format );
	vsnprintf( msg_holder , size_of_msg_holder , format , args );
	va_end( args );

	return msg_holder;
}

void _close_socket( sockfd * socket_id )
{
	close( *socket_id );
	*socket_id = -1;
}

LPCSTR read_file( LPCSTR path , LPSTR  pInBuffer /*= NULL*/ )
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
	LPSTR  buffer = pInBuffer ? pInBuffer : malloc( (size_t)(len + 1) );

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

	return ( LPCSTR )buffer;
}

LPCSTR trim_trailing_zeros(LPSTR s) {
    LPSTR dot = strchr(s, '.');
    if (!dot) return s;

    LPSTR end = s + strlen(s) - 1;
    while (end > dot && *end == '0') {
        *end = '\0';
        end--;
    }

    if (*end == '.') {
        *end = '\0';
    }
	return s;
}

LPCSTR format_pps( LPSTR  buf , size_t buflen , ubigint pps , int number_of_float , LPCSTR unit_name /*= "pps"*/ )
{
	LPCSTR units[] = { "", "K", "M", "G", "T", "P" };
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

FILE * create_unique_file( LPCSTR path , LPCSTR filename /*=NULL(app+date)*/ )
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
	size_t len = strlen( buffer );
	if ( len > 0 && buffer[ len - 1 ] == ' ' )
		buffer[ len - 1 ] = '\0';
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

IN_GENERAL LPCSTR rstrstr( LPCSTR sStr , LPCSTR sSubStr ) // Written By Mohsen
{
	LPCSTR lpLastSubStr = NULL;
	while ( ( sStr = strstr( sStr , sSubStr ) ) )
	{
		lpLastSubStr = sStr;
		sStr++;
	}
	return lpLastSubStr;
}

IN_GENERAL LPCSTR rstristr( LPCSTR sStr , LPCSTR sSubStr ) // Written By Mohsen
{
	LPCSTR lpLastSubStr = NULL;
	while ( ( sStr = stristr( sStr , sSubStr ) ) )
	{
		lpLastSubStr = sStr;
		sStr++;
	}
	return lpLastSubStr;
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
	size_t t1 = strlen( from );
	ASSERT( t1 == strlen( to ) );
	LPSTR  lpC;
	while ( ( lpC = strpbrk( ( LPSTR )str , from ) ) )
	{
		*lpC = to[ ( LPSTR  )memchr( from , *lpC , t1 ) - from ];
	}
}

IN_GENERAL void replaceChr( char fromChar , char toChar , LPCSTR str , size_t sz ) // Written By Mohsen
{
	if ( sz == -1 ) sz = strlen( str );
	LPSTR  lpHead;
	while ( ( lpHead = ( LPSTR  )memchr( str , fromChar , sz ) ) )
	{
		*lpHead = toChar;
	}
}


//IN_GENERAL void_p removeChr( void_p const str /*in out*/ , char chr , int sz /*in*/ , int * const pSz /*out*/ ) // 1389/11/19
//{
//	ASSERT( pSz );
//	LPSTR  basestr = ( LPSTR  )str;
//	LPSTR  lpLastChr = basestr;
//	while ( ( lpLastChr = ( LPSTR  )memchr( lpLastChr , chr , basestr + sz - lpLastChr ) ) )
//	{
//		memmove( lpLastChr , lpLastChr + 1 , basestr + sz - lpLastChr - 1 );
//		sz--;
//	}
//	if ( pSz ) *pSz = sz;
//	return sz ? basestr : NULL;
//}

//IN_GENERAL void_p removeiChr( void_p const str /*in out*/ , char chr , int sz /*in*/ , int * const pSz /*out*/ ) // 1389/11/19
//{
//	ASSERT( pSz );
//	LPSTR  basestr = ( LPSTR  )str;
//	LPSTR  lpLastChr = basestr;
//	while ( ( lpLastChr = ( LPSTR  )memichr( lpLastChr , chr , basestr + sz - lpLastChr ) ) )
//	{
//		memmove( lpLastChr , lpLastChr + 1 , basestr + sz - lpLastChr - 1 );
//		sz--;
//	}
//	if ( pSz ) *pSz = sz;
//	return sz ? basestr : NULL;
//}
//
//IN_GENERAL void_p removeChrs( void_p const sMem /*in out*/ , size_t memSz /*in*/ , size_t * const pSz /*out*/ , void_p const chrs /*in*/ , size_t chrsCount /*in*/ ) // 1390/06/03
//{
//	ASSERT( pSz );
//	LPSTR  basestr = ( LPSTR  )sMem;
//	LPSTR  lpLastChr = basestr;
//	while ( ( lpLastChr = ( LPSTR  )memchrs( lpLastChr , basestr + memSz - lpLastChr , chrs , chrsCount ) ) )
//	{
//		memmove( lpLastChr , lpLastChr + 1 , basestr + memSz - lpLastChr - 1 );
//		memSz--;
//	}
//	if ( pSz ) *pSz = memSz;
//	return memSz ? basestr : NULL;
//}
//
//IN_GENERAL void_p removeiChrs( void_p const sMem /*in out*/ , size_t memSz /*in*/ , size_t * const pSz /*out*/ , void_p const chrs /*in*/ , size_t chrsCount /*in*/ ) // 1390/06/03
//{
//	ASSERT( pSz );
//	LPSTR  basestr = ( LPSTR  )sMem;
//	LPSTR  lpLastChr = basestr;
//	while ( ( lpLastChr = ( LPSTR  )memichrs( lpLastChr , basestr + memSz - lpLastChr , chrs , chrsCount ) ) )
//	{
//		memmove( lpLastChr , lpLastChr + 1 , basestr + memSz - lpLastChr - 1 );
//		memSz--;
//	}
//	if ( pSz ) *pSz = memSz;
//	return memSz ? basestr : NULL;
//}

//IN_GENERAL void_p serializeChrs( void_p const sMem /*in out*/ , size_t memSz /*in*/ , size_t * const pSz /*out*/ , void_p const chrs /*in*/ , size_t chrsCount /*in*/ ) // 1390/06/03
//{
//	ASSERT( pSz );
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
//		memmove( lpLastMem , lpChr , lpLastMem + memSz - lpChr );
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
//	ASSERT( pSz );
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
//		memmove( lpLastMem , lpChr , lpLastMem + memSz - lpChr );
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

IN_GENERAL LPCSTR strrichr( LPCSTR str , char c )
{
	char car[] = { c , EOS };
	return rstristr( str , car );
}

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
//				ASSERT( eos == EOS );
//			}
//			::swap( str[ 0 ] , eos );
//			ceos = str;
//		}
//	} while ( str && n-- > 0 );
//	if ( eos != EOS )
//	{
//		::swap( ceos[ 0 ] , eos );
//		ASSERT( eos == EOS );
//	}
//	return str;
//}

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
//				ASSERT( eos == EOS );
//			}
//			::swap( str[ 0 ] , eos );
//			ceos = str;
//		}
//	} while ( str && n-- > 0 );
//	if ( eos != EOS )
//	{
//		::swap( ceos[ 0 ] , eos );
//		ASSERT( eos == EOS );
//	}
//	return str;
//}

// ~~~~~ chr --------------------------------------------------------------------------------
// ---- chrs --------------------------------------------------------------------------------

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

//IN_GENERAL LPCSTR strchrs( LPCSTR str , LPCSTR chrs , int * const pCI ) // same as strpbrk . Written By Mohsen
//{
//	LPCSTR lpRet = strpbrk( str , chrs );
//	if ( pCI && lpRet )
//	{
//		*pCI = strchr( chrs , lpRet[ 0 ] ) - chrs;
//	}
//	return lpRet;
//}

//IN_GENERAL LPCSTR strrchrs( LPCSTR str , LPCSTR chrs , int * const pCI ) // Written By Mohsen
//{
//	LPCSTR pOutStr = NULL , pTempOutStr;
//	for ( ; chrs[ 0 ] != EOS ; chrs++ )
//	{
//		if ( ( pTempOutStr = ( LPCSTR )strrchr( str , chrs[ 0 ] ) ) )
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

LPCSTR strihead( LPCSTR str , LPCSTR head )
{
	return stristr( str , head ) == str ? str : NULL;
}

status sendall( sockfd socketfd , buffer buf , size_t * len ) // as beej book says
{
	size_t total = 0; // how many bytes we've snt
	size_t byteleft = *len; // how many we have left to send
	size_t n;
	ssize_t send_ret;

	while ( total < *len )
	{
		send_ret = send( socketfd , buf + total , byteleft , MSG_NOSIGNAL );
		if ( send_ret == EPIPE )
		{
			return errPeerClosed;
		}
		if ( send_ret == -1 ) { break; }
		n = ( size_t )send_ret;
		total += n;
		byteleft -= n;
	}
	*len  = total; // return number actually sent
	return n == -1 ? errGeneral : errOK; // return -1 on failure, 0 on success
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
		buf[ i ] = ( char )( '0' + num );   // store as character '1'..'9'
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
		if ( strcmp( target , strs[ i ] ) == 0 )
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
		if ( stricmp( target , strs[ i ] ) == 0 )
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

// Simple string hash function (djb2)
ulong hash( LPCSTR str )
{
	ulong h = 5381;
	uchar c;
	while ( ( c = ( uchar )*str++ ) )
		h = ( ( h << 5 ) + h ) + c; // h * 33 + c
	return h;
}
