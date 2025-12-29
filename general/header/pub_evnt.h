#pragma once

#if defined Uses_publish_event || !defined __COMPILING

/*
- this struct make to event distribtion could be happened
- not very fine event distributor and is not usable in very high performance manner
- just good for relatively long periode of time event generator not nano second one
- just one event per periode time and concurrent event ignored
*/

typedef struct pub_event_t
{
	sem_t gateway;
	void_p subscriber_data;

} pub_evt_t;

typedef struct pub_event_coordinator
{
	dyn_mms_arr subs;
	volatile bool * app_closed_signal;

} pub_evt_cord_t;

status init__pub_evt( pub_evt_cord_t * evt_hdlr , volatile bool * app_closed_signal ); /*used for initialized main coordinator*/
void cleanup__pub_evt( pub_evt_cord_t * evt_hdlr );
status subscribe__pub_evt( pub_evt_cord_t * evt_hdlr , pub_evt_t ** pevent ); /*used for subscribe event handler into coordinator*/
status publish__pub_evt( pub_evt_cord_t * evt_hdlr ); /*coordinator distribute event*/
status any_event__pub_evt( pub_evt_cord_t * evt_hdlr , pub_evt_t * pevent , long timeout_sec , bool drain_gateway ); /*check to handling event*/

#endif

