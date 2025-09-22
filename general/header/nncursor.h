#pragma once

#if defined Uses_nncursor || !defined __COMPILING

//#define MAX_TABS 8
//#define MAX_COLS 16
//#define MAX_ROWS 128

// TODO . 14040629 . extend and keep backward compatibility if you want more dynamic in this code

#define TAB_BAR_HEIGHT 3
#define CMD_H 3

#define MIN_COL_SIZE 3

typedef struct
{
	DATAB tmp[64];

	union
	{
		int i;
		//double d;
		//long l;
		PASSED_CSTR pass_str; // passed and shouldnt freed
		//INNER_STR in_str; // must freed

	} val;

	//union
	//{
	//	PASSED_STR outer_conversion_low_buffer;
	//	PASSED_STR outer_conversion_mid_buffer;
	//	PASSED_STR outer_conversion_high_buffer;
	//	PASSED_STR outer_buf;

	//} buf;

	callback_t6 conversion_fxn; // convert any type value into sring
	callback_t1 clean_fxn; /*if not NULL called*/
	callback_t1 propagate_changes;

	bool refresh_cell;

	typedef struct
	{
		int y , x;
		size_t width;
	} paint_s;
	
	void_p prow;
	void_p pcell_container;

} nnc_cell_content;

typedef struct
{
	nnc_cell_content * pcell;

} nnc_cell_container;

/*some default conversion fxn*/
//PASSED_CSTR d_as_MB( pass_p pcell );
PASSED_CSTR i_as_str_pass( pass_p pcell );
PASSED_CSTR data_segment_str_pass( pass_p pcell );

typedef struct
{
	INNER_STR hdr;
	INNER_STR subhdr;
	size_t minw;

	void_p ptbl;
} nnc_column;

typedef struct
{
	dyn_arr cell_containers; // nnc_cell_container

	bool refresh_partial_row;

	void_p ptbl;
} nnc_row;

typedef struct
{
	INNER_STR tabname;

	dyn_arr cols; // nnc_column
	dyn_arr rows; // nnc_row

	bool refresh_partial_table;
	bool refresh_table;

} nnc_table;

typedef struct
{
	size_t x0 , x1; /*boundries of tab title*/

} nnc_tabHit;


typedef struct nncursor_requirement
{
	dyn_arr tables;

	size_t active;
	dyn_arr tabHit_arr;
	bool refresh_tabs;

	struct notcurses_options opts;
	struct notcurses * nc;
	struct ncplane * std;

	struct ncplane * tabs_plane;
	struct ncplane * body_plane;
	struct ncplane * cmd_plane;
	int termw , termh;
} nnc_req;


status nnc_begin_init_mode( nnc_req * nnc );
void nnc_begin_render_mode( nnc_req * nnc );
Boolean couninue_loop_callback( nnc_req * nnc );
void nnc_destroy( nnc_req * nnc );

status nnc_add_table( nnc_req * nnc , PASSED_CSTR tabname , nnc_table ** ptable );
status nnc_add_column( nnc_table * tbl , PASSED_CSTR src_hdr , PASSED_CSTR src_subhdr , size_t src_minw );
status nnc_add_empty_row( nnc_table * tbl , nnc_row ** prow );

status nnc_set_static_text( nnc_table * tbl , size_t row , size_t col , PASSED_CSTR static_text );
status nnc_set_outer_cell( nnc_table * tbl , size_t row , size_t col , nnc_cell_content * pouter_cell );

void nnc_set_int_cell( nnc_cell_content * pcell , int src_i ); // use this function instead of assigning directly

//status nnc_get_emptied_cell( nnc_table * tbl , size_t row , size_t col , nnc_cell_content * * pcell );

#endif

