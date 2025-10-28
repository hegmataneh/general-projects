#pragma once

#if defined(NDEBUG) && ( defined(DEBUG) || defined(_DEBUG) )
#error "this condition is not allowed"
#endif

#if !defined(NDEBUG) && !defined(_DEBUG)
#define _DEBUG
#endif


//#ifdef _DEBUG
//#define Uses_assert
////#pragma message("Debug build active")
//#endif

#ifdef NDEBUG
//#pragma message("Release build active")
#endif

//#define Uses_MemLEAK
//#define TRACE_MEMORY_LEAK

//#define ENABLE_LOCK_TRACING
//#define Uses_LOCK_LINE

#define ENABLE_USE_INTERNAL_C_STATISTIC /*in some c file there is diagnostic var*/
