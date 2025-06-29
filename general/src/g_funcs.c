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

#define USES_time_t
#define USES_wchar_t
#define USES_sprintf_s
#define Uses_sprintf_s

#include <general.dep>

uchar _ErrLvl;

static short _err = NEXT_GENERAL_ERROR_VALUE;
static LPCSTR errStrs[10000]={"errOK","errGeneral","errMemoryLow","errInvalidString","errCanceled","syntax error"};

GLOBAL_ERROR(errCacheNoSave);
GLOBAL_ERROR(errCacheNoLoad);
GLOBAL_ERROR(errCacheNotValid);

GLOBAL_ERROR(errArrayFull);
GLOBAL_ERROR(errArrayMaxNumber);
GLOBAL_ERROR(errArrayMemoryFail);
GLOBAL_ERROR(errArrayOutOfRange);

GLOBAL_ERROR(errCLCTMemLow);
GLOBAL_ERROR(errCLCTNumberTooBig);

GLOBAL_ERROR(errIXFail);
GLOBAL_ERROR(errIXGreaterFound);
GLOBAL_ERROR(errIXNotOpen);


LOCAL_ERROR(errCacheInvalidSpec);
LOCAL_ERROR(errCacheInvalidAdd);

LOCAL_ERROR(errInvalidAlignment);

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

//char *newStr( LPCSTR str )
//{
//	if(!str)str="";
//	if(!str[0])return "";
//	size_t sz=strlen(str)+1;
//	char *temp=NEWBUF(char,sz);
//	if (temp) strcpy_s(temp,sz,str);
//	return temp;
//}
//
//// cleans dest first if can create a new string from src
//status newStr(LPSTR &dest,LPCSTR src)
//{
//	status d_error;
//	char *temp;
//
//	BREAK_IF(!src,errInvalidString,0);
//	NEWSTR(temp,src,0);
//	if (dest[0]) FREE(dest);
//	dest=temp;
//
//	RET
//}
//
//#ifndef UNDER_CE
////wchar_t *newStr( LPCWSTR str )
////{
////	if(!str)str=L"";
////	if(!str[0])return L"";
////	size_t sz=wcslen(str)+1;
////	wchar_t *temp=NEWBUF(wchar_t,sz);
////	if (temp) wcscpy_s(temp,sz,str);
////	return temp;
////}
//
////status newStr(LPWSTR &dest,LPCWSTR src)
////{
////	status d_error;
////	wchar_t *temp;
////
////	BREAK_IF(!src,errInvalidString,0);
////	NEWWSTR(temp,src,0);
////	if (dest[0]) FREE(dest);
////	dest=temp;
////
////	RET
////}
//#endif
