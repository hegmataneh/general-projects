#pragma once

#if defined Uses_instantaneousBps || !defined __COMPILING

/*used for detecting instantaneous load*/

typedef struct instantaneousBps
{
	time_t current_sec;
	uint64_t bytes_this_sec;
	uint64_t last_rate;   // bytes per second from previous second
} instBps_t;

void inst_rate_init( instBps_t * rm );
void inst_rate_add_packet( instBps_t * rm , uint64_t packet_size );
bool inst_has_item( instBps_t * rm );
uint64_t inst_rate_get_last( instBps_t * rm , size_t last_time_timeout );

#endif
