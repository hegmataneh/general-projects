//#pragma once

// hamidi, 850418: this file defines something system include files may lack.
// it's a part of system.h, not general.h.
//#ifdef UNDER_CE
//	// hamidi, 850418: ERRFALSE is used in windbase.h, but is not defined anywhere!
//	#if defined Uses_ERRFALSE || !defined __COMPILING
//		#ifndef ERRFALSE
//			#define ERRFALSE(cond)
//		#endif
//	#endif
//
//	// mohsen, 870325: these macros does not define in popular header file
//	#define WS_EX_RTLREADING        0x00002000L
//	#define WS_EX_LTRREADING        0x00000000L
//
//	// hamidi, 850315: _filelength in CE is not available and file handle in CE is void *
//	long _filelength(void_p);
//
//	#if defined Uses__mkdir || !defined __COMPILING
//		int _mkdir(LPCSTR dirname);
//	#endif
//
//	#if defined Uses__mbstrlen || !defined __COMPILING
//		#define _mbstrlen strlen
//	#endif
//
//	#if defined Uses__fseeki64 || !defined __COMPILING
//		#define _fseeki64 fseek
//	#endif
//
//#endif  // UNDER_CE

#if defined __GNUC__ || defined __SYMBIAN32__  // ANSI C++
	#ifndef _MAX_PATH
		#define _MAX_PATH PATH_MAX
	#endif

	//#define _fileno fileno
	//#define _getch getch
	//#define _stricmp strcasecmp

	//long _filelength( int fd );  // niazi nist check konim bebinim agar Uses__filelength ta'rif shodeh bud prototype-e in function ro lehaz koneh. chon vojude in prototype beh chize digei vabasteh nist va beh hamin jahat beh jaee ham bar nemikhoreh va dependency-i ijad nemikoneh. tebghe standard, gozaashtane chenin ifdef-hayi dar mavaredi az in dast optional-eh.
	//int _mkdir(LPCSTR path);
	//LPSTR _strlwr( LPSTR src );
	//LPSTR _strupr( LPSTR src );
	//LPSTR _strrev( LPSTR src );

	//#define sscanf_s sscanf
	//#define _atoi64 atoll
	//#define _splitpath_s splitpath

	//#ifdef __GNUC__  // dar symbian prototype-esh ro jayi peyda kardam.
	//	//wchar_t *_wcsrev( wchar_t *src );
	//	
	//	#ifdef _DEBUG
	//	#define ASSERT(x) assert(x)
	//	#else
	//	#define ASSERT(x)
	//	#endif

	//#endif

#endif  // end of providing what ANSI C++ lacks

// secure functions declaration and implementation
#if !defined _MSC_VER || _MSC_VER < 1400 || defined UNDER_CE
	typedef int errno_t;  // gharar nist return value-e secure functions check besheh. chon beh tore default, in tavabe' dar surate boruze khata exception midan.

	//#ifndef _tcscpy_s
	//	#ifdef UNICODE
	//		#define _tcscpy_s wcscpy_s
	//	#else
	//		#define _tcscpy_s strcpy_s
	//	#endif // UNICODE
	//#endif // _tcscpy_s

	//#if defined Uses_strcpy_s || !defined __COMPILING
	//	errno_t strcpy_s(LPSTR dest,size_t sz,LPCSTR src);
	//	inline errno_t strcpy_s(char dest[],LPCSTR src)
	//		{ return strcpy_s(dest,sizeof(dest),src); }
	//#endif

	//#if defined Uses_wcscpy_s || !defined __COMPILING
	//	errno_t wcscpy_s(wchar_t *dest,size_t sz,const wchar_t *src);
	//	inline errno_t wcscpy_s(wchar_t dest[],const wchar_t *src)
	//		{ return wcscpy_s(dest,sizeof(dest)/sizeof(wchar_t),src); }
	//#endif

	//#if defined Uses_strncpy_s || !defined __COMPILING
	//	errno_t strncpy_s(LPSTR dest,size_t sz,LPCSTR src,size_t count);
	//	inline errno_t strncpy_s(char dest[],LPCSTR src,size_t count)
	//		{ return strncpy_s(dest,sizeof(dest),src,count); }
	//#endif

	//#if defined Uses_strcat_s || !defined __COMPILING
	//	errno_t strcat_s(LPSTR dest,size_t sz,LPCSTR src);
	//	inline errno_t strcat_s(char dest[],LPCSTR src)
	//		{ return strcat_s(dest,sizeof(dest),src); }
	//#endif

	//#if defined Uses__strlwr_s || !defined __COMPILING
	//	errno_t _strlwr_s(LPSTR str,size_t sz);
	//	inline errno_t _strlwr_s(char str[])
	//		{ return _strlwr_s(str,sizeof(str)); }
	//#endif

	//#if defined Uses__strupr_s || !defined __COMPILING
	//	errno_t _strupr_s(LPSTR str,size_t sz);
	//	inline errno_t _strupr_s(char str[])
	//		{ return _strupr_s(str,sizeof(str)); }
	//#endif

	#if defined Uses_sprintf_s || !defined __COMPILING
		/* hamidi, 850909: majburim ya ba estefadeh az in macro faghat form-e dovvom
		 * (template, array)-e in function ro support konim ya implementation-e jodayi
		 * ba estefadeh az vsprintf barash dar nazar begirim. az unjayi keh ma'mulan argument-e
		 * avvale sprintf yeh buffer (array)-eh, fe'lan raveshe avval ro piadeh mikonim ta ba'd
		 * ageh lazem shod form-e avval ro ham dashteh bashim az raveshe dovvom berim.
		 */
		#define sprintf_s sprintf
		// hamidi, 850922: lazem shod
		//int sprintf(LPSTR buffer,size_t sizeOfBuffer,LPCSTR format,...);
	#endif

	//#if defined Uses_vsprintf_s || !defined __COMPILING
	//	int vsprintf_s(LPSTR buf,size_t sz,LPCSTR fmt,va_list argptr);
	//	inline int vsprintf_s(char buf[],LPCSTR fmt,va_list argptr)
	//		{ return vsprintf_s(buf,sizeof(buf),fmt,argptr); }
	//#endif

	//#if defined Uses_fopen_s || !defined __COMPILING
	//	inline errno_t fopen_s(FILE **pFile,LPCSTR filename,LPCSTR mode)
	//		{ return (*pFile=fopen(filename,mode))==NULL; }
	//#endif

	//#if defined Uses_sscanf_s || !defined __COMPILING
	//	#define sscanf_s sscanf
	//#endif

	//#if defined Uses_strtok_s || !defined __COMPILING
	//	LPSTR strtok_s(LPSTR strToken,LPCSTR strDelimit,char **context);
	//#endif

	//#if defined Uses__itoa_s || !defined __COMPILING
	//	#define _itoa_s _itoa
	//#endif

#endif  // not in visual studio 2005, end of secure functions declaration and implementation

#define PTHREAD_CREATE_OK (0)
#define PTHREAD_JOIN_OK (0)
#define SENDTO_MIN_OK (0)
#define REALLOC_ERR (NULL)
#define FXN_SOCKET_ERR (-1)
#define NEWBUF_ERR (NULL)
#define NEW_ERR (NULL)
#define FXN_BIND_ERR (-1)


