#define Uses_STRCMP
#define Uses_bool
#define Uses_WARNING
#define Uses_MEMSET_ZERO_O
#define Uses_backtrace
#define Uses_StackTrace
#include <general.dep>

#if defined Uses_MemLEAK || !defined __COMPILING

void stktrace_init( bcktrc_t * trc , size_t count )
{
	MEMSET_ZERO_O( trc );
	trc->sz = count;
}

// Callback when we successfully get a frame
static int full_callback( void * data , uintptr_t pc ,
	const char * filename , int lineno ,
	const char * function )
{
	bcktrc_t * trc = ( bcktrc_t * )data;
	trc->fxns[ trc->idx ] = function ? function : "??";
	trc->files[ trc->idx ] = filename ? filename : "??";
	trc->lines[ trc->idx ] = lineno;
	
	bool bnew = true;
	for ( long prev = 0 ; prev <= trc->idx - 1 ; prev++ )
	{
		if ( !STRCMP( trc->files[ prev ] , filename ) )
		{
			bnew = false;
		}
	}
	if ( bnew )
	{
		trc->idx++;
	}
	return trc->idx >= trc->sz; // continue
}

void stktrace_fill( bcktrc_t * trc )
{
	struct backtrace_state * state =
		backtrace_create_state( NULL , /* filename = current executable */
			1 ,    /* threaded = yes */
			NULL ,
			NULL );
	trc->idx = 0;
	backtrace_full( state , 2 , full_callback , NULL , trc );
}

void stktrace_generate( bcktrc_t * ptrace ) // return valid stack trace
{
	stktrace_init( ptrace , 3 );
	stktrace_fill( ptrace );
	int n = 0;
	for ( size_t idx = 0 ; idx < ptrace->idx ; idx++ )
	{
		n += sprintf( ptrace->temp_buf + n , "%s:%d\n" , get_filename( ptrace->files[idx] ) , ptrace->lines[idx] );
	}
	if ( n > sizeof( ptrace->temp_buf ) )
	{
		WARNING( n < sizeof(ptrace->temp_buf) );
	}
}

#endif
