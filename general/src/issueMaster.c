#define Uses_BRIF_M_BREAK_IF
#define Uses_MEMSET_ZERO_O
#define Uses_INIT_BREAKABLE_FXN
#define Uses_issuesMaster_handle
#include <general.dep>

status issue_init( issues_h * hissues , size_t default_timeout_sec , size_t max_issues_cnt , Brief_Err * imortalErrStr )
{
	INIT_BREAKABLE_FXN();

	MEMSET_ZERO_O( hissues );
	BREAK_STAT( mms_array_init( &hissues->issues_ar , sizeof( issue_h ) , max_issues_cnt , /*GROW_STEP*/0/*intentionally . has max count*/ , 0 ) , 0);
	BRIF_M_BREAK_IF( pthread_mutex_init( &hissues->pr_lock , NULL ) , errFault , 0 , "mutex_init()" , imortalErrStr );
	hissues->default_timeout_sec = ( uint16 )default_timeout_sec;

	BEGIN_SMPL
	END_RET
}

void issue_cleanup( issues_h * hissues )
{
	pthread_mutex_destroy( &hissues->pr_lock );
	mms_array_free( &hissues->issues_ar );
}

_PRIVATE_FXN void pr_issue_retire( issues_h * hissues )
{
	size_t cnt = mms_array_get_count( &hissues->issues_ar );
	issue_h * pissue = NULL;
	time_t tnow = time( NULL );
	for ( size_t idx = 0 ; idx < cnt ; idx++ )
	{
		if ( !mms_array_get_s( &hissues->issues_ar , idx , ( void** )&pissue ) && !pissue->retired )
		{
			if ( tnow >= pissue->arrive_time + pissue->timeout_sec )
			{
				pissue->retired = true;
			}
		}
	}
}

_PRIVATE_FXN void pr_issue_garbage_collector( issues_h * hissues )
{
	size_t cnt = mms_array_get_count( &hissues->issues_ar );
	issue_h * pissue = NULL;
	if ( cnt > 0 )
	{
		for ( size_t idx = cnt - 1 ; idx >= 0 ; idx-- )
		{
			if ( !mms_array_get_s( &hissues->issues_ar , idx , ( void ** )&pissue ) )
			{
				if ( pissue->retired )
				{
					mms_array_delete( &hissues->issues_ar , idx );
				}
			}
			if ( !idx ) break;
		}
	}
}

_PRIVATE_FXN int compare_issues( const void * a , const void * b ) /*TOCHECK*/
{
	issue_h * arg1 = *( issue_h ** )a;
	issue_h * arg2 = *( issue_h ** )b;
	if ( arg1->retired && !arg2->retired ) return -1;
	if ( !arg1->retired && arg2->retired ) return 1;
	if ( arg1->arrive_time < arg2->arrive_time ) return -1; // less value means low priority
	if ( arg1->arrive_time > arg2->arrive_time ) return 1; // more value means higher order
	return 0;
}

void issue_add( issues_h * hissues , LPCSTR issue_msg )
{
	if ( !hissues ) return;
	if ( !hissues->default_timeout_sec ) return;

	INIT_BREAKABLE_FXN();

	NO_LOCK_LINE( pthread_mutex_lock( &hissues->pr_lock ) );

	pr_issue_retire( hissues );

	size_t cnt = mms_array_get_count( &hissues->issues_ar );
	issue_h * pissue = NULL;
	for ( size_t idx = 0 ; idx < cnt ; idx++ )
	{
		if ( !mms_array_get_s( &hissues->issues_ar , idx , ( void ** )&pissue ) && pissue->retired )
		{
			DAC( pissue->issue_msg );
			if ( ( pissue->issue_msg = newStr( issue_msg ) ) )
			{
				pissue->arrive_time = time( NULL );
				pissue->timeout_sec = hissues->default_timeout_sec;
				pissue->retired = false;
				pthread_mutex_unlock( &hissues->pr_lock );
				BREAK( errOK , 0 );
			}
		}
	}

	if ( !( d_error = mms_array_get_one_available_unoccopied_item( &hissues->issues_ar , ( void ** )&pissue ) ) )
	{
		if ( ( pissue->issue_msg = newStr( issue_msg ) ) )
		{
			pissue->arrive_time = time( NULL );
			pissue->timeout_sec = hissues->default_timeout_sec;
		}
		else
		{
			pissue->retired = true;
		}
	}
	else
	{
		switch( d_error )
		{
			case errResource:
			{
				// Return a negative value: This means the first item should come before the second item.
				if ( hissues->issues_ar.count ) qsort( hissues->issues_ar.data , hissues->issues_ar.count , sizeof( void * ) , compare_issues );

				if ( !mms_array_get_s( &hissues->issues_ar , 0 , ( void ** )&pissue ) )
				{
					DAC( pissue->issue_msg );
					if ( ( pissue->issue_msg = newStr( issue_msg ) ) )
					{
						pissue->arrive_time = time( NULL );
						pissue->timeout_sec = hissues->default_timeout_sec;
						pissue->retired = false;
						pthread_mutex_unlock( &hissues->pr_lock );
						BREAK( errOK , 0 );
					}
				}


				// use first one
				break;
			}
		}
	}

	pthread_mutex_unlock( &hissues->pr_lock );

	BEGIN_SMPL
	N_V_END_RET
}

void issue_retire( issues_h * hissues )
{
	if ( !hissues ) return;
	if ( !hissues->default_timeout_sec ) return;

	NO_LOCK_LINE( pthread_mutex_lock( &hissues->pr_lock ) );

	pr_issue_retire( hissues );

	pthread_mutex_unlock( &hissues->pr_lock );
}

void issue_garbage_collector( issues_h * hissues )
{
	if ( !hissues ) return;
	if ( !hissues->default_timeout_sec ) return;

	NO_LOCK_LINE( pthread_mutex_lock( &hissues->pr_lock ) );

	pr_issue_garbage_collector( hissues );

	pthread_mutex_unlock( &hissues->pr_lock );
}
