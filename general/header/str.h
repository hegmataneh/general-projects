#pragma once

#if defined Uses_strings_ar || !defined __COMPILING

// Function pointer type for callback
// Takes a pointer to one struct, returns the field string you want
typedef LPCSTR ( *fdl_geter_1 )( const pass_p element );
typedef LPCSTR ( *fdl_geter_2 )( const pass_p arr , size_t i );

// Dynamic string list
typedef struct
{
	strings strs;
	size_t size;
	size_t capacity;
} strings_ar;

status init_string_ar( strings_ar * list );
status addTo_string_ar( strings_ar * list , LPCSTR str );
void free_string_ar( strings_ar * list );

status collect_strings( void * array , size_t count , size_t elem_size , fdl_geter_1 getter , strings_ar * out );
status collect_strings_itr( void * array , size_t count , fdl_geter_2 getter , strings_ar * out );

// free outy by free_string_ar
status strs_distinct( strings_ar * inp , strings_ar * outy );

#endif

#ifdef Uses_TO_STRING

#define TO_STRING(x) _Generic((x), \
    int: int_to_string, \
    long: long_to_string, \
    float: float_to_string, \
    double: double_to_string, \
    char *: string_copy, \
    const char *: string_copy, \
    default: unknown_to_string \
)(x)

static char * int_to_string( int v )
{
	static char buf[ 32 ];
	snprintf( buf , sizeof( buf ) , "%d" , v );
	return buf;
}

static char * long_to_string( long v )
{
	static char buf[ 32 ];
	snprintf( buf , sizeof( buf ) , "%ld" , v );
	return buf;
}

static char * float_to_string( float v )
{
	static char buf[ 32 ];
	snprintf( buf , sizeof( buf ) , "%f" , v );
	return buf;
}

static char * double_to_string( double v )
{
	static char buf[ 32 ];
	snprintf( buf , sizeof( buf ) , "%lf" , v );
	return buf;
}

static char * string_copy( const char * v )
{
	return ( char * )v; // already a string
}

static char * unknown_to_string( void * v )
{
	static char buf[ 32 ];
	snprintf( buf , sizeof( buf ) , "%p" , v );
	return buf;
}

#endif
