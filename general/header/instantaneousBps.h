#pragma once

#if defined Uses_instantaneousBps || !defined __COMPILING

/*used for detecting instantaneous load*/

typedef struct instantaneousBps
{
	time_t current_sec;
	size_t bytes_this_sec;
	size_t last_rate;   // bytes per second from previous second
	pthread_mutex_t lock;
} instBps_t;

void inst_rate_init( instBps_t * rm );
void inst_rate_destroy( instBps_t * rm );
void inst_rate_add_packet( instBps_t * rm , size_t packet_size );
//bool inst_has_item( instBps_t * rm );
size_t inst_rate_get_last( instBps_t * rm , size_t last_time_timeout );

#endif
