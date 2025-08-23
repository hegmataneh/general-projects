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
	for ( int i = 0 ; i < thread_count ; i++ )
	{
		pLB->choosing[ i ] = 0;
		pLB->number[ i ] = 0;
	}
	return errOK;
}

void ba_destroy( Ba_al * pLB )
{
	DAC( pLB->choosing );
	DAC( pLB->number );
}

void ba_lock( Ba_al * pLB , int i )
{
	pLB->choosing[ i ] = 1;
	// Find the maximum number currently held by any process
	int max_num = 0;
	for ( int k = 0; k < pLB->thread_count; k++ )
	{
		if ( pLB->number[ k ] > max_num )
		{
			max_num = pLB->number[ k ];
		}
	}
	pLB->number[ i ] = max_num + 1;
	pLB->choosing[ i ] = 0;

	for ( int j = 0; j < pLB->thread_count; j++ )
	{
		if ( j == i ) continue; // Skip self

		// Wait while process j is choosing a number
		while ( pLB->choosing[ j ] )
		{
			// Busy-wait or yield
		}

		// Wait while process j has a smaller or equal number with higher priority (lower ID)
		while
		(
			( pLB->number[ j ] != 0 ) &&
			(
				( pLB->number[ j ] < pLB->number[ i ] ) ||
				( ( pLB->number[ j ] == pLB->number[ i ] ) && ( j < i ) )
			)
		)
		{
			// Busy-wait or yield
		}
	}
}

void ba_unlock( Ba_al * pLB , int i )
{
	pLB->number[ i ] = 0; // Indicate that process i is no longer in the critical section
}
