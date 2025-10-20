// use this is other project dep

#if defined(NDEBUG) && ( defined(DEBUG) || defined(_DEBUG) )
#error "this condition is not allowed"
#endif

#if !defined(NDEBUG) && !defined(_DEBUG)
#define _DEBUG
#endif



//#define Uses_MemLEAK
//#define TRACE_MEMORY_LEAK


//#ifdef _DEBUG
//#define Uses_assert
////#pragma message("Debug build active")
//#endif

#ifdef NDEBUG
//#pragma message("Release build active")
#endif

#include "general.use"
#include "general.use"
#include "general.use"
#include "general.use"
#include "general.use"
#include "general.use"

#include "system.use"
#include "system.use"
#include "system.use"
#include "system.use"
#include "system.use"
#include "system.use"

#include "system.inc"

#include "general.inc"

