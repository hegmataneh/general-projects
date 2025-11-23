#pragma once

#if defined Uses_circular_sliding_rate_window || !defined __COMPILING

// not thread safe

typedef struct circular_sliding_rate_window
{
	uint64_t * bytes;
	time_t * sec;
	size_t	window_size;

	time_t  last_time;
	int	last_idx;
	uint64_t  total;

} cir_rate_wnd_t;

status cwnd_rate_init( cir_rate_wnd_t * rm , size_t src_windows_size );

void cwnd_rate_free( cir_rate_wnd_t * buf );

void cwnd_clear_all( cir_rate_wnd_t * buf );

void cwnd_rate_add_packet( cir_rate_wnd_t *rm, uint64_t packet_size );

bool cwnd_rate_has_item( cir_rate_wnd_t *rm );

double cwnd_rate_get_bps( cir_rate_wnd_t *rw );

#endif
