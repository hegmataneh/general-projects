//#pragma once

//----T2T functions------------------------------------------------------
status string_to_int( LPCSTR str , int * out );


//----formats wrapper functions------------------------------------------------------
LPCSTR format_pps( LPSTR buf , size_t buflen , ubigint pps , int number_of_float /*=2*/ , LPCSTR unit_name /*= "pps"*/);


//----time functions------------------------------------------------------
void format_clock_time( const struct timespec * ts , LPSTR buffer , size_t buf_size );
void round_up_to_next_interval( struct timespec * now , int min_val , int interval , struct timespec * result ); // use min as least value to sleep and interval to round up to iteration of that value
void format_elapsed_time( time_t start , time_t end , LPSTR buffer , size_t buf_size );
void format_elapsed_time_with_millis( struct timeval start , struct timeval end , LPSTR buffer , size_t buf_size , int type /*0 dd:hh:mm:ss:ms , 1 ss.ms*/ );

//long long timespec_diff_nsec( struct timespec * start , struct timespec * end ); // difference in nanoseconds
//double timespec_diff_ms( struct timespec * start , struct timespec * end ); // difference in milliseconds
//double timespec_diff_sec( struct timespec * start , struct timespec * end ); // difference in seconds
//struct timespec timespec_sub( struct timespec start , struct timespec end );
//struct timespec timeval_diff_timespec( struct timeval start , struct timeval end );
long long timeval_diff_nsec( struct timeval * start , struct timeval * end );

//----file functions------------------------------------------------------
FILE* create_unique_file(LPCSTR path, LPCSTR filename /*=NULL(app+date)*/ );
LPCSTR read_file( LPCSTR path , buffer pInBuffer /*= if NULL alloc memory so release deligate to caller*/ );


//----socket functions------------------------------------------------------
void _close_socket( sockfd * socket_id );
status sendall( sockfd socketfd , buffer buf , size_t * len );
int peerTcpClosed( sockfd socketfd );


//----error functions------------------------------------------------------
_WEAK_ATTR void M_showMsg( LPCSTR msg );
LPCSTR internalErrorStr( status errValue );
status internalErrorVal( LPCSTR errStr );
LPCSTR make_msg_appnd_sys_err( LPSTR msg_holder , size_t size_of_msg_holder , LPCSTR cst_msg );


//----String functions------------------------------------------------------
LPSTR newStr(LPCSTR); // donot use it directly use NEWSTR instead

//LPCSTR __msg( LPSTR msg_holder , size_t size_of_msg_holder , LPCSTR msg , int line_number );
LPCSTR __snprintf( LPSTR msg_holder , size_t size_of_msg_holder , LPCSTR format , ... );

IN_GENERAL void convertChr( LPCSTR str , LPCSTR from , LPCSTR to ); // Written By Mohsen
IN_GENERAL void replaceChr( char fromChar , char toChar , LPCSTR str , size_t sz /*= -1*/ ); // Written By Mohsen

//IN_GENERAL void_p removeChr( void_p const str /*in out*/ , char chr , int sz /*in*/ , int * const pSz /*out*/ ); // Written By Mohsen
//IN_GENERAL void_p removeiChr( void_p const str /*in out*/ , char chr , int sz /*in*/ , int * const pSz /*out*/ ); // Written By Mohsen
//IN_GENERAL void_p removeChrs( void_p const sMem /*in out*/ , size_t memSz /*in*/ , size_t * const pSz /*out*/ , void_p const chrs /*in*/ , size_t chrsCount /*in*/ ); // Written By Mohsen . 1390/06/03
//IN_GENERAL void_p removeiChrs( void_p const sMem /*in out*/ , size_t memSz /*in*/ , size_t * const pSz /*out*/ , void_p const chrs /*in*/ , size_t chrsCount /*in*/ ); // Written By Mohsen . 1390/06/03

//IN_GENERAL void_p serializeChrs( void_p const sMem /*in out*/ , size_t memSz /*in*/ , size_t * const pSz /*out*/ , void_p const chrs /*in*/ , size_t chrsCount /*in*/ ); // 1390/06/03
//IN_GENERAL void_p serializeiChrs( void_p const sMem /*in out*/ , size_t memSz /*in*/ , size_t * const pSz /*out*/ , void_p const chrs /*in*/ , size_t chrsCount /*in*/ ); // 1390/06/03

IN_GENERAL int fstrcmp( LPCSTR str1 , LPCSTR str2 ); // Farsi Cmp . Written By Mohsen
IN_GENERAL LPCSTR stristr( LPCSTR sSrc , LPCSTR sDest ); // case insensitive cmp . Written By Mohsen
IN_GENERAL LPCSTR rstrstr( LPCSTR sStr , LPCSTR sSubStr ); // reverse search . Written By Mohsen
IN_GENERAL LPCSTR rstristr( LPCSTR sStr , LPCSTR sSubStr ); // reverse search . case insensitive cmp . Written By Mohsen

// strstrs(s)
IN_GENERAL LPCSTR strstrs( LPCSTR sSrc , int * const pISubStr , int count , LPCSTR * const strs ); // Written By Mohsen
IN_GENERAL LPCSTR stristrs( LPCSTR sSrc , int * const pISubStr , int count , LPCSTR * const strs ); // Written By Mohsen
//IN_GENERAL LPCSTR strstrs( LPCSTR sSrc , int * const pISubStr , int count , ... ); // Written By Mohsen
//IN_GENERAL LPCSTR stristrs( LPCSTR sSrc , int * const pISubStr , int count , ... ); // Written By Mohsen
//IN_GENERAL LPCSTR strstrs( LPCSTR sSrc , int count , ... ); // Written By Mohsen
//IN_GENERAL LPCSTR stristrs( LPCSTR sSrc , int count , ... ); // Written By Mohsen
//IN_GENERAL LPCSTR strstrs( LPCSTR sSrc , LPCSTR doubleStrBuf , int * const pISubStr /*= NULL*/ ); // Written By Mohsen
//IN_GENERAL LPCSTR stristrs( LPCSTR sSrc , LPCSTR doubleStrBuf , int * const pISubStr /*= NULL*/ ); // Written By Mohsen

IN_GENERAL int strsstr( LPCSTR * strs , int strs_count , LPCSTR target );
IN_GENERAL int strsistr( LPCSTR * strs , int strs_count , LPCSTR target );

// ---- chr --------------------------------------------------------------------------------
//IN_GENERAL LPCSTR strchr( LPCSTR str , char c ); // Exist
// n , i , r
IN_GENERAL LPCSTR strnchr( LPCSTR str , char c , int n ); // n(th) chr . Written By Mohsen
IN_GENERAL LPCSTR strichr( LPCSTR str , char c ); // case insensitive cmp . Written By Mohsen
//IN_GENERAL LPCSTR strrchr( LPCSTR str , char c ); // Exist
// ni , ri , nr
//IN_GENERAL LPCSTR strinchr( LPCSTR str , char c , int n ); // n(th) chr . case insensitive cmp . Written By Mohsen
//IN_GENERAL LPCSTR strrichr( LPCSTR str , char c ); // reverse search . case insensitive cmp . Written By Mohsen
//IN_GENERAL LPCSTR strrnchr( LPCSTR str , char c , int n ); // Written By Mohsen
// nir
IN_GENERAL LPCSTR strrinchr( LPCSTR str , char c , int n ); // Written By Mohsen

// ---- chrs --------------------------------------------------------------------------------
IN_GENERAL LPCSTR strchrs( LPCSTR str , LPCSTR chrs , int * const pCI /*= NULL*/ ); // Written By Mohsen
// n , i , r
//IN_GENERAL LPCSTR strnchrs( LPCSTR str , int n , LPCSTR chrs , int * const pCI /*= NULL*/ ); // n(th) chr . Written By Mohsen
//IN_GENERAL LPCSTR strichrs( LPCSTR str , LPCSTR chrs , int * const pCI /*= NULL*/ ); // case insensitive cmp . Written By Mohsen
//IN_GENERAL LPCSTR strrchrs( LPCSTR str , LPCSTR chrs , int * const pCI /*= NULL*/ ); // Written By Mohsen
// ni , ri , nr
//IN_GENERAL LPCSTR strinchrs( LPCSTR str , int n , LPCSTR chrs , int * const pCI /*= NULL*/ ); // n(th) chr . case insensitive cmp . Written By Mohsen
//IN_GENERAL LPCSTR strrichrs( LPCSTR str , LPCSTR chrs , int * const pCI /*= NULL*/ ); // reverse search . case insensitive cmp . Written By Mohsen
//IN_GENERAL LPCSTR strrnchrs( LPCSTR str , int n , LPCSTR chrs , int * const pCI /*= NULL*/ ); // Written By Mohsen
// nir
//IN_GENERAL LPCSTR strrinchrs( LPCSTR str , int n , LPCSTR chrs , int * const pCI /*= NULL*/ ); // Written By Mohsen

IN_GENERAL LPCSTR strihead( LPCSTR str , LPCSTR head );
//IN_GENERAL LPCSTR stritail( LPCSTR str , LPCSTR head );

//----buffer functions------------------------------------------------------
void buff_fill_seq( buffer buf , size_t size );
void dump_buffer( const buffer buf , size_t size );
uint8 hash8_fnv1a_avalanche( const char * s );
ulong hash( LPCSTR str );
