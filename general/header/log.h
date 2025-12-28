#pragma once

#if defined Uses_LOG || !defined __COMPILING

// ENABLE_LOGGING

typedef enum
{
	LOG_DEBUG ,
	LOG_INFO ,
	LOG_WARN ,
	LOG_ERROR
} LogLevel;

// Initialize logger with a file path (creates or appends)
status log_init( const char * filename , bool bnew );

// Write a formatted log message with level and timestamp
// this fxn put \n at the end
void log_write( LogLevel level , const char * fmt , ... );

// Close the log file
void log_close( void );

#define COOLDOWN_LOGGING_EXP( exp ) do { \
		static uint8_t delay = 0; static time_t t = 0; \
		if ( ( time( NULL ) - t ) >= delay ) \
		{ exp; if ( !delay ) { delay = 1; } else { if ( delay < 10 ) delay += 1; } } } while(0)


#ifdef ENABLE_LOCK_LOGGING

_EXTERN void * __log_lock;
_EXTERN int __log_lock_pos;

#undef PG_STCK_LOCK_LINE
#undef VQUEUE_LOCK_LINE
#undef VSTACK_LOCK_LINE
#undef BP_LOCK_LINE
#undef PKT_MGR_LOCK_LINE
#undef DEFRG_LOCK_LINE

#define LOG_LOCK_LINE( exp ) ( { \
								__log_lock_pos += sprintf( ( char * )__log_lock + __log_lock_pos , " (lck(%s,%d)" , __FUNCTION__ , ( int )__LINE__ ); \
								exp; \
								__log_lock_pos += sprintf( ( char * )__log_lock + __log_lock_pos , " (%s,%d)lck)\r\n" , __FUNCTION__ , ( int )__LINE__ ); \
							} )




#define PG_STCK_LOCK_LINE( exp )	LOG_LOCK_LINE( exp )
#define VQUEUE_LOCK_LINE( exp )		LOG_LOCK_LINE( exp )
#define VSTACK_LOCK_LINE( exp )		LOG_LOCK_LINE( exp )
#define BP_LOCK_LINE( exp )			LOG_LOCK_LINE( exp )
#define PKT_MGR_LOCK_LINE( exp )	LOG_LOCK_LINE( exp )
#define DEFRG_LOCK_LINE( exp )		LOG_LOCK_LINE( exp )

#endif

#endif
