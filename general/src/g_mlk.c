#define IN_MLEAK_H

#if defined TRACE_MEMORY_LEAK || !defined __COMPILING
#define Uses_MemLEAK
#endif

#include <general.dep>

#if defined TRACE_MEMORY_LEAK || !defined __COMPILING

mLeak_t __alc_hit[MLK_HASH_WIDTH][EACH_ADDR_COUNT] = {0};

#endif
