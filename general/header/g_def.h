/* put global definitions in this file instead of preprocessor definitions section of
each project. since these definitions may affect on system files inclusion as well as
decisions in general2.use, i decided it to be included via general.use.
*/

/* hamidi, 860609: uncomment the following line to log memory allocations and find
probable memory leaks.
*/
//#define LOG_MEMORY_ALLOCATIONS

// mohsen , 870312: multithreading ba global variable hayeh ma moshkel dar mi shavad.
#ifndef UNDER_CE
	//#define LOG_PENDING_ERROR
#endif

