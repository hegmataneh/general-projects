#define Uses_sem_wait_with_timeout
#define Uses_MEMSET_ZERO_O
#define Uses_publish_event
#include <general.dep>


status init__pub_evt( pub_evt_cord_t * evt_hdlr , volatile bool * app_closed_signal ) /*used for initialized main coordinator*/
{
	MEMSET_ZERO_O( evt_hdlr );
	evt_hdlr->app_closed_signal = app_closed_signal;
	return mms_array_init( &evt_hdlr->subs , sizeof( pub_evt_t ) , 1 , GROW_STEP , 0 );
}

void cleanup__pub_evt( pub_evt_cord_t * evt_hdlr )
{
	size_t cnt = mms_array_get_count( &evt_hdlr->subs );
	pub_evt_t * pevent;
	status d_error;
	for ( size_t itm_idx = 0 ; itm_idx < cnt ; itm_idx++ )
	{
		if ( !( d_error = mms_array_get_s( &evt_hdlr->subs , itm_idx , ( void ** )&pevent ) ) )
		{
			sem_destroy( &pevent->gateway );	
		}
	}
	mms_array_free( &evt_hdlr->subs );
}

status subscribe__pub_evt( pub_evt_cord_t * evt_hdlr , pub_evt_t ** pevent ) /*used for subscribe event handler into coordinator*/
{
	status d_error = mms_array_get_one_available_unoccopied_item( &evt_hdlr->subs , (void**)pevent );
	if ( d_error == errOK )
	{
		if ( sem_init( &((*pevent)->gateway) , 0 , 0 ) < 0 )
		{
			return errCreation;
		}
	}
	return d_error;
}

status publish__pub_evt( pub_evt_cord_t * evt_hdlr ) /*coordinator distribute event*/
{
	status d_error = errOK;
	size_t cnt = mms_array_get_count( &evt_hdlr->subs );
	pub_evt_t * pevent;
	for ( size_t itm_idx = 0 ; itm_idx < cnt ; itm_idx++ )
	{
		if ( ( d_error = mms_array_get_s( &evt_hdlr->subs , itm_idx , ( void ** )&pevent ) ) )
		{
			return d_error;
		}
		sem_post( &pevent->gateway ); // some udp packet complete
	}
	return d_error;
}

status any_event__pub_evt( pub_evt_cord_t * evt_hdlr , pub_evt_t * pevent , long timeout_sec , bool drain_gateway ) /*check to handling event*/
{
	status d_error = sem_wait_with_timeout( &pevent->gateway , timeout_sec , evt_hdlr->app_closed_signal );
	switch ( d_error )
	{
		case errOK:
		{
			if ( drain_gateway )
			{
				while( !sem_trywait( &pevent->gateway ) ); /*not best idea but for now ignore mutiple event at the same time*/
			}
			return d_error;
		}
		case errTimeout: return d_error;
		default:
		{
			// TODO . log gere
			break;
		}
	}
	return d_error;
}
