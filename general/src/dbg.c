#define IN_DBG_H
#define Uses_INTERNAL_ERR

#define Uses_DBG
#include <general.dep>

//_GLOBAL_VAR short __local_err_val = NEXT_GENERAL_ERROR_VALUE;
_GLOBAL_VAR internal_err_text_t __local_err_strs[ _internal_err_count ] =
{
	DEF_LCL_ERR( "errOK" )			,DEF_LCL_ERR( "errGeneral" )		,DEF_LCL_ERR( "MemoryLow" )			,DEF_LCL_ERR( "InvalidString" ),
	DEF_LCL_ERR( "Canceled" )		,DEF_LCL_ERR( "syntax error" )		,DEF_LCL_ERR( "invalid argument" )	,DEF_LCL_ERR( "timed out" ),
	DEF_LCL_ERR( "peer closed" )	,DEF_LCL_ERR( "OutofRanje" )		,DEF_LCL_ERR( "MaximumExceeded" )	,DEF_LCL_ERR( "NoPeer" ),
	DEF_LCL_ERR( "NotFound" )		,DEF_LCL_ERR( "errDevice")			,DEF_LCL_ERR( "errSocket" )			,DEF_LCL_ERR( "errCreation" ),
	DEF_LCL_ERR( "errOverflow" )	,DEF_LCL_ERR( "errCorrupted" )		,DEF_LCL_ERR( "errResource" )		,DEF_LCL_ERR( "errPath" ),
	DEF_LCL_ERR( "errRetry" )		,DEF_LCL_ERR( "errEmpty" )			,DEF_LCL_ERR( "errTooManyAttempt" )	,DEF_LCL_ERR( "errShutdown" ),
	DEF_LCL_ERR( "errBind" )		,DEF_LCL_ERR( "errsockopt" )		,DEF_LCL_ERR( "errListen" )			,DEF_LCL_ERR( "errSelect" ),
	DEF_LCL_ERR( "errAccept" )		,DEF_LCL_ERR( "errNoConnection" )	,DEF_LCL_ERR( "errConnect" )		,DEF_LCL_ERR( "errOpen" ),
	DEF_LCL_ERR( "errACK" )			,DEF_LCL_ERR( "errNoCountinue" )	,DEF_LCL_ERR( "errMapped" )			,DEF_LCL_ERR( "errDoneAlready" ),
	DEF_LCL_ERR( "errButContinue" )	,DEF_LCL_ERR( "errPortOccupied" )
};

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

#if defined __USE_DBG_TOOLS || !defined __COMPILING

#ifdef ENABLE_USE_DBG_TAG

_GLOBAL_VAR int __tu_global_counter = 0;

_GLOBAL_VAR LPCSTR __map_c2idx[MAX_TU] = {0};

_GLOBAL_VAR int/*program count*/ __FXN_HIT[MAX_TU/*max TU count or c files*/][MAX_TU_LINES/*max lines count*/][BACKTRACK_COUNT/*n times of calling the line*/] = {0}; // max size is about number of code lines

_GLOBAL_VAR int __pc = 1; // step of each call globally . prog counter

#endif

#endif

#if defined ENABLE_USE_DBG_TAG

_GLOBAL_VAR char TMP_DUMP_BUFF_NAME[ 10000 ] = { 0 };
_GLOBAL_VAR int TMP_DUMP_BUFF_N = { 0 };

#endif
