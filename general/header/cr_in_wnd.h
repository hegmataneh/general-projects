#pragma once

#if defined Uses_circular_sliding_input_window || !defined __COMPILING

// not thread safe

typedef struct
{
	size_t Byte;
	time_t sec;
} cr_wnd_slide;

typedef struct circular_sliding_input_window
{
	cr_wnd_slide * Bytes;
	size_t	window_size;

	size_t filled_count;
	time_t  last_time;
	union
	{
		int	last_idx;
		long pad1;
	};
	size_t  total_pkt_sz_B;

	pthread_mutex_t lock;
	
} cr_in_wnd_t;

status cr_in_wnd_init( cr_in_wnd_t * rm , size_t src_windows_size );

void cr_in_wnd_free( cr_in_wnd_t * buf );

//void cr_in_wnd_clear_all( cr_in_wnd_t * buf );

void cr_in_wnd_add_packet( cr_in_wnd_t *rm, size_t packet_size_B );

//bool cr_in_wnd_has_item( cr_in_wnd_t *rm );

double cr_in_wnd_get_Bps( cr_in_wnd_t *rw );

//int cr_in_wnd_regression( cr_in_wnd_t * rm );

size_t cr_in_wnd_get_ordered_items( cr_in_wnd_t *rw , cr_wnd_slide * buffer_bytes );

double cr_in_wnd_calc_pearson_correlation( cr_in_wnd_t *rw1 , cr_in_wnd_t *rw2 ); // also spearman is good also some where i read it is better because correlation showed up better

#endif
