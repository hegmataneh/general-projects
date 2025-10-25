#define IN_DDLCK_H

#define Uses_ddlck
#include <general.dep>

#if defined ENABLE_LOCK_TRACING || !defined __COMPILING

GLOBAL_VAR int __tu_global_counter = 0;

_tmp_arr_type_ __lck_hit = {0};

#endif
