#define IN_DDLCK_H

#define Uses_ddlck
#include <general.dep>

#if defined ENABLE_LOCK_TRACING || !defined __COMPILING

#ifdef ENABLE_USE_DBG_TAG

_GLOBAL_VAR int __ddl_global_counter = 0;

_GLOBAL_VAR ulong __ddl_call_seq_counter = 0;

_GLOBAL_VAR _tmp_arr_type_ __lck_hit = {0};

_GLOBAL_VAR ulong __lck_lines_hit[SEQ_TRACE_COUNTE] = {0};

#endif

#endif
