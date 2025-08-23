//#pragma once

//-------------------------------------------------------------------------
#define NP 0  // not provided

//#ifndef _MSC_VER
//	#define null (void_p)0
//#endif

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
#if defined __GNUC__
	/* in general, when static libraries are being made,
	 * aya mishavad chize moshtaraki ra beyne platform-ha peyda kard. dar symbian, vaghti static
	 * library darad sakhteh mishavad __LIB__ define shodeh ast.
	 */
	#define _EXPORT
	#define _IMPORT


	#define _WEAK_ATTR __attribute__( ( weak ) )
	#define _STRONG_ATTR

#else
//	#define _EXPORT __declspec(dllexport)
//	#define _IMPORT __declspec(dllimport)
#endif

//-------------------------------------------------------------------------
//#define ERROR_DEF(err) status err=internalErrorVal((LPCSTR)#err)
//#define ERROR_USE(err) extern status err

//#define LOCAL_ERROR(err) //static ERROR_DEF(err)
//#define GLOBAL_ERROR(err) //_EXPORT ERROR_DEF(err)


//----- string macro --------------------------------------------------------------------
#if defined Uses_STR_funcs || !defined __COMPILING

#if defined Uses_strcasecmp || !defined __COMPILING

#define stricmp _stricmp
#define _stricmp strcasecmp

#endif

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)


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

#define STR_SAME( a , b ) ( strcmp( a , b ) == 0 )
#define STR_DIFF( a , b ) ( strcmp( a , b ) != 0 )

#define iSTR_SAME( a , b ) ( stricmp( a , b ) == 0 )
#define iSTR_DIFF( a , b ) ( stricmp( a , b ) != 0 )

#define DELSTR(str) do { if(str[0]!=EOS) { FREE(str); str=""; } } while(0)

#endif



//-------------------------------------------------------------------------
#if defined Uses_memory_funcs || !defined __COMPILING

#define MALLOC( size ) malloc( size )
#define MALLOC_AR( p , count ) ( ( __typeof__( *p ) * )malloc( ( size_t )count * sizeof( *p ) ) )
#define REALLOC( ptr , size ) realloc( ptr , size )
#define CALLOC calloc
//#define NEWBUF( type , n ) ( type * )MALLOC( sizeof( type ) * ( n ) )
//#define NEW( type ) ( ( type * )MALLOC( sizeof( type ) ) )

#define MEMSET_ZERO_T( p , type , n ) memset( p , 0 , sizeof( type ) * ( size_t )n )
#define MEMSET_ZERO( p , n ) memset( (p) , 0 , ( size_t )n * sizeof( *(p) ) )

#define MEMCPY_S( a , b , size ) memcpy( a , b , size )
#define MEMCPY( a , b ) memcpy( a , b , sizeof( *a ) )
#define MEMCPY_AR( a , b , count ) memcpy( a , b , sizeof( *a ) * ( size_t )count )

#define MEMCMP_T( a , b , type ) memcmp( a , b , sizeof type )
#define MEMCMP( a , b ) memcmp( a , b , sizeof *a )

#define DEL( p ) FREE( p )
#define DEL_AR( p ) FREE( p )
#define FREE( p ) free( ( void_p )p )
#define DAC_PTR( x ) do { if( x ) { FREE( x ); x = NULL; } } while( 0 )
#define DAC( x ) do { if( x ) { DEL( x ); x = NULL; } } while( 0 )

#endif



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

// INIT_BREAKABLE_FXN()

//#define _MSG(s) __msg(__custom_message,sizeof(__custom_message),s,__LINE__)

//#define _DETAIL_ERROR( user_friendly_msg ) do 
// { 
// perror(_MSG(user_friendly_msg)); 
// perror( __snprintf( __custom_message , sizeof(__custom_message) , "more details: %s(#%d)@ln(%d)\n" , strerror(errno), errno , __LINE__ ) );
// } while(0);

#define SYS_ERR_STR(msg) make_msg_appnd_sys_err( __custom_message , sizeof(__custom_message) , msg )

#define SET_STDERR(s) do { perror(s); } while(0)

#define _ECHO(s,...) do { SET_STDERR(__snprintf(__custom_message , sizeof(__custom_message),s,##__VA_ARGS__)); } while(0)

#if defined DIRECT_ECHO_BUF || !defined __COMPILING
	#define _DIRECT_ECHO(s,...) do { __snprintf(DIRECT_ECHO_BUF , sizeof(DIRECT_ECHO_BUF),s,##__VA_ARGS__); } while(0)
#endif

#define _VERBOSE_ECHO(msg,...) do {\
	SET_STDERR( __snprintf( __custom_message , sizeof( __custom_message ) , "ln%d-" msg , __LINE__ , ##__VA_ARGS__ ) );  } while(0)

#define VOID_RET ((void_p)NULL)
#define MAIN_BAD_RET (1/*Indicate an error*/)

//#define ERR_RET( user_friendly_msg , RET ) 
//	do {
//	_DETAIL_ERROR( user_friendly_msg );
//	return RET; } while(0);


#define MIN(a, b) ((a) <= (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define _THREAD_FXN /*identified as thread callback fxn*/
#define _CALLBACK_FXN

#endif
