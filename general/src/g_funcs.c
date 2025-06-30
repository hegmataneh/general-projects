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

#define Uses_strlen
#define Uses_malloc
#define Uses_close
#define Uses_va_list
#define Uses_time_t
#define Uses_wchar_t
#define Uses_sprintf_s
#define Uses_sprintf_s
#define Uses_strcpy_s

#include <general.dep>

//uchar _ErrLvl;

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


//#ifdef LOG_PENDING_ERROR
//static GcString _M_logMsg;
//static Boolean _M_logMsgPending;
//#endif

void M_mkLogMsg(const char *fn,int ln,const char *msg,status error)
{
	#ifdef LOG_PENDING_ERROR
	if (msg)
	{
		// kheyli zeshteh keh compiler natuneh chenin conversion-i ro beh tore automatic anjam bedeh.
		_M_logMsg=(LPCSTR)GcPrintfString("%s (%d): %s, %s\n",LPCSTR(getfn(fn)),ln,internalErrorStr(error),msg);
	}
	else
	{
		_M_logMsg=(LPCSTR)GcPrintfString("%s (%d): %s\n",LPCSTR(getfn(fn)),ln,internalErrorStr(error));
	}
	_M_logMsgPending=True;
	#endif
}

void M_logMsg()
{
	#ifdef LOG_PENDING_ERROR
	if (_M_logMsgPending)
		{
		Log(L_ERR,_M_logMsg);
		_M_logMsgPending=False;
		}
	#endif
}

void M_mkShowMsg( const char * fn , int ln , const char * msg , status error )
{
}

void M_showMsg()
{
}

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

const char * __msg( char * msg_holder , size_t size_of_msg_holder , const char * msg , int line_number )
{
	snprintf( msg_holder , size_of_msg_holder , "%s: ln(%d)\n" , msg , line_number );
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

	fread( buffer , 1 , (size_t)len , file );
	buffer[ len ] = EOS;

	fclose( file );
	file = NULL;

	return ( const char * )buffer;
}
