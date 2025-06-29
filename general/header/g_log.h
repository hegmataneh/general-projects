#if defined USES_Log || !defined __COMPILING


// init log file
//void InitLog(const char *path, Boolean isAppend = False );
//void LogTagEnable(GeLogTag lvl, bool enable);

// log to file
//void dbgLog( LPCSTR fmt , ... ); // moshen
//void Log(GeLogTag lvl,const char *fmt,...);
//void LogHexBuff(GeLogTag lvl,void *buffer,int size);
#endif

//typedef void (*GtDbgMsg)(LPCSTR);
// overrides the default implementation of dbgMsg. returns the previous implementation.
//GtDbgMsg setDbgMsg(GtDbgMsg newDbgMsg);

#ifdef UNDER_CE
//_EXPORT void dbg(const char *file,int line);
//_EXPORT void dbg(float n,const char *func);
//_EXPORT void dbg(const char *func,LPCSTR fmt,...);
//_EXPORT void dbg_end(const char *func);

#else  // assuming __FUNCTION__ is available
// _ as prefix indicates not to be used directly
//void _dbg_fp_li( LPCSTR filePath , int line );
//void _dbg_n_fp_fxn( double n , LPCSTR filePath , LPCSTR func );
//void _dbg_fmt_fp_fxn( LPCSTR fmt , va_list , LPCSTR filePath , LPCSTR func );
//void _dbg_fp_fxn_fmt( LPCSTR filePath , LPCSTR func , LPCSTR fmt , ... );
//
//#define DBG _dbg_fp_li(__FILE__,__LINE__);
//#define DBG_FL DBG
//#define DBG_N(n) _dbg_n_fp_fxn(n,__FILE__,__FUNCTION__)
//
//#define DBG0(str) _dbg_fp_fxn_fmt(__FILE__,__FUNCTION__,str)
//#define DBG1(fmt,arg1) _dbg_fp_fxn_fmt(__FILE__,__FUNCTION__,fmt,arg1)
//#define DBG2(fmt,arg1,arg2) _dbg_fp_fxn_fmt(__FILE__,__FUNCTION__,fmt,arg1,arg2)
//#define DBG3(fmt,arg1,arg2,arg3) _dbg_fp_fxn_fmt(__FILE__,__FUNCTION__,fmt,arg1,arg2,arg3)
//#define DBG4(fmt,arg1,arg2,arg3,arg4) _dbg_fp_fxn_fmt(__FILE__,__FUNCTION__,fmt,arg1,arg2,arg3,arg4)
//#define DBG5(fmt,arg1,arg2,arg3,arg4,arg5) _dbg_fp_fxn_fmt(__FILE__,__FUNCTION__,fmt,arg1,arg2,arg3,arg4,arg5)
//#define DBG6(fmt,arg1,arg2,arg3,arg4,arg5,arg6) _dbg_fp_fxn_fmt(__FILE__,__FUNCTION__,fmt,arg1,arg2,arg3,arg4,arg5,arg6)
//#define DBG7(fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7) _dbg_fp_fxn_fmt(__FILE__,__FUNCTION__,fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7)
//#define DBG8(fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8) _dbg_fp_fxn_fmt(__FILE__,__FUNCTION__,fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8)
//#define DBG9(fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9) _dbg_fp_fxn_fmt(__FILE__,__FUNCTION__,fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9)
//#define DBG10(fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10) _dbg_fp_fxn_fmt(__FILE__,__FUNCTION__,fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)
//#define DBG11(fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11) _dbg_fp_fxn_fmt(__FILE__,__FUNCTION__,fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11)
//#define DBG12(fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12) _dbg_fp_fxn_fmt(__FILE__,__FUNCTION__,fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12)

#endif

// Sequence of use
// 1. InitLog
// 2. LogTagEnable . it is optional
// 3. use these fxn ( dbgLog , Log , LogHexBuff , DBG , DBG_FL , DBG_N , DBG0 , ... )
