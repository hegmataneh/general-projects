//#pragma once

#if defined Uses_M_SERIES_MACROS || !defined __COMPILING


#ifndef private

//#ifndef __ENGINE  // applications must provide a way to show the error occured
//
//	_IMPORT void M_mkShowMsg(LPCSTR fn,int ln,LPCSTR msg,status error);
//	_IMPORT void M_showMsg();
//
//#endif

#define M_MK_ERR_MSG(msg,echo) do {\
	make_msg_appnd_sys_err( __custom_message , sizeof( __custom_message ) , __snprintf( __custom_message , sizeof( __custom_message ) , "(ln%d)-%s" , __LINE__ , ( msg ? msg : "" ) ) ); } while ( 0 )

#define M_MK_ERR_FMT_MSG(fmt,...) do {\
	make_msg_appnd_sys_err( __custom_message , sizeof( __custom_message ) , __snprintf( __custom_message , sizeof( __custom_message ) , "(ln%d)-" fmt , __LINE__ , __VA_ARGS__ ) ); } while ( 0 )

	
#ifdef __ENGINE // app must provide def for M_MSG
	#define M_MSG {\
		M_showMsg(); }
#endif

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
//#define MME_BREAK_STAT(err,lvl,msg,echo)
//	do {
//		d_error=(status)err;
//		if(d_error!=errOK)
//		{
//			M_MK_ERR_MSG(msg,echo);
//			_ErrLvl=lvl;
//			goto __ret;
//		}
//	} while(0)

#endif


#ifndef public

#define INIT_BREAKABLE_FXN() \
	status d_error = errOK;  /*c does not have class and data member*/\
	uchar _ErrLvl = 0; \
	char __custom_message[ 256 ] = "";

#define BREAK_OK(lvl)\
	do {\
		d_error=(status)errOK;\
		_ErrLvl=lvl;\
		goto __ret;\
	} while(0)

//#define BREAK(err,lvl) MME_BREAK(err,lvl,NULL,False)
#define BREAK_IF(cond,err,lvl) MME_BREAK_IF(cond,err,lvl,NULL,False) /*semicolon after this macro*/
//#define BREAK_STAT(err,lvl) MME_BREAK_STAT(err,lvl,NULL,False)

// N: indicates that this must give a message (is indeed of type M_ series macros), but since this message iterates a lot, we want to temporarily disable it. at last we must get no message if we convert them back to M_.
//#define N_BREAK BREAK
#define N_BREAK_IF BREAK_IF
//#define N_BREAK_STAT BREAK_STAT

// when giving the default message is enough and no additional msg is required
//#define M_BREAK(err,lvl) MME_BREAK(err,lvl,NULL,True)
#define M_BREAK_IF(cond,err,lvl) MME_BREAK_IF(cond,err,lvl,NULL,True) /*semicolon after this macro*/
//#define M_BREAK_STAT(err,lvl) MME_BREAK_STAT(err,lvl,NULL,True)

//#define MM_BREAK(err,lvl,msg) MME_BREAK(err,lvl,msg,True)
#define MM_BREAK_IF(cond,err,lvl,msg) MME_BREAK_IF(cond,err,lvl,msg,true) /*semicolon after this macro*/
#define MM_FMT_BREAK_IF(cond,err,lvl,fmt,...) MME_FMT_BREAK_IF(cond,err,lvl,fmt,__VA_ARGS__) /*semicolon after this macro*/
//#define MM_BREAK_STAT(err,lvl,msg) MME_BREAK_STAT(err,lvl,msg,True)

// begin ret and series
//#define BEGIN_RET_CLOCK d_error=errOK;
#define BEGIN_RET d_error=errOK; _ErrLvl=0; while( 0 ) { goto __ret; };  __ret: status ___localError=d_error; switch(_ErrLvl) {
#define BEGR(lvl) d_error=errOK; _ErrLvl=lvl; while( 0 ) { goto __ret; }; __ret: status ___localError=d_error; switch(_ErrLvl) { case lvl:

#define V_END_RET } d_error=___localError;
#define END_RET V_END_RET return d_error;
//#define B_END_RET V_END_RET return ToBoolean(d_error==errOK);

#define M_V_END_RET V_END_RET M_MSG
#define M_END_RET M_V_END_RET return d_error;
//#define M_B_END_RET M_V_END_RET return ToBoolean(d_error==errOK);

// no case
#define V_RET d_error=errOK; while( 0 ) { goto __ret; }; __ret: ;
#define RET V_RET return d_error;
//#define B_RET V_RET return ToBoolean(d_error==errOK);

#define M_V_RET V_RET M_MSG
#define M_RET M_V_RET return d_error;
//#define M_B_RET M_V_RET return ToBoolean(d_error==errOK);

//#define ASSERT_NO_ERROR
//	V_RET
//	ASSERT(d_error==errOK);
//
//#define ASSERT_ERROR
//	ASSERT_NO_ERROR
//	return d_error;

#define NEWSTR(var,str,lvl)\
	M_BREAK_IF(!(var=newStr(str)),errMemoryLow,lvl)

#endif

#endif  // Uses_M_SERIES_MACROS

