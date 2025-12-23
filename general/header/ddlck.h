#pragma once

#if defined Uses_ddlck || !defined __COMPILING /*at scope of compiler each source must use it*/

#if defined ENABLE_LOCK_TRACING || !defined __COMPILING

#define DD_MAX_FILE 40 /*according to app c files*/
#define DD_LINE_COUNT 700 /*your app c file max lines count*/

/*
	to use this util just put important lock in the LOCK_LINE( expr )
*/

#pragma GCC diagnostic ignored "-Wunused-variable"

#ifndef IN_DDLCK_H // in other source they prepare prerequisit
	_EXTERN int __ddl_global_counter;
	static int __ddl_file_counter = 0; /*each source has one instance*/
	static int __ddl_line_counter = 0; /*each source has one instance*/
	_EXTERN ulong __ddl_call_seq_counter;
#endif

#define DD_EACH_FILE_INIT	__attribute__((constructor)) \
							static void ___code_init( void )\
							{\
								__ddl_file_counter = __ddl_global_counter;\
								__ddl_global_counter++;\
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

#define SEQ_TRACE_COUNTE 100000

_EXTERN _tmp_arr_type_ __lck_hit;
_EXTERN ulong __lck_lines_hit[SEQ_TRACE_COUNTE];

#ifdef LOCK_LINE
#undef LOCK_LINE
#endif

#define LOCK_LINE( exp /*put lockable line in this macro*/ )	({ \
							ushort ___line = __LINE__; \
							if ( ___line >= DD_LINE_COUNT ) ___line = DD_LINE_COUNT - 1; \
							__lck_hit[ __ddl_file_counter ][ ___line ].fxn = __FUNCTION__; \
							__lck_hit[ __ddl_file_counter ][ ___line ].line = ___line; \
							__lck_hit[ __ddl_file_counter ][ ___line ].state++; \
							ulong tmp_ddl = __ddl_call_seq_counter++; \
							__lck_lines_hit[ tmp_ddl % SEQ_TRACE_COUNTE/*first % then ++*/ ] = tmp_ddl; \
							for ( size_t t = 0 ; t < DD_LINE_COUNT ; t++ ) \
								__lck_hit[ __ddl_file_counter ][ t ].last_line = ___line; \
							exp; \
							__lck_hit[ __ddl_file_counter ][ ___line ].state--; \
							})

/*
because we have many aspect that could test in application so there should be some LOCK_LINE experties that help seperate these aspects
*/
#define NO_LOCK_LINE( exp ) exp

#define THREAD_LOCK_LINE( exp )		NO_LOCK_LINE( exp )
#define DICT_FST_LOCK_LINE( exp )	NO_LOCK_LINE( exp )
#define CIRBUF_INF_LOCK_LINE( exp ) NO_LOCK_LINE( exp )
#define CR_WND_LOCK_LINE( exp )		NO_LOCK_LINE( exp )
#define INSTANTANEOUSBPS_LOCK_LINE( exp )	NO_LOCK_LINE( exp )
#define NNC_LOCK_LINE( exp )				NO_LOCK_LINE( exp )
#define PUB_SUB_LOCK_LINE( exp )			NO_LOCK_LINE( exp )
#define TOKEN_RING_P_LOCK_LINE( exp )		NO_LOCK_LINE( exp )
#define CFG_LOCK_LINE( exp )				NO_LOCK_LINE( exp )

#define PG_STCK_LOCK_LINE( exp )	LOCK_LINE( exp )
#define VQUEUE_LOCK_LINE( exp )		LOCK_LINE( exp )
#define VSTACK_LOCK_LINE( exp )		LOCK_LINE( exp )
#define BP_LOCK_LINE( exp )			LOCK_LINE( exp )
#define PKT_MGR_LOCK_LINE( exp )	LOCK_LINE( exp )


#endif // ENABLE_LOCK_TRACING

#endif // Uses_ddlck

