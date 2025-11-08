#pragma once

//-------------------------------------------------------------------------
#define NP 0  // not provided

//#ifndef _MSC_VER
//	#define null (void_p)0
//#endif

//#if defined Uses_VECTOR_LEN || !defined __COMPILING
//	#define VECTOR_LEN(x1,y1,x2,y2) (sqrt((double(x2)-double(x1))*(double(x2)-double(x1))+(double(y2)-double(y1))*(double(y2)-double(y1))))
//#endif

//-------------------------------------------------------------------------
#define COUNTOF(array) (sizeof(array)/sizeof(array[0]))


#define ALEN(...) /*count preproc variadc args*/ ALEN0(__VA_ARGS__, \
						 0x1E, 0x1F, 0x1D,0x1C, 0x1B, 0x1A,0x19,0x18, \
						 0x17, 0x16, 0x15,0x14, 0x13, 0x12,0x11,0x10, \
						 0x0E, 0x0F, 0x0D,0x0C, 0x0B, 0x0A,0x09,0x08, \
						 0x07, 0x06, 0x05,0x04, 0x03, 0x02,0x01,0x00)

#define ALEN0(_00,_01,_02,_03,_04,_05,_06,_07,_08,\
				_09,_0A,_0B,_0C,_0D,_0F,_0E,_10,_11,\
				_12,_13,_14,_15,_16,_17,_18,_19,_1A,\
				_1B,_1C,_1D,_1F,_1E,...) _1E /*ti 31 arg can count*/

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

//----- scope and access privilage --------------------------------------------------------------------
#if defined Uses_access_qualifier || !defined __COMPILING
	#if defined __GNUC__
		/* in general, when static libraries are being made,
		 * aya mishavad chize moshtaraki ra beyne platform-ha peyda kard. dar symbian, vaghti static
		 * library darad sakhteh mishavad __LIB__ define shodeh ast.
		 */
		#define _EXPORT /*obj declared at global namespace*/
		#define _IMPORT /*extern and linked to global namespace obj*/


		#define _WEAK_ATTR __attribute__( ( weak ) ) /*low priority in linkage*/
		#define _STRONG_ATTR /*hi priority in linkage*/

	#else
	//	#define _EXPORT __declspec(dllexport)
	//	#define _IMPORT __declspec(dllimport)
	#endif

	#define DO_WHILE(x) do { x; } while(0)

	#ifdef _INx
	#error
	#endif
	#define _INx /*input argument that stored*/

	#define _IN /*input argument*/

	#ifdef HDLR
	#error
	#endif
	#define HDLR /*handler of module*/

	//#define _OUT /*output argument*/

	#ifdef _NEW_OUT_P
	#error
	#endif
	#define _NEW_OUT_P /*output argument that allocated and freed by caller*/

	#ifdef _RET_VAL_P
	#error
	#endif
	#define _RET_VAL_P /*output argument that just filled and mng by caller*/

	#ifdef _CPY_VAL_P
	#error
	#endif
	#define _CPY_VAL_P /*output argument that just filled and mng by caller*/

	#ifdef OUTalc
	#error
	#endif
	#ifdef OUTx
	#error
	#endif
	#ifdef OUTcpy
	#error
	#endif

	#define OUTalc	_NEW_OUT_P /*allocate new*/
	#define OUTx	_RET_VAL_P /*assign*/
	#define OUTcpy	_CPY_VAL_P /*copy content*/


	#define _THREAD_FXN /*identified as thread callback fxn*/
	#define _CALLBACK_FXN
	#define _REGULAR_FXN

	#define _PRIVATE_FXN static
	#define _PUB_FXN /*can call from another lib*/

	#define PRE_MAIN_INITIALIZATION( id ) __attribute__( ( constructor( id ) ) ) /*put it before global fxn then system call it before main in gcc*/
	//#define PRE_MAIN_INITIALIZATION( id )


	#define _GLOBAL_VAR /*variable on global namespace*/

	#define _EXTERN extern

#endif

//-------------------------------------------------------------------------
//#define ERROR_DEF(err) status err=internalErrorVal((LPCSTR)#err)
//#define ERROR_USE(err) _EXTERN status err

//#define LOCAL_ERROR(err) //static ERROR_DEF(err)
//#define GLOBAL_ERROR(err) //_EXPORT ERROR_DEF(err)

//----- string macro --------------------------------------------------------------------
#if defined Uses_STR_funcs || !defined __COMPILING

	#if defined Uses_strcasecmp || defined Uses_iSTR_SAME || defined Uses_iSTR_DIFF || !defined __COMPILING

		#define stricmp			_stricmp
		#define _stricmp		strcasecmp

		#define STRICMP			stricmp

		#define iSTR_SAME( a , b )	( STRICMP( a , b ) == 0 )
		#define iSTR_DIFF( a , b )	( STRICMP( a , b ) != 0 )

	#endif

	#define STRINGIFY(x)	#x
	#define TOSTRING(x)		STRINGIFY(x)

	#define STRDUP( str ) strdup( str )

	#define STRCMP(s1,s2)	strcmp(s1,s2)
	//#define STRCMP(s1,s2)	strncmp_s(s1,s2,g_min(strlen(s1),strlen(s2)))

	#define STRCPY(dst,src)\
		do {\
		strncpy_s(dst,src,sizeof(dst)-1);\
		dst[sizeof(dst)-1]=EOS;\
		delBlanks_s(dst,sizeof(dst));\
		} while(0)

	#define STRNCPY(s1,s2,n)\
		do {\
		strncpy_s(s1,s2,n-1);\
		s1[n-1]=EOS;\
		} while(0)

	#define STR_SAME( a , b )	( strcmp( a , b ) == 0 )
	#define STR_DIFF( a , b )	( strcmp( a , b ) != 0 )
	
	#define DELSTR(str)			do { if(str[0]!=EOS) { FREE(str); str=""; } } while(0)

#endif



//-------------------------------------------------------------------------
#if defined Uses_memory_funcs || !defined __COMPILING

	#define MALLOC( size )					malloc( size ) /*allocate and left uninitialized. so faster*/
	#define MALLOC_ONE( ptr )				MALLOC_AR( ptr , 1 )
	#define MALLOC_AR( ptr , count )		( ( __typeof__( *(ptr) ) * )malloc( ( size_t )( count ) * sizeof( *(ptr) ) ) )

	#define REALLOC( ptr , size )			realloc( ptr , size )
	#define REALLOC_ONE( ptr )				REALLOC_AR( ptr , 1 )
	#define REALLOC_AR( ptr , count )		( ( __typeof__( *(ptr) ) * )realloc( ptr , ( size_t )( count ) * sizeof( *(ptr) ) ) )
	#define REALLOC_AR_SAFE( ptr , count )	do { __typeof__( *(ptr) ) * __tmp_ = ptr; if ( !( ptr = ( ( __typeof__( *(ptr) ) * )realloc( ptr , ( size_t )( count ) * sizeof( *(ptr) ) ) ) ) ) { ptr = __tmp_; } } while(0) /*safe reallocation*/

	#define CALLOC( count , elem_size )		calloc( ( count ) , ( elem_size ) ) /*allocate and memset with zero. so slower*/
	#define CALLOC_ONE( ptr )				CALLOC( 1 , sizeof( *(ptr) ) )
	#define CALLOC_AR( ptr , count )		CALLOC( ( count ) , sizeof( *(ptr) ) )

	//#define NEWBUF( type , n )			( type * )MALLOC( sizeof( type ) * ( n ) )
	//#define NEW( type )					( ( type * )MALLOC( sizeof( type ) ) )

	#define MEMSET( ptr , chr , sz )		memset( ptr , chr , sz )

	#define MEMSET_ZERO_T( ptr , type , n )	memset( ptr , 0 , sizeof( type ) * ( size_t )( n ) )
	#define MEMSET_ZERO( ptr , n )			memset( (ptr) , 0 , ( size_t )( n ) * sizeof( *(ptr) ) )
	#define MEMSET_ZERO_O( ptr )			memset( ptr , 0 , sizeof( *(ptr) ) )

	#define MEMCPY_ORIGINAL( dst , src , size )	memcpy( dst , src , size )
	#define MEMCPY_OR( dst , src , size )		MEMCPY_ORIGINAL( dst , src , size )
	#define MEMCPY( dst , src )					MEMCPY_ORIGINAL( dst , src , sizeof( *dst ) )
	#define MEMCPY_AR( dst , src , count )		MEMCPY_ORIGINAL( dst , src , sizeof( *dst ) * ( size_t )( count ) )

	#define MEMCMP_T( a , b , type )		memcmp( a , b , sizeof type )
	#define MEMCMP( a , b )					memcmp( a , b , sizeof *a )

	//#define DEL( ptr )					FREE( ptr ) // 14040616 . do not uncomment this
	//#define DEL_AR( ptr )					FREE( ptr ) // 14040616 . do not uncomment this

	#define FREE( ptr )						FREE_PTR( ptr )
	#define FREE_PTR( ptr )					free( ( void_p )ptr )
	#define FREE_PPTR( pptr , count )		do { for ( int ___i = 0 ; ___i < ( count ) ; ___i++ ) { FREE_PTR( pptr[ ___i ] ); } FREE_PTR( pptr ); } while( 0 )

	#define FREE_PLAIN_PTR( ptr )			FREE_PTR( ptr )
	#define FREE_DOUBLE_PTR( pptr , count )	FREE_PPTR( pptr , count )

	#define DAC( ptr )						DAC_PTR( ptr )
	#define DAC_PTR( ptr )					do { if( ptr ) { FREE( ptr ); ptr = NULL; } } while( 0 )
	//#define DAC_PPTR( pptr , count )		do { if( pptr ) { FREE_PPTR( pptr , count ); pptr = NULL; } } while( 0 )

	#define MEMMOVE( dest , src , len )		memmove( dest , src , len )

	#define MEMCHR( s , c , n )				memchr( s , c , n )

	#define STRLEN( s ) strlen( s )

#endif // Uses_memory_funcs



//-------------------------------------------------------------------------
#define SGN(value) (((value)<0)?-1:((value)>0)?1:0)
#define IP2L( x, y, z, t ) ( ( (x) << 24 ) + ( (y) << 16 ) + ( (z) << 8 ) + (t) )


//-------------------------------------------------------------------------
// hamidi, 860510: where to store memory leak log. may be useful also elsewhere.
//#if defined ARM || !defined __COMPILING // CE Device
//	//#define ROOT_PATH "\\SDMMC Disk\\Navi\\" // Navi Up+
//	#define ROOT_PATH "\\Storage Card\\Navi\\" // Mio Device
//#elif defined UNDER_CE || !defined __COMPILING // CE Emulator
//	#define ROOT_PATH "\\Storage Card\\"
//#elif defined _MSC_VER || !defined __COMPILING // Windows XP
//		/*yeh fekri beh hale in bekonim.
//		c:\ jaye monasebi nist. %temp% beh nazar dorosttarin ja miad, vali bayad process
//		besheh ta tabdil beh yek folder-e ghabele estefadeh basheh.
//		dar halikeh in kar hamehja nemituneh anjam besheh...*/
//	#define ROOT_PATH "c:\\"
//#elif defined __GNUC__ || !defined __COMPILING // Linux
//	#define ROOT_PATH "/tmp/"
//#elif defined __SYMBIAN32__ || !defined __COMPILING
//	#define ROOT_PATH "c:\\"  // fe'lan farz mikonim hamin khubeh ta ba'd
//#endif  // platform selector

//-------------------------------------------------------------------------

#if defined _MSC_VER && _MSC_VER >= 1400
	#define UNIQUE_IDENTIFIER __COUNTER__
#else
	#define UNIQUE_IDENTIFIER __LINE__
#endif

//-------------------------------------------------------------------------
#define Tobool( condition )		( ( condition ) ? true : false )
//#define ToBOOL( condition )		( ( condition ) ? TRUE : FALSE )
#define ToBoolean( condition )	( ( condition ) ? True : False )

#define Booleanize( val ) ( !!val )

//-------------------------------------------------------------------------

#define CONTAIN_FLAG( key , flag )		( ( key ) & ( flag ) )
#define INCLUDE_FLAG( key , flag )		( ( key ) | ( flag ) )
#define EXCLUDE_FLAG( key , flag )		( ( key ) & ( ( flag ) ^ 0xffffffff ) ) // 32 bit
#define EXCLUDE_FLAG64( key , flag )	( ( key ) & ( ( flag ) ^ 0xffffffffffffffff ) )
#define SET_INCLUDE_FLAG( key , flag )	( ( key ) = INCLUDE_FLAG( key , flag ) )
#define SET_EXCLUDE_FLAG( key , flag )	( ( key ) = EXCLUDE_FLAG( key , flag ) )

#define IS_ODD( i )		( ( ( i ) & 1 ) ? TRUE : FALSE ) // fard
#define IS_EVEN( i )	( ( ( i ) & 1 ) ? FALSE : TRUE ) // zoj
#define POW_OF_2( p )	( 1 << p )

//#define MUTABLE_D_ERROR mutable status d_error

#define ROUND_UP( i , x )	( ( (i) + ((x)-1) ) & ~((x)-1) ) // i ra zarib x mi konad

#ifndef ISNULL
	#define ISNULL( a , b ) ( (a) ? (a) : (b) )
	#define COALESCE2( a , b ) ( (a) ? (a) : (b) )
	#define COALESCE( a , b , c ) ( (a) ? (a) : (b) ? (b) : (c) )
	#define COALESCE4( a , b , c , d ) ( (a) ? (a) : (b) ? (b) : (c) ? (c) : (d) )
#endif


#if defined Uses_ERROR_SECTION || !defined __COMPILING

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
		//#define DIRECT_ECHO_BUF _g->stat.last_command // add this just before include dep
	#endif

	#define _VERBOSE_ECHO(msg,...) do {\
		SET_STDERR( __snprintf( __custom_message , sizeof( __custom_message ) , "ln%d-" msg , __LINE__ , ##__VA_ARGS__ ) );  } while(0)


	//#define ERR_RET( user_friendly_msg , RET ) 
	//	do {
	//	_DETAIL_ERROR( user_friendly_msg );
	//	return RET; } while(0);

#endif // #if defined Uses_ERROR_SECTION


#define VOID_RET ((void_p)NULL)
#define MAIN_BAD_RET (1/*Indicate an error*/)

#define MIN(a, b) ((a) <= (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define _FALSE_SHARE_SAFE /*speed up memory access*/
#define _ALIGNED(date) /*speed up struct member access*/

#define _MEMMOVE_UNSAFE_FXN /*used in fxn that return pointer to place that is not safe when main block moved to other place and pointer is dangled*/

#define _PAD_NAME2(line) pad_##line
#define _PAD_NAME(line)  _PAD_NAME2(line)
// Main macro to insert padding
#define PAD(sz) char _PAD_NAME(__LINE__)[sz]

#define CACHE_LINE_SIZE 64 /*see chatgpt for extra explnation*/

#define ALIGN_STRUCT(sz) __attribute__( ( aligned( sz ) ) ) /*__attribute__( ( aligned( CACHE_LINE_SIZE ) ) )*/

#define MACRO_E( mcro ) do { mcro; } while(0);
#define NULL_ACT MACRO_E( while(0) )
#define RANJE_ACT1( swt_var , cse1 , true_act , false_act ) do { switch( swt_var ) { case cse1: { true_act; break; } default: { false_act; } } } while( 0 )
#define RANJE_ACT2( swt_var , cse1 , cse2 , true_act , false_act ) do { switch( swt_var ) { case cse1: case cse2: { true_act; break; } default: { false_act; } } } while( 0 )

#ifdef _DEBUG
	#if defined Uses_WARNING || !defined __COMPILING
		#define CHK_WARN( expr )	if (!(expr)) {\
							fprintf( stderr , "Assertion failed: %s, file %s, line %d\n" ,\
								#expr , __FILE__ , __LINE__ );\
							}
		#define WARNING( expr ) CHK_WARN( expr )
	#endif
#else
	#define WARNING( expr )
#endif


//-----  color part --------------------------------------------------------------------
#ifndef _MSC_VER
	#define RGB(r,g,b) (((((b)<<8)|(g))<<8)|(r))
#endif
#define RGBColor(r,g,b) ((((((ulong)(b))<<8)|((ulong)(g)))<<8)|((ulong)(r)))

#define RGB2YUV(R,G,B) MAKE_DWORD( MAKE_WORD( 0 , 0.7*(R) - 0.6*(G) - 0.1*(B) ) , MAKE_WORD( -0.3*(R) - 0.6*(G) + 0.9*(B) , 0.3*(R) + 0.6*(G) + 0.1*(B) ) )
#define YUV2RGB(Y,U,V) MAKE_DWORD( MAKE_WORD( 0 , Y + U ) , MAKE_WORD( Y - 0.166667*(U) - 0.5*(V) , Y + V ) )

// sort according to Microsoft Windows default 16-color palette
#define BLACK		RGB(0x00,0x00,0x00)
#define LOW_RED		RGB(0x80,0x00,0x00) // MAROON
#define LOW_GREEN	RGB(0x00,0x80,0x00)
#define OLIVE		RGB(0x80,0x80,0x00) // BROWN
#define LOW_BLUE	RGB(0x00,0x00,0x80) // NAVY
#define LOW_MAGENTA	RGB(0x80,0x00,0x80) // PURPLE
#define LOW_CYAN	RGB(0x00,0x80,0x80) // TEAL
#define LIGHT_GRAY	RGB(0xC0,0xC0,0xC0) // SILVER
#define GRAY		RGB(0x80,0x80,0x80)
#define RED			RGB(0xff,0x00,0x00)
#define GREEN		RGB(0x00,0xff,0x00)
#define YELLOW		RGB(0xff,0xff,0x00)
#define BLUE		RGB(0x00,0x00,0xff)
#define MAGENTA		RGB(0xff,0x00,0xff)
#define CYAN		RGB(0x00,0xff,0xff)
#define WHITE		RGB(0xff,0xff,0xff)

#define BLUE_3		RGB(0x00,0x00,0xc8)
#define BLUE_4		RGB(0x00,0x00,0x2D)
#define GRAY_3		RGB(0x55,0x55,0x55)

#define C_RGB_1		RGB(0x1e,0x1e,0x1e)
#define C_RGB_2		RGB(0x33,0x33,0x33)


#define RANDOM_COLOR RGB(random(256),random(256),random(256))
#define INVALID_COLOR 0x01000000  // >= this value are invalid colors

#define RED_PART(rgb)   (((rgb)&0x0000FFL)>>0)
#define GREEN_PART(rgb) (((rgb)&0x00FF00L)>>8)
#define BLUE_PART(rgb)  (((rgb)&0xFF0000L)>>16)

#define DARKER(rgb)		(((rgb)>>1)&0x7f7f7fL)

//----- ~ color part --------------------------------------------------------------------

#define GROW_STEP 1

#ifndef LOCK_LINE
	#define LOCK_LINE( exp ) exp
#endif

#define STAT_FLD /*it is used for statistics*/

#define TODO 
