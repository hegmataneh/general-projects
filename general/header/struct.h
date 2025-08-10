
#if defined Uses_struct || !defined __COMPILING

typedef struct Thread_watchDog_needs
{
	int threadId;
	time_t lastAccs;
	int counter;
	callback_t cal;
} TWD;



#endif
