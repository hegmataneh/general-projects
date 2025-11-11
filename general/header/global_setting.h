#pragma once

/* at .inc time this file has environment effect so be carefull */

//#define Uses_MemLEAK
//#define TRACE_MEMORY_LEAK
//#define Uses_StackTrace
//#define __INC_StackTrace

//#define ENABLE_LOCK_TRACING
//#define Uses_LOCK_LINE


#define ENABLE_USE_DBG_TAG /*in some c file there is diagnostic var*/

#ifdef _DEBUG
	#define ENABLE_USE_DBG_TAG /*there is buffer that keep trace of some mark and checkpoint*/
#endif

#ifdef _DEBUG
	//#define ENABLE_LOGGING
#endif

