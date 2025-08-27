#define Uses_MEMSET_ZERO
#define Uses_BakeryAl
#include <general.dep>

status ba_init( Ba_al * pLB , int thread_count )
{
	pLB->thread_count = thread_count;
	pLB->choosing = MALLOC_AR( pLB->choosing , thread_count );
	if ( !pLB->choosing ) return errGeneral;
	pLB->number = MALLOC_AR( pLB->number , thread_count );
	if ( !pLB->number ) return errGeneral;
	for ( int itrd = 0 ; itrd < thread_count ; itrd++ )
	{
		pLB->choosing[ itrd ] = 0;
		pLB->number[ itrd ] = 0;
	}
	return errOK;
}

void ba_destroy( Ba_al * pLB )
{
	DAC( pLB->choosing );
	DAC( pLB->number );
}

void ba_lock( Ba_al * pLB , int idx )
{
	pLB->choosing[ idx ] = 1;
	// Find the maximum number currently held by any process
	int max_num = 0;
	for ( int itrd = 0; itrd < pLB->thread_count; itrd++ )
	{
		if ( pLB->number[ itrd ] > max_num )
		{
			max_num = pLB->number[ itrd ];
		}
	}
	pLB->number[ idx ] = max_num + 1;
	pLB->choosing[ idx ] = 0;

	for ( int itrd = 0; itrd < pLB->thread_count; itrd++ )
	{
		if ( itrd == idx ) continue; // Skip self

		// Wait while process j is choosing a number
		while ( pLB->choosing[ itrd ] )
		{
			// Busy-wait or yield
		}

		// Wait while process j has a smaller or equal number with higher priority (lower ID)
		while
		(
			( pLB->number[ itrd ] != 0 ) &&
			(
				( pLB->number[ itrd ] < pLB->number[ idx ] ) ||
				( ( pLB->number[ itrd ] == pLB->number[ idx ] ) && ( itrd < idx ) )
			)
		)
		{
			// Busy-wait or yield
		}
	}
}

void ba_unlock( Ba_al * pLB , int idx )
{
	pLB->number[ idx ] = 0; // Indicate that process i is no longer in the critical section
}
