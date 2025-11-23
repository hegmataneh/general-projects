#define Uses_instantaneousBps
#include <general.dep>

void inst_rate_init( instBps_t * rm )
{
	rm->current_sec = 0;
	rm->bytes_this_sec = 0;
	rm->last_rate = 0;
}

void inst_rate_add_packet( instBps_t * rm , uint64_t packet_size )
{
	time_t now = time( NULL );  // current second

	if ( rm->current_sec == 0 )
	{
		// first packet
		rm->current_sec = now;
	}

	if ( now != rm->current_sec )
	{
		// a new second started → finalize last second's rate
		rm->last_rate = rm->bytes_this_sec;

		// reset for new second
		rm->bytes_this_sec = 0;
		rm->current_sec = now;
	}

	// add packet to the current second
	rm->bytes_this_sec += packet_size;
}

bool inst_has_item( instBps_t * rm )
{
	if ( !rm ) return false;
	return rm->current_sec != 0;
}

// get rate and check wheater store time not too many old
uint64_t inst_rate_get_last( instBps_t * rm , size_t last_time_timeout )
{
	if ( last_time_timeout )
	{
		time_t now = time( NULL );
		if ( now - rm->current_sec > last_time_timeout )
		{
			rm->current_sec = 0;
			rm->bytes_this_sec = 0;
			rm->last_rate = 0;
		}
	}
	return rm->last_rate;
}
