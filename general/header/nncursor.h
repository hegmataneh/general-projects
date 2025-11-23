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
	struct
	{
		DATAB tmpbuf[DEFAULT_MFS_BUF_SZ];

		union /*please be aware of union and struct ability*/
		{
			int i;
			//double d;
			//long l;
			PASSED_CSTR pass_str; // passed and shouldnt freed
			//INNER_STR in_str; // must freed
			struct
			{
				int j;
				pass_p pass_data;
			};

		} bt; // basic type
	} storage;

	callback_t6 conversion_fxn; // convert any type value into sring
	callback_t1 clean_fxn; /*if not NULL called*/
	callback_t1 propagate_changes; /*arg0: void_p src_cell_container*/

	union
	{
		bool refresh_cell;
		long pad1;
	};

	struct
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
//_CALLBACK_FXN PASSED_CSTR d_as_MB( pass_p pcell );
_CALLBACK_FXN PASSED_CSTR i_as_str_pass( pass_p pcell );
_CALLBACK_FXN PASSED_CSTR data_segment_str_pass( pass_p pcell );
//_CALLBACK_FXN PASSED_CSTR tmp_str_pass( pass_p pcell );

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

	union
	{
		bool refresh_partial_row;
		long pad1;
	};

	void_p ptbl;
} nnc_row;

typedef struct
{
	INNER_STR tabname;

	dyn_mms_arr cols; // nnc_column
	dyn_mms_arr rows; // nnc_row

	union
	{
		bool refresh_partial_table;
		bool refresh_table;
		long pad1;
	};

	void_p pnnc;
} nnc_table;

typedef struct
{
	size_t x0 , x1; /*boundaries of tab title*/

} nnc_tabHit;


typedef struct nncursor_requirement
{
	dyn_mms_arr tables; // nnc_table

	dyn_mms_arr * pmain_tbl_shadow; // nnc_table

	size_t active;
	dyn_arr tabHit_arr;
	union
	{
		bool refresh_tabs;
		long pad1;
	};

	char message_text[ 1024 ];

	struct notcurses_options opts;
	struct notcurses * nc;
	struct ncplane * std;

	struct ncplane * tabs_plane;
	struct ncplane * body_plane;
	struct ncplane * msg_plane;
	struct ncplane * cmd_plane;
	int termw , termh;
	pthread_mutex_t nn_lock;

} nnc_req;


status nnc_begin_init_mode( nnc_req * nnc );
Boolean continue_loop_callback( nnc_req * nnc );
void nnc_destroy( nnc_req * nnc );

void nnc_lock_for_changes( nnc_req * nnc );
void nnc_release_lock( nnc_req * nnc );

status nnc_add_table( nnc_req * nnc , PASSED_CSTR tabname , nnc_table ** ptable );
status nnc_add_column( nnc_table * tbl , PASSED_CSTR src_hdr , PASSED_CSTR src_subhdr , size_t src_minw );
status nnc_add_empty_row( nnc_table * tbl , nnc_row ** prow );

status nnc_register_into_page_auto_refresh( nnc_table * ptable , distributor_t * ptor );

// just show string from input
status nnc_set_static_text( nnc_table * tbl , size_t row , size_t col , PASSED_CSTR static_text );
status nnc_set_static_int( nnc_table * tbl , size_t row , size_t col , int i );

// set inner cell content provider . so provide data is caller responsibility
status nnc_set_outer_cell( nnc_table * tbl , size_t row , size_t col , nnc_cell_content * pouter_cell );

// use this function instead of assigning directly
void nnc_cell_triggered( nnc_cell_content * pcell );
void nnc_set_int_cell( nnc_cell_content * pcell , int src_i );
void nnc_set_string_cell( nnc_cell_content * pcell , PASSED_CSTR str );

status get_shadow_cell( nnc_req * nnc , nnc_table * ptbl , size_t row , size_t col , nnc_cell_container ** pcell_container );

//status nnc_get_emptied_cell( nnc_table * tbl , size_t row , size_t col , nnc_cell_content * * pcell );

#endif

