//#pragma once

//-------------------
#define MALLOC( size ) malloc( size )
#define FREE( p ) free( ( void * )p )
#define REALLOC( ptr , size ) realloc( ptr , size )
#define CALLOC calloc

//-----------------------

#if defined __GNUC__
	#define DEL(p) FREE( p )
	#define DEL_AR(p) FREE( p )
#else
	#define DEL(p) delete p
	#define DEL_AR(p) delete[] p
#endif
