#if defined USES_M_SERIES_MACROS || !defined __COMPILING

// give declarations of functions used by these macros
//void M_mkLogMsg(const char *fn,int ln,const char *msg,status error);
//void M_logMsg();

#ifndef __ENGINE  // applications must provide a way to show the error occured

	_IMPORT void M_mkShowMsg(const char *fn,int ln,const char *msg,status error);
	_IMPORT void M_showMsg();

#endif

#if !defined __ENGINE

	#define M_MK_ERR_MSG(msg,echo) do {\
		if(echo)M_mkShowMsg(__FILE__,__LINE__,msg,d_error);\
		M_mkLogMsg(__FILE__,__LINE__,msg,d_error); } while(0)

	#define M_MSG {\
		M_showMsg();\
		M_logMsg(); }

#else  // __ENGINE

	#define M_MK_ERR_MSG(msg,echo) M_mkLogMsg(__FILE__,__LINE__,msg,d_error)
	#define M_MSG M_logMsg();

#endif  // __ENGINE

#define BREAK_OK(lvl)\
	do {\
		d_error=(status)errOK;\
		_ErrLvl=lvl;\
		goto __ret;\
	} while(0)


// M: give a message, M: and add the specified msg, E: and echo it on the screen
#define MME_BREAK(err,lvl,msg,echo)\
	do {\
		d_error=(status)err;\
		M_MK_ERR_MSG(msg,echo);\
		_ErrLvl=lvl;\
		goto __ret;\
	} while(0)

#define MME_BREAK_IF(cond,err,lvl,msg,echo)\
	do {\
		MME_BREAK_IF_CALL_APPEND\
		int _cond=int(cond);\
		MME_BREAK_IF_CALL_CROP\
		if (_cond)\
			MME_BREAK(err,lvl,msg,echo);\
	} while(0)

/* ageh err function call nabasheh, append o crop ham lazem nist anjam besheh, vali khob
 * ma keh nemitunim in ro tashkhis bedim.
 */
#define MME_BREAK_STAT(err,lvl,msg,echo)\
	do {\
		MME_BREAK_STAT_CALL_APPEND\
		d_error=(status)err;\
		MME_BREAK_STAT_CALL_CROP\
		if(d_error!=errOK)\
		{\
			M_MK_ERR_MSG(msg,echo);\
			_ErrLvl=lvl;\
			goto __ret;\
		}\
	} while(0)

#define BREAK(err,lvl) MME_BREAK(err,lvl,NULL,False)
#define BREAK_IF(cond,err,lvl) MME_BREAK_IF(cond,err,lvl,NULL,False)
#define BREAK_STAT(err,lvl) MME_BREAK_STAT(err,lvl,NULL,False)

// N: indicates that this must give a message (is indeed of type M_ series macros), but since this message iterates a lot, we want to temporarily disable it. at last we must get no message if we convert them back to M_.
#define N_BREAK BREAK
#define N_BREAK_IF BREAK_IF
#define N_BREAK_STAT BREAK_STAT

// when giving the default message is enough and no additional msg is required
#define M_BREAK(err,lvl) MME_BREAK(err,lvl,NULL,True)
#define M_BREAK_IF(cond,err,lvl) MME_BREAK_IF(cond,err,lvl,NULL,True)
#define M_BREAK_STAT(err,lvl) MME_BREAK_STAT(err,lvl,NULL,True)

#define MM_BREAK(err,lvl,msg) MME_BREAK(err,lvl,msg,True)
#define MM_BREAK_IF(cond,err,lvl,msg) MME_BREAK_IF(cond,err,lvl,msg,True)
#define MM_BREAK_STAT(err,lvl,msg) MME_BREAK_STAT(err,lvl,msg,True)

// begin ret and series
#define BEGIN_RET d_error=errOK; _ErrLvl=0; __ret: status ___localError(d_error); switch(_ErrLvl) {
#define BEGR(lvl) d_error=errOK; _ErrLvl=lvl; __ret: status ___localError(d_error); switch(_ErrLvl) { case lvl:

#define V_END_RET } d_error=___localError;
#define END_RET V_END_RET return d_error;
#define B_END_RET V_END_RET return ToBoolean(d_error==errOK);

#define M_V_END_RET V_END_RET M_MSG
#define M_END_RET M_V_END_RET return d_error;
#define M_B_END_RET M_V_END_RET return ToBoolean(d_error==errOK);

// no case
#define V_RET d_error=errOK; __ret: ;
#define RET V_RET return d_error;
#define B_RET V_RET return ToBoolean(d_error==errOK);

#define M_V_RET V_RET M_MSG
#define M_RET M_V_RET return d_error;
#define M_B_RET M_V_RET return ToBoolean(d_error==errOK);

#define ASSERT_NO_ERROR\
	V_RET\
	ASSERT(d_error==errOK);

#define ASSERT_ERROR\
	ASSERT_NO_ERROR\
	return d_error;

#define NEWSTR(var,str,lvl)\
	M_BREAK_IF(!(var=newStr(str)),errMemoryLow,lvl)
#define DEF_NEWSTR(var,str,lvl)\
	char *var;\
	M_BREAK_IF(!(var=newStr(str)),errMemoryLow,lvl)
#define RET_NEWSTR(str,lvl) do {\
	char *tmpRetStr;\
	M_BREAK_IF(!(tmpRetStr=newStr(str)),errMemoryLow,lvl);\
	return tmpRetStr; } while(0)

#define NEWWSTR(var,str,lvl)\
	M_BREAK_IF(!(var=newStr(str)),errMemoryLow,lvl)
#define DEF_NEWWSTR(var,str,lvl)\
	wchar_t *var;\
	M_BREAK_IF(!(var=newStr(str)),errMemoryLow,lvl)
#define RET_NEWWSTR(str,lvl) do {\
	wchar_t *tmpRetStr;\
	M_BREAK_IF(!(tmpRetStr=newStr(str)),errMemoryLow,lvl);\
	return tmpRetStr; } while(0)

#endif  // USES_M_SERIES_MACROS

