#pragma once

#if defined Uses_MemLEAK || !defined __COMPILING

#if defined Uses_StackTrace || !defined __COMPILING

typedef struct
{
	const char * fxns[ 3 ];
	const char * files[ 3 ];
	int lines[ 3 ];
	size_t sz;
	long idx;
	char temp_buf[ CALLSTACK_LOG_LENGH ];

} bcktrc_t;

void stktrace_init( bcktrc_t * trc , size_t count );
void stktrace_fill( bcktrc_t * trc );

void stktrace_generate( bcktrc_t * trace ); // return valid stack trace

#endif

#endif

