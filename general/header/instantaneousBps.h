#pragma once

#if defined Uses_instantaneousBps || !defined __COMPILING

/*
used for detecting instantaneous load
not concurrent usable
*/


typedef struct instantaneousBps
{
	struct
	{
		//time_t current_sec;
		timespec current_exact_tm;
		size_t bytes;
		//size_t last_rate;   // bytes per second from previous second
	} data;
	//pthread_mutex_t lock;

} instBps_t;

void inst_rate_init( instBps_t * rm );
//void inst_rate_destroy( instBps_t * rm );
void inst_rate_init2start( instBps_t * rm );

bool inst_rate_add_packet( instBps_t * rm , size_t packet_size );
//size_t inst_rate_get_last( instBps_t * rm , size_t last_time_timeout );
status inst_rate_loadBps( instBps_t * rm , size_t * Bps , double * elapsed_sec );

#endif
