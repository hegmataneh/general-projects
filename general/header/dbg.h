#pragma once


#if defined Uses_INTERNAL_ERR

typedef struct
{
	IMMORTAL_LPCSTR simple_text;
	IMMORTAL_LPCSTR local_sign_text;
} internal_err_text_t;

#define DEF_LCL_ERR( exp ) { ""exp"" , "[" ""exp"" "]" }

#ifndef IN_DBG_H
		
//_GLOBAL_VAR _EXTERN short __local_err_val;
_GLOBAL_VAR _EXTERN internal_err_text_t __local_err_strs[ _internal_err_count ];

#endif
#endif // Uses_INTERNAL_ERR

//#define __USE_DBG_TOOLS /*uncomment this to use dbg tools*/

#ifdef _DEBUG
#if defined Uses_DBG || !defined __COMPILING /*at scope of compiler each source must use it*/

	#if defined __USE_DBG_TOOLS || !defined __COMPILING /*manually turn it off*/

		/*set based on source count . max source size and backtrack we want*/
		#define MAX_TU 50
		#define MAX_TU_LINES 500
		#define BACKTRACK_COUNT 10 // first for hit count and last alwayz zero

		/* **** private **** */
		#ifndef IN_DBG_H // in other source they prepare prerequisit
			#define __GLOBAL_COUNTER _EXTERN int __tu_global_counter;
			_EXTERN int __pc;
			_EXTERN int/*program count*/ __FXN_HIT[MAX_TU/*max TU count or c files*/][MAX_TU_LINES/*max lines count*/][BACKTRACK_COUNT/*n times of calling the line*/];
			_EXTERN LPCSTR __map_c2idx[MAX_TU];
		#else
			#define __GLOBAL_COUNTER NULL_ACT /*_EXTERN int __tu_global_counter = 0;*/ /*translation unit or c file*/
		#endif

		#define __LOCAL_COUNTER static int __tu_local_counter = 0; /*each source has one instance*/

		#define __CODE_DBG_TOOLS_INIT __attribute__((constructor)) \
									static void ___code_init( void )\
									{\
										__tu_local_counter = __tu_global_counter;\
										__map_c2idx[ __tu_local_counter ] = get_filename(__FILE__);\
										__tu_global_counter++;\
									}
		/* ~~~~ private **** */


		/* **** public **** */

		/*at the begin of source code that you want to dbg*/
		#define CODE_DBG_TOOLS() __GLOBAL_COUNTER __LOCAL_COUNTER __CODE_DBG_TOOLS_INIT

		/*put it every place you want consider in dbg trace and see where are the hits sequences*/
		#define SYS_ALIVE_CHECK() \
			do {\
			int __function_line = __LINE__;\
			__FXN_HIT[ __tu_local_counter ][ __function_line ][ 0 ]++;\
			static int pc = 0; /*each line hit*/\
			__FXN_HIT[ __tu_local_counter ][ __function_line ][ 1 + pc++ ] = __pc++; pc %= ( BACKTRACK_COUNT - 1 );\
			} while ( 0 )

		/* ~~~~ public **** */

	#endif

	#if !defined __USE_DBG_TOOLS

		#define CODE_DBG_TOOLS()
		#define SYS_ALIVE_CHECK()

	#endif

#endif // Uses_DBG
#endif // _DEBUG

#if defined ENABLE_USE_DBG_TAG

#define TMP_DUMP_BUFF_NAME __arrr
#define TMP_DUMP_BUFF_N __arrr_n
 
#ifndef IN_DBG_H
	_GLOBAL_VAR _EXTERN char TMP_DUMP_BUFF_NAME[ 100000 ];
	_GLOBAL_VAR _EXTERN int TMP_DUMP_BUFF_N;
#endif

#ifndef ENABLE_LOGGING

#define MARK_START_THREAD() TMP_DUMP_BUFF_N += sprintf( TMP_DUMP_BUFF_NAME + TMP_DUMP_BUFF_N , "%s started %lu\n" , __FUNCTION__ , this_thread );
#define MARK_LINE() TMP_DUMP_BUFF_N += sprintf( TMP_DUMP_BUFF_NAME + TMP_DUMP_BUFF_N , "%s %d\n" , __FUNCTION__ , __LINE__ );

#define DBG_TAG_BASE( _ar , _ar_n , fmt , ... ) do { _ar_n+=snprintf((_ar) + (_ar_n), sizeof(_ar), (fmt), ##__VA_ARGS__); } while( 0 )

#else // defined ENABLE_LOGGING

#define MARK_START_THREAD() TMP_DUMP_BUFF_N += sprintf( TMP_DUMP_BUFF_NAME + TMP_DUMP_BUFF_N , "%s started %lu\n" , __FUNCTION__ , this_thread ); \
							log_write( LOG_INFO , "%s started %lu\n" , __FUNCTION__ , this_thread );

#define MARK_LINE() TMP_DUMP_BUFF_N += sprintf( TMP_DUMP_BUFF_NAME + TMP_DUMP_BUFF_N , "%s %d\n" , __FUNCTION__ , __LINE__ ); \
							log_write( LOG_INFO , "%s %d\n" , __FUNCTION__ , __LINE__ );

#define DBG_TAG_BASE( _ar , _ar_n , fmt , ... ) do { _ar_n+=snprintf((_ar) + (_ar_n), sizeof(_ar), (fmt), ##__VA_ARGS__); \
												log_write( LOG_INFO , (fmt "\n"), ##__VA_ARGS__ ); } while( 0 )

#endif // ~ENABLE_LOGGING

#define DBG_TAG( fmt , ... ) DBG_TAG_BASE( TMP_DUMP_BUFF_NAME , TMP_DUMP_BUFF_N , fmt , __VA_ARGS__ )
#define DBG_PT() DBG_TAG( "%s %d\n" , __FILE_shrtn( __FILE__ ) , __LINE__ )

#endif // ENABLE_USE_DBG_TAG
