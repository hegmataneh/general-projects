
#if defined Uses_struct || !defined __COMPILING

typedef struct Thread_watchDog_needs
{
	pthread_t threadId;
	time_t lastAccs;
	int counter;
	callback_t cal;
	void * callback_arg;
} TWD;



#endif
