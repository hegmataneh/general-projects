#pragma once

#if defined Uses_ddlck || !defined __COMPILING /*at scope of compiler each source must use it*/

#if defined ENABLE_LOCK_TRACING || !defined __COMPILING

#define DD_MAX_FILE 40
#define DD_LINE_COUNT 700

#pragma GCC diagnostic ignored "-Wunused-variable"

#ifndef IN_DDLCK_H // in other source they prepare prerequisit
	extern int __tu_global_counter;
	static int __tu_file_counter = 0; /*each source has one instance*/
	static int __tu_line_counter = 0; /*each source has one instance*/
#endif

#define DD_EACH_FILE_INIT	__attribute__((constructor)) \
							static void ___code_init( void )\
							{\
								__tu_file_counter = __tu_global_counter;\
								__tu_global_counter++;\
							}

#ifndef IN_DDLCK_H // in other source they prepare prerequisit
	DD_EACH_FILE_INIT;
#endif

typedef struct
{
	const char * fxn;
	long line;
	long state;

	long last_line;
} ddlck_t;

typedef ddlck_t _tmp_arr_type_[ DD_MAX_FILE ][ DD_LINE_COUNT ];

extern _tmp_arr_type_ __lck_hit;

#ifdef LOCK_LINE
#undef LOCK_LINE
#endif
#define LOCK_LINE( exp )	({ \
							ushort ___line = __LINE__; \
							if ( ___line >= DD_LINE_COUNT ) ___line = DD_LINE_COUNT - 1; \
							__lck_hit[ __tu_file_counter ][ ___line ].fxn = __FUNCTION__; \
							__lck_hit[ __tu_file_counter ][ ___line ].line = ___line; \
							__lck_hit[ __tu_file_counter ][ ___line ].state++; \
							for ( size_t t = 0 ; t < DD_LINE_COUNT ; t++ ) \
								__lck_hit[ __tu_file_counter ][ t ].last_line = ___line; \
							exp; \
							__lck_hit[ __tu_file_counter ][ ___line ].state--; \
							})


#endif // ENABLE_LOCK_TRACING

#endif // Uses_ddlck

