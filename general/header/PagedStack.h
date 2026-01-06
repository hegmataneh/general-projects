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
	union
	{
		struct
		{
			bool isstack;
			VStack stack;
			char nonuse[ DEFAULT_SFS_BUF_SZ ];
		};
		struct
		{
			bool isnotstack_soisque;
			Vqueue_t que;
			char que_name[ DEFAULT_SFS_BUF_SZ ];
		};
	};
} pg_stk_page_hdr_t; // wriable in file

/* ----------------- Memfile structure ----------------- */
typedef struct
{
	char path[ MAX_PATH ];
	void_p map;					/* mmap pointer */
	size_t memmap_size;			/*mem size*/
	
	pg_stk_page_hdr_t * hdr;	/* pointer into void_p map */
	
	union
	{
		int fd;
		size_t pad1;
	};
	union
	{
		int decrease_time;		// sec
		size_t pad2;
	};
	union
	{
		bool to_be_absolete;	// about to absolete
		size_t pad3;
	};
	//bool absoleted;
	union
	{
		bool nocked_up;			// some insertion happened
		size_t pad4;
	};
	
} pg_stk_memfile_t;				// just in memory

/* ----------------- Manager ----------------- */
typedef struct
{
	char base_dir[ MAX_PATH ];
	dyn_mms_arr files;					/* array of pointers to memfiles (chain) . pg_stk_memfile_t*/
	pg_stk_memfile_t * active_stack;	/* active file for writes */
	pg_stk_memfile_t * hot_stack_spare; /* pre allocated unused memfile */
	void_p custom_data;				/* like g */
	pthread_mutex_t ps_lock;		/* protects manager's top-level state */

	/* lifetime statistics */
	STAT_FLD size_t item_stored;
	STAT_FLD size_t item_stored_byte;
	size_t file_name_counter;

	dict_s_o_t que_access;

} page_stack_t;

typedef enum
{
	pgstk_not_send__stop_sending ,
	pgstk_not_send__stop_sending_delayedMemmap ,
	pgstk_not_send__continue_sending ,
	pgstk_not_send__continue_sending_onTooManyAttempt ,
	pgstk_not_send__not_any_peer , /*not any tcp out so donot fetch memmap files items*/
	
	pgstk_sended__continue_sending ,
	pgstk_sended__stop_sending ,

	pgstk_remapped__continue_sending ,
	pgstk_remapped__stop_sending ,
} pgstk_cmd;

typedef pgstk_cmd ( *ps_callback_data )( void_p , void_p , size_t );

status pg_stk_init( page_stack_t * mm , LPCSTR base_dir , void_p custom_data );
status pg_stk_store_at_stack( page_stack_t * mm , const void_p buf , size_t len );
status pg_stk_store_at_queue( page_stack_t * mm , const void_p buf , size_t len , LPCSTR queue_name );
status pg_stk_try_to_pop_latest( page_stack_t * mm , ps_callback_data data_getter );
size_t pg_stk_get_page_occupied_MB( page_stack_t * mm );
void pg_stk_shutdown( page_stack_t * mm );
_THREAD_FXN void_p cleanup_unused_memfile_proc( pass_p src_g );

#endif
