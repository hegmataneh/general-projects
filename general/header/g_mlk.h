//-------------------
#define MALLOC( size ) malloc( size )
#define FREE( p ) free( ( void * )p )
#define REALLOC( ptr , size ) realloc( ptr , size )
#define CALLOC calloc

//-------------------
#define MME_BREAK_IF_CALL_APPEND
#define MME_BREAK_IF_CALL_CROP
#define MME_BREAK_STAT_CALL_APPEND
#define MME_BREAK_STAT_CALL_CROP
//-----------------------

#if defined __GNUC__
	#define DEL(p) FREE( p )
	#define DEL_AR(p) FREE( p )
#else
	#define DEL(p) delete p
	#define DEL_AR(p) delete[] p
#endif
