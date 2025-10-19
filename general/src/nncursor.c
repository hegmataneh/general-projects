#define _XOPEN_SOURCE 700

//#define Uses_STRDUP
#define Uses_WARNING
#define Uses_MEMSET_ZERO_O
#define Uses_INIT_BREAKABLE_FXN
#define Uses_nncursor
#include <general.dep>

_PRIVATE_FXN size_t slen( PASSED_CSTR s )
{
	return s ? strlen( s ) : ( size_t )0;
}

status nnc_begin_init_mode( nnc_req * nnc )
{
	INIT_BREAKABLE_FXN();

	setlocale( LC_ALL , "" );
	MEMSET_ZERO_O( nnc );
	struct notcurses_options opts = { 0 };
	opts.flags = NCOPTION_SUPPRESS_BANNERS;
	nnc->nc = notcurses_core_init( &opts , NULL );
	N_BREAK_IF( !nnc->nc , errCreation , 0 );
	
	notcurses_mice_enable( nnc->nc , NCMICE_BUTTON_EVENT );

	nnc->std = notcurses_stdplane( nnc->nc ); 
	ncplane_dim_yx( nnc->std , &nnc->termh , &nnc->termw );

	struct ncplane_options topts = { .y = 0 , .x = 0 , .rows = ( unsigned int )TAB_BAR_HEIGHT , .cols = ( unsigned int )nnc->termw };
	nnc->tabs_plane = ncplane_create( nnc->std , &topts );
	N_BREAK_IF( !nnc->tabs_plane , errCreation , 0 );

	struct ncplane_options bopts = { .y = TAB_BAR_HEIGHT , .x = 0 , .rows = ( unsigned int )( nnc->termh - TAB_BAR_HEIGHT - CMD_H ) , .cols = ( unsigned int )nnc->termw };
	nnc->body_plane = ncplane_create( nnc->std , &bopts );
	N_BREAK_IF( !nnc->body_plane , errCreation , 0 );

	struct ncplane_options copts = { .y = TAB_BAR_HEIGHT + ( nnc->termh - TAB_BAR_HEIGHT - CMD_H ) , .x = 0 , .rows = ( unsigned int )CMD_H , .cols = ( unsigned int )nnc->termw };
	nnc->cmd_plane = ncplane_create( nnc->std , &copts );
	N_BREAK_IF( !nnc->cmd_plane , errCreation , 0 );

	BREAK_STAT( mms_array_init( &nnc->tables , sizeof( nnc_table ) , 1 , GROW_STEP , 0 ) , 0 );

	BREAK_STAT( array_init( &nnc->tabHit_arr , sizeof( nnc_tabHit ) , 1 , GROW_STEP , 0 ) , 0 );

	nnc->refresh_tabs = true;

	BEGIN_RET
	END_RET
}

status nnc_add_table( nnc_req * nnc , PASSED_CSTR tabname , nnc_table ** ptable )
{
	INIT_BREAKABLE_FXN();

	BREAK_STAT( mms_array_get_one_available_unoccopied_item( &nnc->tables , ( void ** )ptable ) , 0 );
	BREAK_IF( !( (*ptable)->tabname = strdup( tabname ) ) , errMemoryLow , 0 );
	
	(*ptable)->pnnc = nnc;

	BREAK_STAT( mms_array_init( &(*ptable)->cols , sizeof( nnc_column ) , 1 , GROW_STEP , 0 ) , 0 );
	BREAK_STAT( mms_array_init( &(*ptable)->rows , sizeof( nnc_row ) , 1 , GROW_STEP , 0 ) , 0 );
	
	BREAK_STAT( array_resize( &nnc->tabHit_arr , nnc->tables.count , nnc->tables.count ) , 0 );

	( *ptable )->refresh_table = true;
	nnc->refresh_tabs = true;

	// TODO . be aware of memory leak

	BEGIN_RET
	END_RET
}

status nnc_add_column( nnc_table * ptbl , PASSED_CSTR src_hdr , PASSED_CSTR src_subhdr , size_t src_minw )
{
	INIT_BREAKABLE_FXN();

	nnc_column * pcol = NULL;
	BREAK_STAT( mms_array_get_one_available_unoccopied_item( &ptbl->cols , ( void ** )&pcol ) , 0 );
	BREAK_IF( !( pcol->hdr = strdup( src_hdr ) ) , errMemoryLow , 0 );
	BREAK_IF( !( pcol->subhdr = strdup( src_subhdr ) ) , errMemoryLow , 0 );
	pcol->minw = src_minw;
	pcol->ptbl = ptbl;

	BEGIN_RET
	END_RET
}

status nnc_add_empty_row( nnc_table * tbl , nnc_row ** prow )
{
	INIT_BREAKABLE_FXN();

	nnc_row * tmp_row = NULL;
	if ( prow == NULL )
	{
		prow = &tmp_row;
	}

	BREAK_STAT( mms_array_get_one_available_unoccopied_item( &tbl->rows , ( void ** )prow ) , 0 );
	size_t col_size = mms_array_get_count( &tbl->cols );
	BREAK_STAT( array_init( &(*prow)->cell_containers , sizeof( nnc_cell_container ) , col_size , GROW_STEP , col_size ) , 0 );

	( *prow )->ptbl = tbl;

	BEGIN_RET
	END_RET
}

_PRIVATE_FXN void nnc_clean_prev_cell_content( nnc_cell_container * pcell_container )
{
	if ( pcell_container )
	{
		if ( pcell_container->pcell && pcell_container->pcell->clean_fxn ) // if there is clean function then clean pointer then zero pointer
		{
			pcell_container->pcell->clean_fxn( ( pass_p )pcell_container );
		}
		pcell_container->pcell = NULL; // if pointer come from outside then clean should be done outside
	}
}

_CALLBACK_FXN void clean_cell_container( void_p src_cell_container )
{
	nnc_cell_container * pcell_container = ( nnc_cell_container * )src_cell_container;
	if ( pcell_container && pcell_container->pcell )
	{
		DAC( pcell_container->pcell );
	}
}

status nnc_set_static_text( nnc_table * tbl , size_t row , size_t col , PASSED_CSTR static_text )
{
	INIT_BREAKABLE_FXN();

	if ( row < 0 ) return errOverflow;
	if ( ( d_error = mms_array_idx_exist_s( &tbl->rows , row ) ) ) return d_error;
	if ( col < 0 )  return errOverflow;
	if ( ( d_error = mms_array_idx_exist_s( &tbl->cols , col ) ) ) return d_error;

	nnc_row * prow = NULL;
	BREAK_STAT( mms_array_get_s( &tbl->rows , row , ( void ** )&prow ) , 0 );
	
	nnc_cell_container * pcell_container = NULL;
	BREAK_STAT( array_get_s( &prow->cell_containers , col , ( void ** )&pcell_container ) , 0 );

	nnc_clean_prev_cell_content( pcell_container );

	BREAK_IF( !( pcell_container->pcell = CALLOC_ONE( pcell_container->pcell ) ) , errMemoryLow , 0 );

	pcell_container->pcell->prow = NULL;
	pcell_container->pcell->storage.bt.pass_str = static_text;
	pcell_container->pcell->clean_fxn = clean_cell_container;
	pcell_container->pcell->conversion_fxn = data_segment_str_pass;
	pcell_container->pcell->propagate_changes = NULL; // no changes acceptable for static cell

	//ptbl->rows[ row ].cells[ col ] = strdup( text );

	BEGIN_RET
	END_RET
}

_CALLBACK_FXN void propagate_cell_changes( void_p src_cell_container )
{
	nnc_cell_container * pcell_container = ( nnc_cell_container * )src_cell_container;
	if ( pcell_container && pcell_container->pcell )
	{
		pcell_container->pcell->refresh_cell = true;
		nnc_row * prow = ( nnc_row * )pcell_container->pcell->prow;
		prow->refresh_partial_row = true;
		nnc_table * ptbl = ( nnc_table * )prow->ptbl;
		ptbl->refresh_partial_table = true;
	}
}

status nnc_set_outer_cell( nnc_table * tbl , size_t row , size_t col , nnc_cell_content * pouter_cell )
{
	INIT_BREAKABLE_FXN();

	if ( row < 0 ) return errOverflow;
	if ( ( d_error = mms_array_idx_exist_s( &tbl->rows , row ) ) ) return d_error;
	if ( col < 0 )  return errOverflow;
	if ( ( d_error = mms_array_idx_exist_s( &tbl->cols , col ) ) ) return d_error;

	nnc_row * prow = NULL;
	BREAK_STAT( mms_array_get_s( &tbl->rows , row , ( void ** )&prow ) , 0 );

	nnc_cell_container * pcell_container = NULL;
	BREAK_STAT( array_get_s( &prow->cell_containers , col , ( void ** )&pcell_container ) , 0 );

	nnc_clean_prev_cell_content( pcell_container );

	pcell_container->pcell = pouter_cell;
	pouter_cell->pcell_container = pcell_container;
	pcell_container->pcell->prow = prow;
	pcell_container->pcell->propagate_changes = propagate_cell_changes;
	//pcell_container->pcell->conversion_fxn = data_segment_str_pass;
	
	//pcell_container->pcell->val.pass_str = static_text;
	//pcell_container->pcell->clean_fxn = NULL;
	//pcell_container->pcell->propagate_changes = NULL; // no changes acceptable for static cell
	//ptbl->rows[ row ].cells[ col ] = strdup( text );

	BEGIN_RET
	END_RET
}

void nnc_cell_triggered( nnc_cell_content * pcell )
{
	if ( pcell && pcell->propagate_changes )
	{
		pcell->propagate_changes( pcell->pcell_container );
	}
}

void nnc_set_int_cell( nnc_cell_content * pcell , int src_i )
{
	pcell->storage.bt.i = src_i;
	nnc_cell_triggered( pcell );
}

void nnc_set_string_cell( nnc_cell_content * pcell , PASSED_CSTR str )
{
	WARNING( strlen(str) <= DEFAULT_SFS_BUF_SZ );
	strcpy( pcell->storage.tmpbuf , str );
	nnc_cell_triggered( pcell );
}

//status nnc_get_emptied_cell( nnc_table * ptbl , size_t row , size_t col , nnc_cell_content * * ppcell )
//{
//	INIT_BREAKABLE_FXN();
//
//	if ( row < 0 ) return errOverflow;
//	if ( !array_idx_exist( &ptbl->rows , row ) ) return errOverflow;
//	if ( col < 0 )  return errOverflow;
//	if ( !array_idx_exist( &ptbl->cols , col ) ) return errOverflow;
//
//	nnc_row * prow = NULL;
//	BREAK_STAT( array_get_s( &ptbl->rows , row , ( void ** )&prow ) , 0 );
//	BREAK_STAT( array_get_s( &prow->cells , col , ( void ** )ppcell ) , 0 );
//
//	nnc_clean_prev_cell_content( *ppcell );
//
//	BEGIN_RET
//	END_RET
//}

void nnc_destroy( nnc_req * nnc )
{
	notcurses_stop( nnc->nc );

	size_t tbl_cnt = mms_array_get_count( &nnc->tables );
	for ( size_t itbl = 0; itbl < tbl_cnt; itbl++ )
	{
		nnc_table * ptbl = NULL;
		if ( mms_array_get_s( &nnc->tables , itbl , ( void ** )&ptbl ) == errOK )
		{
			size_t col_cnt = mms_array_get_count( &ptbl->cols );
			for ( size_t icl = 0; icl < col_cnt; icl++ )
			{
				nnc_column * pcol = NULL;
				if ( mms_array_get_s( &ptbl->cols , icl , ( void ** )&pcol ) == errOK )
				{
					DAC( pcol->hdr );
					DAC( pcol->subhdr );
				}
			}
			mms_array_free( &ptbl->cols );

			size_t rw_cnt = mms_array_get_count( &ptbl->rows );
			for ( size_t irw = 0; irw < rw_cnt; irw++ )
			{
				nnc_row * prow = NULL;
				if ( mms_array_get_s( &ptbl->rows , irw , ( void ** )&prow ) == errOK )
				{
					size_t cel_sz = array_get_count( &prow->cell_containers );
					for ( size_t cel = 0; cel < cel_sz; cel++ )
					{
						nnc_cell_container * pcell_container = NULL;
						if ( array_get_s( &prow->cell_containers , cel , ( void ** )&pcell_container ) == errOK )
						{
							nnc_clean_prev_cell_content( pcell_container );
						}
					}
					array_free( &prow->cell_containers );
				}
			}
			mms_array_free( &ptbl->rows );

			DAC( ptbl->tabname );
		}
	}
	mms_array_free( &nnc->tables );
	array_free( &nnc->tabHit_arr );
}

// Compute column widths (fit to contents)
_PRIVATE_FXN void compute_widths( nnc_table * ptbl , size_t screen_w , size_t * arr_col_sz )
{
	size_t col_cnt = mms_array_get_count( &ptbl->cols );
	MEMSET_ZERO( arr_col_sz , col_cnt );
	for ( size_t icl = 0; icl < col_cnt; icl++ ) // iterate throw columns
	{
		nnc_column * pcol = NULL;
		if ( mms_array_get_s( &ptbl->cols , icl , ( void ** )&pcol ) == errOK )
		{
			arr_col_sz[ icl ] = slen( pcol->hdr );
			size_t subhdr_sz = slen( pcol->subhdr );
			if ( arr_col_sz[ icl ] < subhdr_sz ) arr_col_sz[ icl ] = subhdr_sz;
			if ( arr_col_sz[ icl ] < pcol->minw ) arr_col_sz[ icl ] = pcol->minw;
		}

		size_t rw_cnt = mms_array_get_count( &ptbl->rows );
		for ( size_t irw = 0; irw < rw_cnt; irw++ )
		{
			nnc_row * prow = NULL;
			if ( mms_array_get_s( &ptbl->rows , irw , ( void ** )&prow ) == errOK )
			{
				nnc_cell_container * pcell_container = NULL;
				if ( array_get_s( &prow->cell_containers , icl , ( void ** )&pcell_container ) == errOK && pcell_container && pcell_container->pcell && pcell_container->pcell->conversion_fxn )
				{
					size_t content_sz = slen( pcell_container->pcell->conversion_fxn( pcell_container->pcell ) );
					if ( arr_col_sz[ icl ] < content_sz ) arr_col_sz[ icl ] = content_sz;
				}
			}
		}
	}
	int need = 0;
	for ( size_t icl = 0; icl < col_cnt ; icl++ )
	{
		need += ( int )arr_col_sz[ icl ];
	}
	need += ( int )col_cnt + 1; // separators
	if ( need > screen_w )
	{
		int over = need - ( int )screen_w;
		while ( over > 0 )
		{
			int maxidx = -1 , maxw = -1;
			for ( size_t icl = 0; icl < col_cnt; icl++ )
			{
				if ( arr_col_sz[ icl ] > maxw && arr_col_sz[ icl ] > MIN_COL_SIZE ) // find max col size
				{
					maxw = ( int )arr_col_sz[ icl ]; maxidx = ( int )icl;
				}
			}
			if ( maxidx < 0 ) break;
			arr_col_sz[ maxidx ]--; arr_col_sz--; // decrease most fat col
		}
	}
}

_PRIVATE_FXN void put_clipped( struct ncplane * plane , int y , int x , PASSED_CSTR s /*=NULL if cell arrive*/ , nnc_cell_content * pcell /*=NULL if constant cell arrive*/ , size_t w )
{
	if ( !s && pcell && pcell->conversion_fxn ) s = pcell->conversion_fxn( pcell );
	if ( !s ) s = "";
	if ( pcell )
	{
		pcell->paint_s.y = y;
		pcell->paint_s.x = x;
		pcell->paint_s.width = w;
	}
	size_t L = slen( s );
	if ( L <= w )
	{
		int i = 0;
		for ( i = 0; i < ( ( w - L + 1 ) / 2 ) ; i++ ) ncplane_putstr_yx( plane , y , x + i , " " );
		ncplane_putstr_yx( plane , y , x + i , s ); i += ( int )L;
		for ( ; i < ( int )w; i++ ) ncplane_putstr_yx( plane , y , x + i , " " );
	}
	else
	{
		if ( w <= 3 )
		{
			for ( int i = 0; i < ( int )w; i++ ) ncplane_putstr_yx( plane , y , x + i , "." );
		}
		else
		{
			for ( int i = 0; i < ( int )w - 3; i++ ) ncplane_putnstr_yx( plane , y , x + i , 1 , s + i );
			ncplane_putstr_yx( plane , y , x + ( int )w - 3 , "..." );
		}
	}
	if ( pcell )
	{
		pcell->refresh_cell = false;
	}
}

_PRIVATE_FXN void draw_tabs( nnc_req * nnc , _RET_VAL_P nnc_tabHit * hits )
{
	ncplane_erase( nnc->tabs_plane );
	size_t x = 1;
	size_t tabs_cnt = mms_array_get_count( &nnc->tables );
	for ( size_t itb = 0; itb < tabs_cnt; itb++ )
	{
		nnc_table * ptbl = NULL;
		if ( mms_array_get_s( &nnc->tables , itb , ( void ** )&ptbl ) == errOK )
		{
			size_t w = slen( ptbl->tabname ) + 2;
			if ( itb == nnc->active )
			{
				ncplane_set_fg_rgb8( nnc->tabs_plane , RED_PART( WHITE ) , GREEN_PART( WHITE ) , BLUE_PART( WHITE ) );
				ncplane_set_bg_rgb8( nnc->tabs_plane , RED_PART( BLUE_3 ) , GREEN_PART( BLUE_3 ) , BLUE_PART( BLUE_3 ) );
				ncplane_set_styles( nnc->tabs_plane , NCSTYLE_BOLD );
			}
			else
			{
				ncplane_set_fg_rgb8( nnc->tabs_plane , RED_PART( BLACK ) , GREEN_PART( BLACK ) , BLUE_PART( BLACK ) );
				ncplane_set_bg_rgb8( nnc->tabs_plane , RED_PART( LIGHT_GRAY ) , GREEN_PART( LIGHT_GRAY ) , BLUE_PART( LIGHT_GRAY ) );
			}
			ncplane_putstr_yx( nnc->tabs_plane , 1 , (int)x , " " );
			ncplane_putstr( nnc->tabs_plane , ptbl->tabname );
			ncplane_putstr( nnc->tabs_plane , " " );
			ncplane_set_styles( nnc->tabs_plane , NCSTYLE_NONE );
			hits[ itb ].x0 = x;
			hits[ itb ].x1 = x + w - 1;
			x += w + 1;
		}
	}
	for ( int i = 0; i < nnc->termw; i++ )
	{
		ncplane_set_fg_rgb8( nnc->tabs_plane , RED_PART( GRAY ) , GREEN_PART( GRAY ) , BLUE_PART( GRAY ) ); 
		ncplane_set_bg_rgb8( nnc->tabs_plane , RED_PART( BLUE_3 ) , GREEN_PART( BLUE_3 ) , BLUE_PART( BLUE_3 ) );
		ncplane_putstr_yx( nnc->tabs_plane , 2 , i , "─" );
	}
	ncplane_set_fg_default( nnc->tabs_plane );
	ncplane_set_bg_default( nnc->tabs_plane );
	nnc->refresh_tabs = false;
}

_PRIVATE_FXN void draw_table( nnc_req * nnc , nnc_table * ptbl )
{
	INIT_BREAKABLE_FXN();

	ncplane_erase( nnc->body_plane );
	
	size_t col_cnt = mms_array_get_count( &ptbl->cols );
	dyn_arr int_arr;
	BREAK_STAT( array_init( &int_arr , sizeof( size_t ) , col_cnt , GROW_STEP , col_cnt ) , 0 );
	
	size_t * colw = ( size_t * )int_arr.data;
	compute_widths( ptbl , (size_t)(nnc->termw - 2) , colw );
	int x = 0 , y = 0;
	// top border
	ncplane_putstr_yx( nnc->body_plane , y++ , 0 , "┌" );
	for ( size_t icl = 0; icl < col_cnt; icl++ )
	{
		for ( int k = 0; k < colw[ icl ]; k++ ) ncplane_putstr( nnc->body_plane , "─" );
		if ( icl == col_cnt - 1 ) ncplane_putstr( nnc->body_plane , "┐" );
		else ncplane_putstr( nnc->body_plane , "┬" );
	}

	// header row

	ncplane_set_fg_default( nnc->body_plane ); ncplane_set_bg_default( nnc->body_plane );
	ncplane_putstr_yx( nnc->body_plane , y , 0 , "│" );

	for ( size_t icl = 0; icl < col_cnt; icl++ )
	{
		nnc_column * pcol = NULL;
		if ( mms_array_get_s( &ptbl->cols , icl , ( void ** )&pcol ) == errOK )
		{
			ncplane_set_fg_rgb8( nnc->body_plane , RED_PART( WHITE ) , GREEN_PART( WHITE ) , BLUE_PART( WHITE ) );
			ncplane_set_bg_rgb8( nnc->body_plane , RED_PART( BLUE_4 ) , GREEN_PART( BLUE_4 ) , BLUE_PART( BLUE_4 ) );
			ncplane_set_styles( nnc->body_plane , NCSTYLE_BOLD | NCSTYLE_UNDERLINE );
			
			put_clipped( nnc->body_plane , y , 1 + x , pcol->hdr , NULL , colw[ icl ] );
			x += ( int )colw[ icl ];

			ncplane_set_fg_default( nnc->body_plane ); ncplane_set_bg_default( nnc->body_plane );
			ncplane_putstr_yx( nnc->body_plane , y , 1 + x , "│" );
			x += 1;
		}
	}
	ncplane_set_styles( nnc->body_plane , NCSTYLE_NONE );
	y++;
	
	// Subheader
	// TODO . if all subheader is empty so why allocate space for it
	x = 0;
	ncplane_set_fg_default( nnc->body_plane ); ncplane_set_bg_default( nnc->body_plane );
	ncplane_putstr_yx( nnc->body_plane , y , 0 , "│" );

	for ( size_t icl = 0; icl < col_cnt; icl++ )
	{
		nnc_column * pcol = NULL;
		if ( mms_array_get_s( &ptbl->cols , icl , ( void ** )&pcol ) == errOK )
		{
			ncplane_set_fg_rgb8( nnc->body_plane , RED_PART( WHITE ) , GREEN_PART( WHITE ) , BLUE_PART( WHITE ) );
			//ncplane_set_bg_rgb8( nnc->body_plane , RED_PART( BLUE_4 ) , GREEN_PART( BLUE_4 ) , BLUE_PART( BLUE_4 ) );
			put_clipped( nnc->body_plane , y , 1 + x , pcol->subhdr , NULL , colw[ icl ] );
			x += ( int )colw[ icl ];
			
			ncplane_set_fg_default( nnc->body_plane ); ncplane_set_bg_default( nnc->body_plane );
			ncplane_putstr_yx( nnc->body_plane , y , 1 + x , "│" ); x += 1;
		}
	}

	ncplane_set_styles( nnc->body_plane , NCSTYLE_NONE ); ncplane_set_fg_default( nnc->body_plane ); y++;

	// separator under header
	ncplane_putstr_yx( nnc->body_plane , y++ , 0 , "├" );
	for ( size_t icl = 0; icl < col_cnt; icl++ )
	{
		for ( size_t k = 0; k < colw[ icl ]; k++ ) ncplane_putstr( nnc->body_plane , "─" );
		if ( icl == col_cnt - 1 ) ncplane_putstr( nnc->body_plane , "┤" );
		else ncplane_putstr( nnc->body_plane , "┼" );
	}
	
	// rows
	size_t row_cnt = mms_array_get_count( &ptbl->rows );
	for ( size_t irw = 0; irw < row_cnt; irw++ )
	{
		x = 0;
		
		if ( irw % 2 == 0 )
		{
			ncplane_set_bg_rgb8( nnc->body_plane , RED_PART( C_RGB_1 ) , GREEN_PART( C_RGB_1 ) , BLUE_PART( C_RGB_1 ) );
		}
		else
		{
			ncplane_set_bg_rgb8( nnc->body_plane , RED_PART( C_RGB_2 ) , GREEN_PART( C_RGB_2 ) , BLUE_PART( C_RGB_2 ) );
		}

		ncplane_putstr_yx( nnc->body_plane , y , 0 , "│" );
		
		for ( size_t icl = 0; icl < col_cnt; icl++ )
		{
			nnc_row * prow = NULL;
			if ( mms_array_get_s( &ptbl->rows , irw , ( void ** )&prow ) == errOK )
			{
				nnc_cell_container * pcell_container = NULL;
				if ( array_get_s( &prow->cell_containers , icl , ( void ** )&pcell_container ) == errOK && pcell_container && pcell_container->pcell )
				{
					put_clipped( nnc->body_plane , y , 1 + x , NULL , pcell_container->pcell , colw[ icl ] );					
				}

				prow->refresh_partial_row = false;
			}
			x += ( int )colw[ icl ];

			//ncplane_set_fg_default( nnc->body_plane ); ncplane_set_bg_default( nnc->body_plane );
			ncplane_putstr_yx( nnc->body_plane , y , 1 + x , "│" );
			x += 1;
		}
		ncplane_set_bg_default( nnc->body_plane );
		y++;
	}

	// bottom border
	ncplane_putstr_yx( nnc->body_plane , y++ , 0 , "└" );
	for ( size_t icl = 0; icl < col_cnt; icl++ )
	{
		for ( size_t k = 0; k < colw[ icl ]; k++ ) ncplane_putstr( nnc->body_plane , "─" );
		if ( icl == col_cnt - 1 ) ncplane_putstr( nnc->body_plane , "┘" );
		else ncplane_putstr( nnc->body_plane , "┴" );
	}

	array_free( &int_arr );

	ptbl->refresh_table = false;
	ptbl->refresh_partial_table = false;

	BEGIN_RET
	V_END_RET
}

_PRIVATE_FXN void draw_partial_table( nnc_req * nnc , nnc_table * ptbl )
{
	//INIT_BREAKABLE_FXN();

	size_t rw_cnt = mms_array_get_count( &ptbl->rows );
	for ( size_t irw = 0; irw < rw_cnt; irw++ )
	{
		nnc_row * prow = NULL;
		if ( mms_array_get_s( &ptbl->rows , irw , ( void ** )&prow ) == errOK && prow->refresh_partial_row )
		{
			size_t cel_sz = array_get_count( &prow->cell_containers );
			for ( size_t cel = 0; cel < cel_sz; cel++ )
			{
				nnc_cell_container * pcell_container = NULL;
				if ( array_get_s( &prow->cell_containers , cel , ( void ** )&pcell_container ) == errOK && pcell_container->pcell && pcell_container->pcell->refresh_cell )
				{
					nnc_cell_content * pcell = pcell_container->pcell;

					ncplane_erase_region( nnc->body_plane , pcell->paint_s.y , pcell->paint_s.x , 1 , ( int )pcell->paint_s.width );
					put_clipped( nnc->body_plane , pcell->paint_s.y , pcell->paint_s.x , NULL , pcell , pcell->paint_s.width );
				}
			}
			prow->refresh_partial_row = false;
		}
	}

	ptbl->refresh_partial_table = false;

	//BEGIN_RET
	//V_END_RET
}

// Draw command box
_PRIVATE_FXN void draw_cmdbox( struct ncplane * cmd , const char * buf )
{
	ncplane_erase( cmd );
	int w , h; ncplane_dim_yx( cmd , &h , &w );
	ncplane_set_fg_rgb8( cmd , 255 , 165 , 0 ); ncplane_set_bg_rgb8( cmd , 10 , 10 , 40 ); ncplane_set_styles( cmd , NCSTYLE_BOLD );
	ncplane_putstr_yx( cmd , 0 , 0 , "┌" ); for ( int i = 1; i < w - 1; i++ ) ncplane_putstr( cmd , "─" ); ncplane_putstr( cmd , "┐" );
	ncplane_putstr_yx( cmd , 1 , 0 , "│ Command: " ); ncplane_putstr( cmd , buf ? buf : "" ); ncplane_putstr_yx( cmd , 1 , w - 1 , "│" );
	ncplane_putstr_yx( cmd , 2 , 0 , "└" ); for ( int i = 1; i < w - 1; i++ ) ncplane_putstr( cmd , "─" ); ncplane_putstr( cmd , "┘" );
	ncplane_set_styles( cmd , NCSTYLE_NONE ); ncplane_set_fg_default( cmd ); ncplane_set_bg_default( cmd );
}

_PRIVATE_FXN void draw( nnc_req * nnc )
{
	if ( nnc->refresh_tabs )
	{
		draw_tabs( nnc , ( nnc_tabHit * )nnc->tabHit_arr.data );
	}

	nnc_table * ptbl = NULL;
	if ( mms_array_get_s( &nnc->tables , nnc->active , ( void ** )&ptbl ) == errOK )
	{
		if ( ptbl->refresh_table )
		{
			draw_table( nnc , ptbl );
		}
		else if ( ptbl->refresh_partial_table )
		{
			draw_partial_table( nnc , ptbl );
		}
	}

	//char cmdbuf[ 128 ] = { 0 }; int cmdpos = 0;
	//draw_cmdbox( nnc->cmd_plane , cmdbuf );
	
	notcurses_render( nnc->nc );
}

Boolean continue_loop_callback( nnc_req * nnc )
{
	// act by events
	struct ncinput ni;
	uint32_t id = notcurses_get_nblock( nnc->nc , &ni );
	if ( id == 'q' ) return False;
	if ( id == NCKEY_RESIZE )
	{
		nnc->refresh_tabs = true;
		nnc_table * ptbl = NULL;
		if ( mms_array_get_s( &nnc->tables , nnc->active , ( void ** )&ptbl ) == errOK )
		{
			ptbl->refresh_table = true;
		}

		ncplane_dim_yx( nnc->std , &nnc->termh , &nnc->termw );
		ncplane_resize_simple( nnc->tabs_plane , ( unsigned int )TAB_BAR_HEIGHT , ( unsigned int )nnc->termw );
		ncplane_move_yx( nnc->body_plane , TAB_BAR_HEIGHT , 0 );
		ncplane_resize_simple( nnc->body_plane , ( unsigned int )( nnc->termh - TAB_BAR_HEIGHT ) , ( unsigned int )nnc->termw );
		//draw( nnc );
		//return True;
	}
	if ( ni.evtype == NCTYPE_PRESS && id == NCKEY_BUTTON1 )
	{
		if ( ni.y == 1 )
		{
			nnc_tabHit * ptabHit = ( nnc_tabHit * )nnc->tabHit_arr.data;
			for ( size_t i = 0; i < nnc->tabHit_arr.count ; i++ )
			{
				if ( ni.x >= ptabHit[ i ].x0 && ni.x <= ptabHit[ i ].x1 )
				{
					nnc->active = i;

					nnc->refresh_tabs = true;
					nnc_table * ptbl = NULL;
					if ( mms_array_get_s( &nnc->tables , nnc->active , ( void ** )&ptbl ) == errOK )
					{
						ptbl->refresh_table = true;
					}

					draw( nnc );
					break;
				}
			}
		}
	}
	
	// iterativly refresh table
	nnc_table * ptbl = NULL;
	if ( mms_array_get_s( &nnc->tables , nnc->active , ( void ** )&ptbl ) == errOK )
	{
		if ( ptbl->refresh_table || ptbl->refresh_partial_table || nnc->refresh_tabs )
		{
			draw( nnc );
		}
	}

	return True;
}

_CALLBACK_FXN PASSED_CSTR data_segment_str_pass( pass_p src_pcell )
{
	nnc_cell_content * pcell = ( nnc_cell_content * )src_pcell;
	return pcell->storage.bt.pass_str;
}

//_CALLBACK_FXN PASSED_CSTR tmp_str_pass( pass_p pcell )
//{
//	nnc_cell_content * pcell = ( nnc_cell_content * )src_pcell;
//	return ( PASSED_CSTR )pcell->storage.tmpbuf;
//}

_CALLBACK_FXN PASSED_CSTR i_as_str_pass( pass_p src_pcell )
{
	nnc_cell_content * pcell = ( nnc_cell_content * )src_pcell;
	sprintf( pcell->storage.tmpbuf , "%d" , pcell->storage.bt.i );
	return ( PASSED_CSTR )pcell->storage.tmpbuf;
}

_CALLBACK_FXN _PRIVATE_FXN void nnc_page_auto_refresh( void_p src_page )
{
	nnc_table * ptable = ( nnc_table * )src_page;
	for ( size_t irow = 0 ; irow < ptable->rows.count ; irow++ )
	{
		nnc_row * prow = NULL;
		if ( mms_array_get_s( &ptable->rows , irow , ( void ** )&prow ) == errOK )
		{
			for ( size_t icell = 0 ; icell < prow->cell_containers.count ; icell++ )
			{
				nnc_cell_container * pcell = NULL;
				if ( array_get_s( &prow->cell_containers , icell , (void**)&pcell ) == errOK )
				{
					if ( pcell && pcell->pcell && pcell->pcell->propagate_changes )
					{
						pcell->pcell->propagate_changes( pcell );
					}
				}
			}
		}
	}
}

status nnc_register_into_page_auto_refresh( nnc_table * ptable , distributor_t * ptor )
{
	return distributor_subscribe( ptor , SUB_VOID , SUB_FXN( nnc_page_auto_refresh ) , ptable );
}
