#pragma once

#if defined Uses_MemLEAK || !defined __COMPILING /*at scope of compiler each source must use it*/

	//#define MALLOC( size )					// malloc( size ) /*allocate and left uninitialized. so faster*/
	//#define MALLOC_AR( ptr , count )			// ( ( __typeof__( *(ptr) ) * )malloc( ( size_t )( count ) * sizeof( *(ptr) ) ) )
	//#define REALLOC( ptr , size )				// realloc( ptr , size )
	//#define REALLOC_AR( ptr , count )			// ( ( __typeof__( *(ptr) ) * )realloc( ptr , ( size_t )( count ) * sizeof( *(ptr) ) ) )
	//#define REALLOC_AR_SAFE( ptr , count )	// do { __typeof__( *(ptr) ) * __tmp_ = ptr; if ( !( ptr = ( ( __typeof__( *(ptr) ) * )realloc( ptr , ( size_t )( count ) * sizeof( *(ptr) ) ) ) ) ) { ptr = __tmp_; } } while(0) /*safe reallocation*/
	//#define CALLOC( count , elem_size )		// calloc( ( count ) , ( elem_size ) ) /*allocate and memset with zero. so slower*/
	//#define FREE_PTR( ptr )					// free( ( void_p )ptr )
	//#define STRDUP( str )						// strdup( str )

	#if defined TRACE_MEMORY_LEAK || !defined __COMPILING /*manually turn it off*/
		
		/* **** private **** */

		/*macros that should be changed if i want to dump memory leak*/
		#ifdef MALLOC
		#undef MALLOC
		#endif
		#ifdef MALLOC_AR
		#undef MALLOC_AR
		#endif
		#ifdef REALLOC
		#undef REALLOC
		#endif
		#ifdef REALLOC_AR
		#undef REALLOC_AR
		#endif
		
		#ifdef REALLOC_AR_SAFE
		#undef REALLOC_AR_SAFE
		#endif
		
		#ifdef CALLOC
		#undef CALLOC
		#endif
		#ifdef FREE_PTR
		#undef FREE_PTR
		#endif
		#ifdef STRDUP
		#undef STRDUP
		#endif
		#ifdef FREE_DOUBLE_PTR
		#undef FREE_DOUBLE_PTR
		#endif


		/*set based on source count . max source size and backtrack we want*/
		#define MLK_HASH_WIDTH 256
		#define EACH_ADDR_COUNT 30000 /*large than 200000 cause error : (.text+0x1df9): relocation truncated to fit: R_X86_64_PC32 against `.bss'*/

		typedef struct
		{
			const char * caller_fxn;
			const char * act_fxn; // malloc , calloc , free
			ushort line;
			void * address;
			size_t size;
			short counter; // alloc or free counter
			bcktrc_t klstck;
		} mLeak_t;

		#pragma GCC diagnostic ignored "-Wuse-after-free"

		extern void _Breaked();

		#define MALLOC(sz) ({ ushort _lline = (ushort)__LINE__; \
							static ulong __ulcounter=0; void *___p = malloc(sz);\
							mLeak_t *pml = &__alc_hit[hash8_fnv1a_avalanche_l((long)___p)][__ulcounter++]; \
							__ulcounter = __ulcounter % EACH_ADDR_COUNT;\
							pml->caller_fxn = get_filename(__FILE__);\
							pml->act_fxn = "malloc";\
							pml->line = _lline;\
							pml->address = ___p;\
							pml->size = sz;\
							pml->counter++;\
							stktrace_generate(&pml->klstck);\
							___p; })

		#define MALLOC_AR(ptr , count) ({ ushort _lline = (ushort)__LINE__; \
							static ulong __ulcounter=0; void *___p = malloc(( size_t )( count ) * sizeof( *(ptr) ));\
							mLeak_t *pml = &__alc_hit[hash8_fnv1a_avalanche_l((long)___p)][__ulcounter++]; \
							__ulcounter = __ulcounter % EACH_ADDR_COUNT;\
							pml->caller_fxn = get_filename(__FILE__);\
							pml->act_fxn = "malloc";\
							pml->line = _lline;\
							pml->address = ___p;\
							pml->size = ( size_t )( count ) * sizeof( *(ptr) );\
							pml->counter++;\
							stktrace_generate(&pml->klstck);\
							( __typeof__( *(ptr) ) * )___p; })
							
		#define REALLOC(ptr , sizee) ({ ushort _lline = (ushort)__LINE__; \
							void * ptr_before = (void*)(ptr);\
							static ulong __ulcounter=0;\
							void *___p = realloc(ptr , sizee);\
							if ( ptr_before && ___p && ptr_before != ___p)\
							{\
								mLeak_t *pml1 = NULL;\
								for ( ulong ic = 0 ; ic < EACH_ADDR_COUNT ; ic++ ){\
									if ( ptr_before && __alc_hit[ hash8_fnv1a_avalanche_l( ( long )ptr_before ) ][ ic ].address == ptr_before ){\
										pml1 = &__alc_hit[ hash8_fnv1a_avalanche_l( ( long )ptr_before ) ][ ic ]; break;}}\
								if ( pml1 ) { pml1->counter--; } if ( pml1->counter < -1 ) {_Breaked();}  \
								mLeak_t *pml2 = &__alc_hit[hash8_fnv1a_avalanche_l((long)___p)][__ulcounter++]; \
								__ulcounter = __ulcounter % EACH_ADDR_COUNT;\
								pml2->caller_fxn = get_filename(__FILE__);\
								pml2->act_fxn = "realloc";\
								pml2->line = _lline;\
								pml2->address = ___p;\
								pml2->size = sizee;\
								pml2->counter++;\
								stktrace_generate(&pml2->klstck);\
							}\
							if ( !ptr_before )\
							{\
								mLeak_t *pml2 = &__alc_hit[hash8_fnv1a_avalanche_l((long)___p)][__ulcounter++]; \
								__ulcounter = __ulcounter % EACH_ADDR_COUNT;\
								pml2->caller_fxn = get_filename(__FILE__);\
								pml2->act_fxn = "realloc";\
								pml2->line = _lline;\
								pml2->address = ___p;\
								pml2->size = sizee;\
								pml2->counter++;\
								stktrace_generate(&pml2->klstck);\
							}\
							( __typeof__( *(ptr) ) * )___p; })

		#define REALLOC_AR(ptr , count) ({ ushort _lline = (ushort)__LINE__; \
							void * ptr_before = (ptr);\
							static ulong __ulcounter=0; void *___p = realloc(ptr , ( size_t )( count ) * sizeof( *(ptr) ));\
							if ( ptr_before && ___p && ptr_before != ___p)\
							{\
								mLeak_t *pml1 = NULL;\
								for ( ulong ic = 0 ; ic < EACH_ADDR_COUNT ; ic++ ){\
									if ( ptr_before && __alc_hit[ hash8_fnv1a_avalanche_l( ( long )ptr_before ) ][ ic ].address == ptr_before ){ pml1 = &__alc_hit[ hash8_fnv1a_avalanche_l( ( long )ptr_before ) ][ ic ]; break;}}\
								if ( pml1 ) {pml1->counter--;} if ( pml1->counter < -1 ) {_Breaked();} \
								mLeak_t *pml2 = &__alc_hit[hash8_fnv1a_avalanche_l((long)___p)][__ulcounter++]; \
								__ulcounter = __ulcounter % EACH_ADDR_COUNT;\
								pml2->caller_fxn = get_filename(__FILE__);\
								pml2->act_fxn = "realloc";\
								pml2->line = _lline;\
								pml2->address = ___p;\
								pml2->size = ( size_t )( count ) * sizeof( *(ptr) );\
								pml2->counter++;\
								stktrace_generate(&pml2->klstck);\
							}\
							if ( !ptr_before )\
							{\
								mLeak_t *pml2 = &__alc_hit[hash8_fnv1a_avalanche_l((long)___p)][__ulcounter++]; \
								__ulcounter = __ulcounter % EACH_ADDR_COUNT;\
								pml2->caller_fxn = get_filename(__FILE__);\
								pml2->act_fxn = "realloc";\
								pml2->line = _lline;\
								pml2->address = ___p;\
								pml2->size = ( size_t )( count ) * sizeof( *(ptr) );\
								pml2->counter++;\
								stktrace_generate(&pml2->klstck);\
							}\
							( __typeof__( *(ptr) ) * )___p; })

							
		#define REALLOC_AR_SAFE(ptr , count) ({ ushort _lline = (ushort)__LINE__; \
							void * ptr_before = (ptr);\
							static ulong __ulcounter=0; void *___p = realloc(ptr , ( size_t )( count ) * sizeof( *(ptr) ));\
							if ( ptr_before && ___p && ptr_before != ___p)\
							{\
								mLeak_t *pml1 = NULL;\
								for ( ulong ic = 0 ; ic < EACH_ADDR_COUNT ; ic++ ){\
									if ( ptr_before && __alc_hit[ hash8_fnv1a_avalanche_l( ( long )ptr_before ) ][ ic ].address == ptr_before ){\
										pml1 = &__alc_hit[ hash8_fnv1a_avalanche_l( ( long )ptr_before ) ][ ic ]; break;}}\
								if ( pml1 ) { pml1->counter--;} if ( pml1->counter < -1 ) {_Breaked();\
								}\
								mLeak_t * pml2 = &__alc_hit[ hash8_fnv1a_avalanche_l( ( long )___p ) ][ __ulcounter++ ]; \
								__ulcounter = __ulcounter % EACH_ADDR_COUNT; \
								pml2->caller_fxn = get_filename( __FILE__ ); \
								pml2->act_fxn = "realloc"; \
								pml2->line = _lline; \
								pml2->address = ___p; \
								pml2->size = ( size_t )( count ) * sizeof( *(ptr) );\
								pml2->counter++;\
								stktrace_generate( &pml2->klstck ); \
							}\
							if ( !ptr_before && ___p )\
							{\
								mLeak_t * pml2 = &__alc_hit[ hash8_fnv1a_avalanche_l( ( long )___p ) ][ __ulcounter++ ]; \
								__ulcounter = __ulcounter % EACH_ADDR_COUNT; \
								pml2->caller_fxn = get_filename( __FILE__ ); \
								pml2->act_fxn = "realloc"; \
								pml2->line = _lline; \
								pml2->address = ___p; \
								pml2->size = ( size_t )( count ) * sizeof( *(ptr) );\
								pml2->counter++;\
								stktrace_generate( &pml2->klstck ); \
							}\
							if ( !___p )\
							{\
								___p = ptr_before;\
							}\
							( __typeof__( *(ptr) ) * )___p; })


		#define CALLOC(count , elem_size) ({ ushort _lline = (ushort)__LINE__; \
							static ulong __ulcounter=0; void *___p = calloc(( count ) , ( elem_size ));\
							mLeak_t *pml = &__alc_hit[hash8_fnv1a_avalanche_l((long)___p)][__ulcounter++]; \
							__ulcounter = __ulcounter % EACH_ADDR_COUNT;\
							pml->caller_fxn = get_filename(__FILE__);\
							pml->act_fxn = "calloc";\
							pml->line = _lline;\
							pml->address = ___p;\
							pml->size = ( size_t )(( count ) * (elem_size));\
							pml->counter++;\
							stktrace_generate(&pml->klstck);\
							___p; })


		#define STRDUP(str) ({ ushort _lline = (ushort)__LINE__; \
							static ulong __ulcounter=0; void *___p = strdup(str);\
							mLeak_t *pml2 = &__alc_hit[hash8_fnv1a_avalanche_l((long)___p)][__ulcounter++]; \
							__ulcounter = __ulcounter % EACH_ADDR_COUNT;\
							pml2->caller_fxn = get_filename(__FILE__);\
							pml2->act_fxn = "strdup";\
							pml2->line = _lline;\
							pml2->address = ___p;\
							pml2->size = strlen(str);\
							pml2->counter++;\
							stktrace_generate(&pml2->klstck);\
							( char * )___p; })


		#define FREE_PTR(ptr) ({ \
							void *___p = (void *)(ptr); free(( void_p )ptr);\
							mLeak_t *pml1 = NULL;\
							for ( ulong ic = 0 ; ic < EACH_ADDR_COUNT && ___p; ic++ ){\
								if ( __alc_hit[ hash8_fnv1a_avalanche_l( ( long )___p ) ][ ic ].address == ___p ){\
									pml1 = &__alc_hit[ hash8_fnv1a_avalanche_l( ( long )___p ) ][ ic ]; break;}}\
							if ( pml1 ) {pml1->counter--;} if ( pml1->counter < -1 ) {_Breaked();} \
							})

		#define FREE_DOUBLE_PTR(pptr , count) ({ \
											for ( int ___i = 0 ; ___i < ( count ) ; ___i++ )\
											{\
											void *___p = (void *)pptr[ ___i ]; free(( void_p )pptr[ ___i ]);\
											mLeak_t *pml1 = NULL;\
											for ( ulong ic = 0 ; ic < EACH_ADDR_COUNT && ___p; ic++ )\
											{\
												if ( __alc_hit[ hash8_fnv1a_avalanche_l( ( long )___p ) ][ ic ].address == ___p )\
												{\
													pml1 = &__alc_hit[ hash8_fnv1a_avalanche_l( ( long )___p ) ][ ic ]; break;\
												}\
											}\
											if ( pml1 ) {pml1->counter--;} if ( pml1->counter < -1 ) {_Breaked();} \
											}\
											void *___p = (void *)pptr; free(( void_p )pptr);\
											mLeak_t *pml1 = NULL;\
											for ( ulong ic = 0 ; ic < EACH_ADDR_COUNT && ___p; ic++ ){\
												if ( __alc_hit[ hash8_fnv1a_avalanche_l( ( long )___p ) ][ ic ].address == ___p ){\
													pml1 = &__alc_hit[ hash8_fnv1a_avalanche_l( ( long )___p ) ][ ic ]; break;}}\
											if ( pml1 ) {pml1->counter--;} if ( pml1->counter < -1 ) {_Breaked();}\
											})

		/* **** private **** */



		/* **** public **** */
		_EXTERN mLeak_t __alc_hit[ MLK_HASH_WIDTH ][ EACH_ADDR_COUNT ]; /*just extern this array in your code and count leaks*/
		/* **** public **** */

	#endif  // TRACE_MEMORY_LEAK

#endif // Uses_MemLEAK

