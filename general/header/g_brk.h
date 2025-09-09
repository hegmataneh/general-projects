//#pragma once

#if defined Uses_M_SERIES_MACROS || !defined __COMPILING

/* NOTICE .
mohsen 14040618
-kernel function must not use M_BREAK because they do not have msg tools
-just inner app space function should show msg and outer must ignore to show because inner know whats the right msg
*/


#ifndef private

//#ifndef __ENGINE  // applications must provide a way to show the error occured
//
//	_IMPORT void M_mkShowMsg(LPCSTR fn,int ln,LPCSTR msg,status error);
//	_IMPORT void M_showMsg();
//
//#endif

#define EACH_FXN_MSG_HOLDER __custom_message

#define M_MK_ERR_MSG(msg,echo) do {\
		__snprintf( EACH_FXN_MSG_HOLDER , sizeof( EACH_FXN_MSG_HOLDER ) , "(%s:%d)-%s(%s)(%s)" ,\
			__FUNCTION_shrtn(__FUNCTION__) , __LINE__ , ( msg ? msg : "" ) , __conditional_internalErrorStr(d_error,msg) , systemErrorStr(NP) ); } while ( 0 )

#define M_MK_ERR_FMT_MSG(fmt,...) do {\
		__snprintf( EACH_FXN_MSG_HOLDER , sizeof( EACH_FXN_MSG_HOLDER ) , "(%s:%d)-" fmt "(%s)(%s)",\
			__FUNCTION_shrtn(__FUNCTION__) , __LINE__ , __VA_ARGS__ , __conditional_internalErrorStr(d_error,fmt) , systemErrorStr(NP) ); } while ( 0 )

	
//#ifdef __ENGINE // app must provide def for M_MSG
	#define MM_MSG(msg) do {\
			M_showMsg( msg ); \
		} while(0)

	#ifdef M_MSG
		#undef M_MSG
	#endif

	#define M_MSG {\
		MM_MSG( EACH_FXN_MSG_HOLDER ); }

	#define IF_M_MSG if ( d_error != errOK && d_error != one_time_echo_err ){\
		M_MSG; d_error = one_time_echo_err; }
//#endif

// M: give a message, M: and add the specified msg, E: and echo it on the screen
#define MME_BREAK(err,lvl,msg,echo)\
	do {\
		d_error=(status)err;\
		M_MK_ERR_MSG(msg,echo);\
		_ErrLvl=lvl;\
		goto __ret;\
	} while(0)
#define MME_FMT_BREAK(err,lvl,fmt,...)\
	do {\
		d_error=(status)err;\
		M_MK_ERR_FMT_MSG(fmt,__VA_ARGS__);\
		_ErrLvl=lvl;\
		goto __ret;\
	} while(0)


#define MME_BREAK_IF(cond,err,lvl,msg,echo)\
	do {\
		int _cond=(int)(cond);\
		if (_cond)\
			MME_BREAK(err,lvl,msg,echo);\
	} while(0)

#define MME_FMT_BREAK_IF(cond,err,lvl,fmt,...)\
	do {\
		int _cond=(int)(cond);\
		if (_cond)\
			MME_FMT_BREAK(err,lvl,fmt,__VA_ARGS__);\
	} while(0)

/* ageh err function call nabasheh, append o crop ham lazem nist anjam besheh, vali khob
 * ma keh nemitunim in ro tashkhis bedim.
 */
#define MME_BREAK_STAT(err,lvl,msg,echo) \
do \
{ \
	d_error = ( status )err; \
	if ( d_error != errOK ) \
	{ \
		M_MK_ERR_MSG( msg , echo ); \
		_ErrLvl = lvl; \
		goto __ret; \
	} \
} while ( 0 )

#endif


#ifndef public

#define INIT_BREAKABLE_FXN() \
	status d_error = errOK;  /*c does not have class and data member*/\
	int _ErrLvl = 0; \
	char EACH_FXN_MSG_HOLDER [ 256 ] = "";

#define BREAK_OK(lvl)\
	do {\
		d_error=(status)errOK;\
		_ErrLvl=lvl;\
		goto __ret;\
	} while(0)

#define BREAK(err,lvl) MME_BREAK(err,lvl,NULL,False)
#define BREAK_IF(cond,err,lvl) MME_BREAK_IF(cond,err,lvl,NULL,False) /*semicolon after this macro*/
#define BREAK_STAT(err,lvl) MME_BREAK_STAT(err,lvl,NULL,False)

// N: indicates that this must give a message (is indeed of type M_ series macros), but since this message iterates a lot, we want to temporarily disable it. at last we must get no message if we convert them back to M_.
#define N_BREAK BREAK
#define N_BREAK_IF BREAK_IF
#define N_BREAK_STAT BREAK_STAT

#if !defined( __ENGINE ) || !defined __COMPILING // kernel does not have any tools to show msg

// when giving the default message is enough and no additional msg is required
#define M_BREAK(err,lvl) MME_BREAK(err,lvl,NULL,True)
#define M_BREAK_IF(cond,err,lvl) MME_BREAK_IF(cond,err,lvl,NULL,True) /*semicolon after this macro*/
#define M_BREAK_STAT(err,lvl) MME_BREAK_STAT(err,lvl,NULL,True)

//#define MM_BREAK(err,lvl,msg) MME_BREAK(err,lvl,msg,True)
#define MM_BREAK_IF(cond,err,lvl,msg) MME_BREAK_IF(cond,err,lvl,msg,true) /*semicolon after this macro*/
#define MM_FMT_BREAK_IF(cond,err,lvl,fmt,...) MME_FMT_BREAK_IF(cond,err,lvl,fmt,__VA_ARGS__) /*semicolon after this macro*/
#define MM_BREAK_STAT(err,lvl,msg) MME_BREAK_STAT(err,lvl,msg,True)

#endif

#define BGR_DUMMY_WHILE  while( 0 ) { if ( _ErrLvl ) goto __ret; } /*just to ignore warning*/
#define BGR_BEFORE_ACTION_PART  BGR_DUMMY_WHILE;  __ret: while(0) {}; status ___localError=d_error

// begin ret and series
//#define BEGIN_RET_CLOCK d_error=errOK;
#define BEGIN_SMPL d_error=errOK; _ErrLvl=0; BGR_BEFORE_ACTION_PART; while(0) {
#define BEGIN_RET d_error=errOK; _ErrLvl=0; BGR_BEFORE_ACTION_PART; switch(_ErrLvl) {
#define BEGR(lvl) d_error=errOK; _ErrLvl=lvl; BGR_BEFORE_ACTION_PART; switch(_ErrLvl) { case lvl:

// if inner buggy fxn show msg then horra if not, outer caller just can through d_error msg

#define V_END_RET } d_error=___localError;
#define END_RET V_END_RET return d_error;
//#define B_END_RET V_END_RET return ToBoolean(d_error==errOK);

#define N_V_END_RET V_END_RET
#define N_END_RET N_V_END_RET return d_error;

#if !defined( __ENGINE ) || !defined __COMPILING // kernel does not have any tools to show msg

#define M_V_END_RET V_END_RET IF_M_MSG
#define M_END_RET M_V_END_RET return d_error;
//#define M_B_END_RET M_V_END_RET return ToBoolean(d_error==errOK);

#endif

// no case
#define V_RET d_error=errOK; BGR_DUMMY_WHILE; __ret: ;
#define RET V_RET return d_error;
//#define B_RET V_RET return ToBoolean(d_error==errOK);

#if !defined( __ENGINE ) || !defined __COMPILING // kernel does not have any tools to show msg

#define M_V_RET V_RET IF_M_MSG
#define M_RET M_V_RET return d_error;
//#define M_B_RET M_V_RET return ToBoolean(d_error==errOK);

#endif

//#define ASSERT_NO_ERROR
//	V_RET
//	ASSERT(d_error==errOK);
//
//#define ASSERT_ERROR
//	ASSERT_NO_ERROR
//	return d_error;

#define N_NEWSTR(ptr,str,lvl)\
	N_BREAK_IF(!(ptr=newStr(str)),errMemoryLow,lvl)

#define N_MALLOC_AR(ptr,count,lvl)\
	N_BREAK_IF(!(ptr=MALLOC_AR(ptr,count)),errMemoryLow,lvl)

#define N_MALLOC_ONE(ptr,lvl)\
	N_BREAK_IF(!(ptr=MALLOC_ONE(ptr)),errMemoryLow,lvl)

#if !defined( __ENGINE ) || !defined __COMPILING // kernel does not have any tools to show msg

#define NEWSTR(ptr,str,lvl)\
	M_BREAK_IF(!(ptr=newStr(str)),errMemoryLow,lvl)

#define M_MALLOC_AR(ptr,count,lvl)\
	M_BREAK_IF(!(ptr=MALLOC_AR(ptr,count)),errMemoryLow,lvl)

#define MM_MALLOC_AR(ptr,count,lvl,msg)\
	MM_BREAK_IF(!(ptr=MALLOC_AR(ptr,count)),errMemoryLow,lvl,msg)

#define M_MALLOC_ONE(ptr,lvl)\
	M_BREAK_IF(!(ptr=MALLOC_ONE(ptr)),errMemoryLow,lvl)

#define MM_MALLOC_ONE(ptr,lvl,msg)\
	MM_BREAK_IF(!(ptr=MALLOC_ONE(ptr)),errMemoryLow,lvl,msg)

#endif

#endif

#endif  // Uses_M_SERIES_MACROS

