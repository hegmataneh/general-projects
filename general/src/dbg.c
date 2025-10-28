#define IN_DBG_H

#define Uses_DBG
#include <general.dep>

#if defined __USE_DBG_TOOLS || !defined __COMPILING

#ifdef ENABLE_USE_INTERNAL_C_STATISTIC

GLOBAL_VAR int __tu_global_counter = 0;

GLOBAL_VAR LPCSTR __map_c2idx[MAX_TU] = {0};

GLOBAL_VAR int/*program count*/ __FXN_HIT[MAX_TU/*max TU count or c files*/][MAX_TU_LINES/*max lines count*/][BACKTRACK_COUNT/*n times of calling the line*/] = {0}; // max size is about number of code lines

GLOBAL_VAR int __pc = 1; // step of each call globally . prog counter

#endif

#endif
