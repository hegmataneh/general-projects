#define Uses_STRDUP
#define Uses_dict_s_s_t
#define Uses_INIT_BREAKABLE_FXN
#define Uses_MEMSET_ZERO_O
#define Uses_strings_ar
#include <general.dep>

//-------------------------------------------------------------------------
const char EOS = '\0'; // 0
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


//-------------------------------------------------------------------------
void init_string_ar( strings_ar * list )
{
	MEMSET_ZERO_O( list );
	list->size = 0;
	list->capacity = AUTO_MODERATE_BUFFER_CAPACITY;
	list->strs = MALLOC_AR( list->strs , list->capacity );
	// TODO . err check
}

status addTo_string_ar( strings_ar * list , LPCSTR str )
{
	INIT_BREAKABLE_FXN();

	if ( list->size >= list->capacity )
	{
		list->capacity += AUTO_MODERATE_BUFFER_CAPACITY;
		REALLOC_AR_SAFE( list->strs , list->capacity ); 
		N_BREAK_IF( !list->strs , errMemoryLow , 1 );
	}
	N_BREAK_IF( !( list->strs[ list->size ] = STRDUP( str ) ) , errMemoryLow , 1 ); // duplicate string
	list->size++;

	BEGIN_SMPL
	N_END_RET
}

void free_string_ar( strings_ar * list )
{
	for ( size_t i = 0; i < list->size; i++ )
	{
		FREE( list->strs[ i ] );
	}
	FREE( list->strs );
	MEMSET_ZERO_O( list );
}

// General function: iterate array, collect strings
status collect_strings( void * array , size_t count , size_t elem_size ,
	fdl_geter_1 getter , strings_ar * out )
{
	INIT_BREAKABLE_FXN();
	init_string_ar( out );
	for ( size_t i = 0; i < count; i++ )
	{
		pass_p elem = ( buffer )array + i * elem_size;
		LPCSTR field = getter( elem );
		if ( field )
		{
			N_BREAK_STAT( addTo_string_ar( out , field ) , 1 );
		}
	}
	
	BEGIN_RET
	case 1: free_string_ar( out );
	N_END_RET
}

/// <summary>
/// free out by free_string_ar
/// </summary>
status collect_strings_itr( void * arr , size_t count , fdl_geter_2 getter , strings_ar * out )
{
	INIT_BREAKABLE_FXN();
	init_string_ar( out );
	for ( size_t i = 0; i < count; i++ )
	{
		LPCSTR field = getter( arr , i );
		if ( field )
		{
			N_BREAK_STAT( addTo_string_ar( out , field ) , 1 );
		}
	}

	BEGIN_RET
	case 1: free_string_ar( out );
	N_END_RET
}

/// <summary>
/// free outy by free_string_ar
/// </summary>
status strs_distinct( strings_ar * inp , strings_ar * outy )
{
	INIT_BREAKABLE_FXN();

	dict_s_s_t dc;
	BREAK_STAT( dict_init( &dc ) , 0 );
	for ( int i = 0 ; i < inp->size ; i++ )
	{
		BREAK_STAT( dict_put( &dc , inp->strs[ i ] , inp->strs[ i ] ) , 0 );
	}
	BREAK_STAT( dict_get_keys_strings( &dc , outy ) , 1 );
	dict_free( &dc );

	BEGIN_RET
	case 1 :
	{
		dict_free( &dc );
		break;
	}
	N_END_RET
}

