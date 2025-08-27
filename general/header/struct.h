
#if defined Uses_struct || !defined __COMPILING

#if defined Uses_Thread_watchDog_needs || !defined __COMPILING

typedef struct Thread_watchDog_needs
{
	pthread_t threadId;
	time_t lastAccs;
	int counter;
	callback_t2 cal;
	void_p callback_arg;
} TWD;

#endif

#if defined Uses_lock_pkg || !defined __COMPILING

typedef struct lock_pkg
{
	callback_t1 clb_init;
	callback_t1 clb_destroy;
	callback_t5 clb_lock;
	callback_t5 clb_unlock;
	void_p clb_pdata;
} l_pkg;

#endif

#if defined Uses_T_2_T || !defined __COMPILING

typedef union int_2_string
{
	int i;
	char ar[ INT_SZ + 1 ]; // base on gcc compiler

} _i2s;

#endif

#endif
