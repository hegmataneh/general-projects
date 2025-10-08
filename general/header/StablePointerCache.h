#pragma once

#if defined Uses_StablePointerCache || !defined __COMPILING

/*
resolve name to fixed address of block
*/

typedef struct
{
	char * key;
	void * block;

} spc_block;

typedef struct
{
	dyn_mms_arr arr;
	pthread_mutex_t lock;

} spc_mgr;

#endif
