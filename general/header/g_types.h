//#pragma once

/*
83/6/29, hamidi
	  - adding string and buffer types
*/

//-------------------------------------------------------------------------
//#ifdef UNDER_CE
//	typedef long off_t;
//#endif

//#ifdef __GNUC__

	typedef long long __int64;
	typedef unsigned long long __int64u;
	typedef signed long long __int64s;
	typedef const char * LPCSTR;
	//typedef const wchar_t * LPCWSTR;

//#endif
	
//#ifdef __SYMBIAN32__
//	typedef long DWORD;
//#endif
	
//#if defined _MSC_VER || defined __SYMBIAN32__
//	typedef unsigned __int64 __int64u;
//	typedef signed __int64 __int64s;
//#endif
	

//typedef wchar_t * LPWSTR;
typedef char * LPSTR;
//#ifdef _UNICODE  // ba farze inkeh dar platform-haye digeh ham az _UNICODE baraye moshakhas kardane unicode budane character-ha dar barnameh estefadeh misheh.
//	typedef LPCWSTR LPCTSTR;
//	typedef LPWSTR LPTSTR;
//#else
	typedef LPCSTR LPCTSTR;
	typedef LPSTR LPTSTR;
//#endif


typedef __int64u ubigint;
typedef __int64u uint64;
typedef unsigned long    ulong;
typedef unsigned int     uint;
typedef unsigned short   ushort;
typedef unsigned char    uchar;

typedef __int64s sbigint;
typedef __int64s sint64;
typedef signed long	   slong;
typedef signed int     sint;
typedef signed short   sshort;
typedef signed char    schar;

typedef char CHAR;

typedef __int64 bigint;
typedef __int64 int64;

#ifndef _QWORD_DEFINED
#define _QWORD_DEFINED
typedef __int64 QWORD, * LPQWORD;
#endif

#ifdef FILE_OFFSET_64
typedef int64 FILEADDR;
#else
typedef int FILEADDR;
#endif

//-------------------------------------------------------------------------
//typedef short status;
// hamidi, 850529: az anjayi keh short budane status baese beh vojud amadane padding bytes dar class-ha mishavad va in khod baese dastresie nadorost beh virtual table-haye class-ha va dar natijeh exception beh khatere seda zadeh shodane pointer-haye nadorost migardad, status ra long ta'rif mikonim.
typedef long status;
// for when a char * is treated as a string, usually null terminated
// variables of this type are not d_valid to be null
typedef char * string;
// for when a char * is treated as a buffer, usually non-null terminated
// variables of this type may be null
typedef char * buffer;

//-------------------------------------------------------------------------
enum GeBoolean { eFalse = 0 , eTrue = 1 , eNull = 0x7fffffff };
#define Boolean GeBoolean
#define False eFalse
#define True eTrue
#define Null eNull

//-------------------------------------------------------------------------
//#if defined Uses_timestamp || !defined __COMPILING
//typedef time_t timestamp;
//#endif

#define LLONG( ll ) ll##L
#define LLONGLONG( ll ) ll##LL


// callback definitions -------------------------------------
typedef void *(*callback_t)(void *);


