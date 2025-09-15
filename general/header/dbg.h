#pragma once

#define __USE_DBG_TOOLS /*uncomment this to use dbg tools*/

#if defined Uses_DGB || !defined __COMPILING /*at scope of compiler each source must use it*/

#if defined __USE_DBG_TOOLS || !defined __COMPILING /*manually turn it off*/

/*set based on source count . max source size and backtrack we want*/
#define MAX_TU 50
#define MAX_TU_LINES 500
#define BACKTRACK_COUNT 10 // first for hit count and last alwayz zero

/*private*/
#ifndef IN_DGB_H // in other source they prepare prerequisit
#define __GLOBAL_COUNTER extern int __tu_global_counter;
extern int __pc;
extern int/*program count*/ __FXN_HIT[MAX_TU/*max TU count or c files*/][MAX_TU_LINES/*max lines count*/][BACKTRACK_COUNT/*n times of calling the line*/];
extern LPCSTR __map_c2idx[MAX_TU];
#else
#define __GLOBAL_COUNTER NULL_ACT /*extern int __tu_global_counter = 0;*/ /*translation unit or c file*/
#endif

#define __LOCAL_COUNTER static int __tu_local_counter = 0; /*each source has one instance*/

#define __CODE_DBG_TOOLS_INIT __attribute__((constructor)) \
							static void ___code_init( void )\
							{\
								__tu_local_counter = ++__tu_global_counter;\
							}
/*~private*/

/*public*/

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
/*~public*/

#endif

#if !defined __USE_DBG_TOOLS

#define CODE_DBG_TOOLS()
#define SYS_ALIVE_CHECK()

#endif

#endif

