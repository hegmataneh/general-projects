#pragma once

#ifdef __GNUC__

// char			:  1 bytes
// signed char	: 1 bytes
// unsigned char	: 1 bytes
// short			: 2 bytes
// unsigned short	: 2 bytes
// int				: 4 bytes
// unsigned int		: 4 bytes
// long				: 8 bytes
// unsigned long	: 8 bytes
// long long		: 8 bytes
// unsigned long long	: 8 bytes
// float				: 4 bytes
// double				: 8 bytes
// long double			: 16 bytes
// bool		: 1 bytes
// void *	: 8 bytes
// size_t	: 8 bytes

//-------------------------------------------------------------------------
typedef long long __int64;				/*in ubuntu gcc -> 8 bytes*/
typedef unsigned long long __int64u;	/*in ubuntu gcc -> 8 bytes*/
typedef signed long long __int64s;		/*in ubuntu gcc -> 8 bytes*/
typedef const char * LPCSTR;	/*in ubuntu gcc -> 8 bytes*/

typedef char * LPSTR;			/*in ubuntu gcc -> 8 bytes*/

typedef LPCSTR LPCTSTR;			/*in ubuntu gcc -> 8 bytes*/
typedef LPSTR LPTSTR;			/*in ubuntu gcc -> 8 bytes*/

typedef __int64u ubigint;		/*in ubuntu gcc -> 8 bytes*/
typedef __int64u uint64;		/*in ubuntu gcc -> 8 bytes*/
typedef unsigned long    ulong;	/*in ubuntu gcc -> 8 bytes*/
typedef unsigned int     uint;	/*in ubuntu gcc -> 4 bytes*/
typedef unsigned short   ushort;/*in ubuntu gcc -> 2 bytes*/
typedef unsigned char    uchar;	/*in ubuntu gcc -> 1 bytes*/

typedef __int64s sbigint;		/*in ubuntu gcc -> 8 bytes*/
typedef __int64s sint64;		/*in ubuntu gcc -> 8 bytes*/
typedef signed long	   slong;	/*in ubuntu gcc -> 8 bytes*/
typedef signed int     sint;	/*in ubuntu gcc -> 4 bytes*/
typedef signed short   sshort;	/*in ubuntu gcc -> 2 bytes*/
typedef signed char    schar;	/*in ubuntu gcc -> 1 bytes*/

typedef char CHAR;				/*in ubuntu gcc -> 1 bytes*/

typedef __int64 bigint;			/*in ubuntu gcc -> 8 bytes*/
typedef __int64 int64;			/*in ubuntu gcc -> 8 bytes*/

#ifndef _QWORD_DEFINED
#define _QWORD_DEFINED
typedef __int64 QWORD, * LPQWORD;
#endif

#ifdef FILE_OFFSET_64
typedef int64 FILEADDR;
#else
typedef int FILEADDR;
#endif

#endif

//-------------------------------------------------------------------------
// hamidi, 850529: az anjayi keh short budane status baese beh vojud amadane padding bytes dar class-ha mishavad va in khod baese dastresie nadorost beh virtual table-haye class-ha va dar natijeh exception beh khatere seda zadeh shodane pointer-haye nadorost migardad, status ra long ta'rif mikonim.
typedef long status;
// for when a char * is treated as a string, usually null terminated
// variables of this type are not d_valid to be null
typedef char * string;
// for when a char * is treated as a buffer, usually non-null terminated
// variables of this type may be null
typedef char * buffer;

typedef void * void_p;

//-------------------------------------------------------------------------
enum GeBoolean { eFalse = 0 , eTrue = 1 , eNull = 0x7fffffff };
#define Boolean GeBoolean
#define False eFalse
#define True eTrue
#define Null eNull

#define LLONG( ll ) ll##L
#define LLONGLONG( ll ) ll##LL


// callback definitions -------------------------------------
typedef void ( *callback_t1 )( void_p );
typedef void_p ( *callback_t2 )( void_p );
typedef void ( *callback_t3 )( void_p , void_p );
typedef void_p ( *callback_t4 )( void_p , void_p );
typedef void ( *callback_t5 )( void_p , int );


