#pragma once

#if defined Uses_PagedStack || !defined __COMPILING

/*
  memmap.c
  Modular memmap manager implementing:
   - 64MiB memfiles (configurable)
   - store(buffer, size) append with rotation
   - preallocated hot-spare file for fast activation
   - durable record layout with footer (LIFO traversal)
   - recover-on-open to trim incomplete tail
   - LIFO consume (pop latest record)
   - metadata chain persisted atomically
   - modular sections, thread-safe
*/

/* ----------------- On-disk record format -----------------
   File layout:
	file header (fixed) -> region of appended records -> unused region
   Record format (append):
	[REC_HEADER: uint32_t magic][uint32_t version][uint64_t seq][uint32_t data_len]
	[data bytes ...]
	[REC_FOOTER: uint32_t data_len]  <-- footer written last and msynced; presence means record durable
   For LIFO traversal: read last 4 bytes as footer, get data_len, backtrack to header.
*/

#ifndef MAX_PATH
#define MAX_PATH 1024
#endif

/* header sizes */
typedef struct /*__attribute__(( packed ))*/
{
	uint32_t magic;
	time_t due_time;	// used to store time in file
	VStack stack;
	
	// TODO . can log retry

} pg_stk_page_hdr_t; // wriable in file


/* ----------------- Memfile structure ----------------- */
typedef struct
{
	char path[ MAX_PATH ];
	void_p map;					/* mmap pointer */
	size_t memmap_size;			/*mem size*/
	pg_stk_page_hdr_t * hdr;	/* pointer into void_p map */
	int fd;
	int decrease_time;			// sec
	bool to_be_absolete;
	//bool absoleted;
	bool nocked_up; // some insertion happened
	
} pg_stk_memfile_t; // just in memory

/* ----------------- Manager ----------------- */
typedef struct
{
	char base_dir[ MAX_PATH ];
	dyn_mms_arr files;				/* array of pointers to memfiles (chain) . pg_stk_memfile_t*/
	pg_stk_memfile_t * current;		/* active file for writes */
	pg_stk_memfile_t * hot_spare;	/* preallocated unused memfile */
	void_p custom_data;				/*like g*/
	pthread_mutex_t ps_lock;		/* protects manager's top-level state */

	//pthread_t trd_clean_unused_memfile;
	//volatile int close_cleaner;

	/* lifetime statistics */
	size_t obsolete_count;
	size_t item_stored;


} page_stack_t;

typedef enum
{
	pgstk_not_send__stop_sending ,
	pgstk_not_send__continue_sending ,
	pgstk_not_send__continue_sending_with_delay ,
	pgstk_sended__continue_sending ,
	pgstk_sended__stop_sending
} pgstk_cmd;

typedef pgstk_cmd ( *ps_callback_data )( void_p , void_p , size_t );

status pg_stk_init( page_stack_t * mm , LPCSTR base_dir , void_p custom_data );
status pg_stk_store( page_stack_t * mm , const void_p buf , size_t len );
status pg_stk_try_to_pop_latest( page_stack_t * mm , ps_callback_data data_getter );
void pg_stk_shutdown( page_stack_t * mm );
_THREAD_FXN void_p cleanup_unused_memfile_proc( pass_p src_g );

#endif
