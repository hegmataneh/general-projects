#define IN_DBG_H

#define Uses_DBG
#include <general.dep>

_GLOBAL_VAR short _err = NEXT_GENERAL_ERROR_VALUE;
_GLOBAL_VAR LPCSTR errStrs[ _internal_err_count ] = { "errOK","errGeneral","MemoryLow","InvalidString","Canceled","syntax error","invalid argument","timed out",\
	"peer closed","OutofRanje","MaximumExceeded","NoPeer","NotFound","errDevice","errSocket","errCreation","errOverflow","errCorrupted","errResource","errPath",\
	"errRetry","errEmpty","errTooManyAttempt","errShutdown","errBind","errsockopt","errListen","errSelect","errAccept","errNoConnection","errConnect","errOpen",\
	"errACK"};

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
