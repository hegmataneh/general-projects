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

void init_string_ar( strings_ar * list );
status addTo_string_ar( strings_ar * list , LPCSTR str );
void free_string_ar( strings_ar * list );

status collect_strings( void * array , size_t count , size_t elem_size , fdl_geter_1 getter , strings_ar * out );
status collect_strings_itr( void * array , size_t count , fdl_geter_2 getter , strings_ar * out );

// free outy by free_string_ar
status strs_distinct( strings_ar * inp , strings_ar * outy );

#endif
