#pragma once

#if defined __GNUC__ || !defined __COMPILING

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

#define CHAR_SZ 1qt
#define SHORT_SZ 2
#define INT_SZ 4
#define LONG_SZ 8
#define LONGLONG_SZ 8
#define FLOAT_SZ 4
#define DOUBLE_SZ 8
#define LONGDOUBLE_SZ 16
#define VOIDP_SZ 8
#define SIZE_T_SZ 8

//-------------------------------------------------------------------------
typedef long long __int64;				/*in ubuntu gcc -> 8 bytes*/
typedef unsigned long long __int64u;	/*in ubuntu gcc -> 8 bytes*/
typedef signed long long __int64s;		/*in ubuntu gcc -> 8 bytes*/

typedef char * LPSTR;			/*in ubuntu gcc -> 8 bytes*/
typedef const char * LPCSTR;	/*in ubuntu gcc -> 8 bytes*/ // keep this style


typedef LPSTR INNER_STR; /*used inside local function and it must be freed at the end*/
typedef LPSTR PASSED_STR; /*string that passed inside function and just could be read not freed atall*/

typedef LPCSTR INNER_CSTR; /*used inside local function and it must be freed at the end*/
typedef LPCSTR PASSED_CSTR; /*string that passed inside function and just could be read not freed atall*/


//typedef LPCSTR LPCTSTR;			/*in ubuntu gcc -> 8 bytes*/
//typedef LPSTR LPTSTR;			/*in ubuntu gcc -> 8 bytes*/

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

typedef unsigned char uint8;

typedef unsigned short uint16;

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

typedef int sockfd;

#endif

//-------------------------------------------------------------------------
// hamidi, 850529: az anjayi keh short budane status baese beh vojud amadane padding bytes dar class-ha mishavad va in khod baese dastresie nadorost beh virtual table-haye class-ha va dar natijeh exception beh khatere seda zadeh shodane pointer-haye nadorost migardad, status ra long ta'rif mikonim.
typedef long status;

// for when a char * is treated as a string, usually null terminated
// variables of this type are not d_valid to be null
typedef PASSED_STR string;

typedef const char * * strings; // array of string. pay attention to free inner data and outer pointer to them

// for when a char * is treated as a buffer, usually non-null terminated
// variables of this type may be null
typedef unsigned char * buffer;

typedef void * void_p;

typedef void * pass_p; // pass data pointer

typedef unsigned char   DATAB; // data block / byte

//-------------------------------------------------------------------------

typedef enum GeBoolean { eFalse = 0 , eTrue = 1 , eNull = 0x7fffffff } GeBoolean;

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

typedef PASSED_CSTR ( *callback_t6 )( pass_p /*pass_data*/ );

//-------------------------------------------------------------------------
#define G_SCHAR_MIN  ((schar)0x80)				// -128
#define G_SCHAR_MAX  ((schar)-1^G_SCHAR_MIN)	// 127
#define G_UCHAR_MIN  ((uchar)0x00)				// 0
#define G_UCHAR_MAX  ((uchar)-1^G_UCHAR_MIN)	// 255

#define G_SSHORT_MIN ((sshort)0x8000)			// -32768
#define G_SSHORT_MAX ((sshort)-1^G_SSHORT_MIN)	// 32767
#define G_USHORT_MIN ((ushort)0x0000)			// 0
#define G_USHORT_MAX ((ushort)-1^G_USHORT_MIN)	// 65535

#define G_SLONG_MIN  ((slong)0x80000000L)		// -2,147,483,648
#define G_SLONG_MAX  ((slong)-1^G_SLONG_MIN)	// 2,147,483,647
#define G_ULONG_MIN  ((ulong)0x00000000L)		// 0
#define G_ULONG_MAX  ((ulong)-1^G_ULONG_MIN)	// 4,294,967,295

#define G_SLONGLONG_MIN  ((sint64)0x8000000000000000i64)	// -9,223,372,036,854,775,808
#define G_SLONGLONG_MAX  ((sint64)-1^G_SLONGLONG_MIN)		// 9,223,372,036,854,775,807
#define G_ULONGLONG_MIN  ((uint64)0x0000000000000000i64)	// 0
#define G_ULONGLONG_MAX  ((uint64)-1^G_ULONGLONG_MIN)		// 18,446,744,073,709,551,615

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef M_2_PI
#define M_2_PI		6.283185307179586476925286766559
#endif
#define PI_2		1.57079632679489662
#define PI_180		0.01745329251994329
#define M_3_PI_2	4.71238898038468985

#define PHI			1.618

#ifdef Uses_timeval
	typedef struct timeval timeval;
#endif
