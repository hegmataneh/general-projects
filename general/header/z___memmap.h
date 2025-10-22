#pragma once

#if defined Uses_memmap || !defined __COMPILING

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
typedef struct __attribute__( ( packed ) )
{
	uint32_t magic;
	uint32_t version;
	uint64_t write_offset; /* next write offset in file */
	uint64_t seq_counter;  /* next sequence number */
	uint8_t reserved[ CACHE_LINE_SIZE ];
} memfile_header_t;
//#define MEMFILE_HEADER_SIZE (sizeof(memfile_header_t))

typedef struct __attribute__( ( packed ) ) /*like memfile_header_t*/
{
	uint32_t rec_magic;
	uint32_t rec_version;
	uint64_t seq;
	uint32_t data_len;
} rec_header_t;
//#define REC_HEADER_SIZE (sizeof(rec_header_t))
//#define REC_FOOTER_SIZE (sizeof(uint32_t))
//#define REC_MAGIC 0x52454331 /* "REC1" */
//#define REC_VERSION 1

/* ----------------- Memfile structure ----------------- */
typedef struct
{
	char path[ MAX_PATH ];
	int fd;
	void * map;           /* mmap pointer */
	size_t size; /*mem size*/
	memfile_header_t * hdr; /* pointer into map */
	pthread_mutex_t lock;
	int active; /* 0=free/reserved, 1=active, 2=sealed/closed */
} memfile_t;

/* ----------------- Manager ----------------- */
typedef struct
{
	char base_dir[ MAX_PATH ];
	
	dyn_mms_arr files;         /* array of pointers to memfiles (chain) . memfile_t*/
	
	memfile_t * current;        /* active file for writes */
	memfile_t * hot_spare;      /* preallocated unused memfile */
	pthread_mutex_t lock;      /* protects manager's top-level state */
} mm_manager_t;

#endif

