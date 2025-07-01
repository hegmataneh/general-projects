//-------------------------------------------------------------------------
#define NP 0  // not provided

//#ifndef _MSC_VER
//	#define null (void *)0
//#endif

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

#define PRECISION_OF_DOUBLE 1e-10

//#if defined Uses_VECTOR_LEN || !defined __COMPILING
//	#define VECTOR_LEN(x1,y1,x2,y2) (sqrt((double(x2)-double(x1))*(double(x2)-double(x1))+(double(y2)-double(y1))*(double(y2)-double(y1))))
//#endif

//-------------------------------------------------------------------------
#define COUNTOF(array) (sizeof(array)/sizeof(array[0]))

//-------------------------------------------------------------------------
#define HI_NIBBLE( byte )					( ( ( uchar )( byte ) ) >> 4 )
#define LO_NIBBLE( byte )					( ( ( uchar )( byte ) ) & 0x0f )
#define MAKE_BYTE( hiNibble , loNibble )	( ( uchar )( ( ( hiNibble & 0x0f ) << 4 ) | ( loNibble & 0x0f ) ) )

#define HI_BYTE(word)						(((ushort)(word))>>8)
#define LO_BYTE(word)						((word)&0x00FF)
#define MAKE_WORD(hiByte,loByte)			((WORD)(((hiByte)<<8)|(loByte)))

#define HI_WORD(dw)							((WORD)((((DWORD_PTR)(dw)) >> 16) & 0xffff))
#define LO_WORD(dw)							((WORD)(((DWORD_PTR)(dw)) & 0xffff))
#define MAKE_DWORD(hiWord,loWord)			((DWORD)(((WORD)(((DWORD_PTR)(loWord)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(hiWord)) & 0xffff))) << 16))

//-------------------------------------------------------------------------
//#if defined __GNUC__ || defined UNDER_CE  || defined __SYMBIAN32__
	/* in general, when static libraries are being made,
	 * aya mishavad chize moshtaraki ra beyne platform-ha peyda kard. dar symbian, vaghti static
	 * library darad sakhteh mishavad __LIB__ define shodeh ast.
	 */
	#define _EXPORT
	#define _IMPORT
//#else
//	#define _EXPORT __declspec(dllexport)
//	#define _IMPORT __declspec(dllimport)
//#endif

//-------------------------------------------------------------------------
#define NEWBUF(type,n) (type *)MALLOC(sizeof(type)*(n))
#define MEMSET_ZERO(p,type,n) memset( p , 0 , sizeof( type ) * n )

//-------------------------------------------------------------------------
#if defined(__GNUC__)

#define DAC_PTR(x) do { if(x) { FREE(x); x = NULL; } } while(0)
#define DAC(x) do { if(x) { DEL(x); x = NULL; } } while(0)
//#define DAC_AR(x) do { if(x) { DEL_AR(x); x = NULL; } } while(0)

#define NEW(type) ( ( type * )MALLOC( sizeof( type ) ) )

#endif

//-------------------------------------------------------------------------
#define DELSTR(str) do { if(str[0]!=EOS) { FREE(str); str=""; } } while(0)
//#define DELWSTR(str) do { if(str[0]!=WEOS) { FREE(str); str=_WT(""); } } while(0)

//#define ____TO_STRING(s) #s
//#define ___TO_STRING(s) (____TO_STRING(s))
//#define __TO_STRING(s) ((LPCSTR)(___TO_STRING(s)))

//-------------------------------------------------------------------------
//#define ERROR_DEF(err) status err=internalErrorVal((LPCSTR)#err)
//#define ERROR_USE(err) extern status err

//#define LOCAL_ERROR(err) //static ERROR_DEF(err)
//#define GLOBAL_ERROR(err) //_EXPORT ERROR_DEF(err)

//-------------------------------------------------------------------------
#if defined Uses_STR_funcs || !defined __COMPILING
#define STRCMP(s1,s2) strncmp_s(s1,s2,g_min(strlen(s1),strlen(s2)))
#define STRCPY(s1,s2)\
	do {\
	strncpy_s(s1,s2,sizeof(s1)-1);\
	s1[sizeof(s1)-1]=EOS;\
	delBlanks_s(s1,sizeof(s1));\
	} while(0)
#define STRNCPY(s1,s2,n)\
	do {\
	strncpy_s(s1,s2,n-1);\
	s1[n-1]=EOS;\
	} while(0)

#endif

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


//-------------------------------------------------------------------------
#define SGN(value) (((value)<0)?-1:((value)>0)?1:0)
#define IP2L( x, y, z, t ) ( ( (x) << 24 ) + ( (y) << 16 ) + ( (z) << 8 ) + (t) )


//-------------------------------------------------------------------------
// hamidi, 860510: where to store memory leak log. may be useful also elsewhere.
#if defined ARM || !defined __COMPILING // CE Device
	//#define ROOT_PATH "\\SDMMC Disk\\Navi\\" // Navi Up+
	#define ROOT_PATH "\\Storage Card\\Navi\\" // Mio Device
#elif defined UNDER_CE || !defined __COMPILING // CE Emulator
	#define ROOT_PATH "\\Storage Card\\"
#elif defined _MSC_VER || !defined __COMPILING // Windows XP
		/*yeh fekri beh hale in bekonim.
		c:\ jaye monasebi nist. %temp% beh nazar dorosttarin ja miad, vali bayad process
		besheh ta tabdil beh yek folder-e ghabele estefadeh basheh.
		dar halikeh in kar hamehja nemituneh anjam besheh...*/
	#define ROOT_PATH "c:\\"
#elif defined __GNUC__ || !defined __COMPILING // Linux
	#define ROOT_PATH "/tmp/"
#elif defined __SYMBIAN32__ || !defined __COMPILING
	#define ROOT_PATH "c:\\"  // fe'lan farz mikonim hamin khubeh ta ba'd
#endif  // platform selector

//-------------------------------------------------------------------------

#if defined _MSC_VER && _MSC_VER >= 1400
	#define UNIQUE_IDENTIFIER __COUNTER__
#else
	#define UNIQUE_IDENTIFIER __LINE__
#endif

//-------------------------------------------------------------------------
#define Tobool( condition )		( ( condition ) ? true : false )
#define ToBOOL( condition )		( ( condition ) ? TRUE : FALSE )
#define ToBoolean( condition )	( ( condition ) ? True : False )

//-------------------------------------------------------------------------

#define CONTAIN_FLAG( key , flag ) ( ( key ) & ( flag ) )
#define INCLUDE_FLAG( key , flag ) ( ( key ) | ( flag ) )
#define EXCLUDE_FLAG( key , flag ) ( ( key ) & ( ( flag ) ^ 0xffffffff ) ) // 32 bit
#define EXCLUDE_FLAG64( key , flag ) ( ( key ) & ( ( flag ) ^ 0xffffffffffffffff ) )
#define SET_INCLUDE_FLAG( key , flag ) ( ( key ) = INCLUDE_FLAG( key , flag ) )
#define SET_EXCLUDE_FLAG( key , flag ) ( ( key ) = EXCLUDE_FLAG( key , flag ) )

#define IS_ODD( i ) ( ( ( i ) & 1 ) ? TRUE : FALSE ) // fard
#define IS_EVEN( i ) ( ( ( i ) & 1 ) ? FALSE : TRUE ) // zoj
#define POW_OF_2( p ) ( 1 << p )

//#define MUTABLE_D_ERROR mutable status d_error

#define ROUND_UP( i , x ) ( ( (i) + ((x)-1) ) & ~((x)-1) ) // i ra zarib x mi konad


#if defined Uses_Remote_vs_prj || !defined __COMPILING

#define _MSG(s) __msg(__custom_message,sizeof(__custom_message),s,__LINE__)

#define _DETAIL_ERROR( user_friendly_msg ) do { perror(_MSG(user_friendly_msg)); perror( __snprintf( __custom_message , sizeof(__custom_message) , "more details: %s(#%d)@ln(%d)\n" , strerror(errno), errno , __LINE__ ) ); } while(0);

#define VOID_RET ((void*)NULL)
#define MAIN_BAD_RET (1/*Indicate an error*/)

#define ERR_RET( user_friendly_msg , RET ) \
	do {\
	_DETAIL_ERROR( user_friendly_msg );\
	return RET; } while(0);


#define MIN(a, b) ((a) <= (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#endif
