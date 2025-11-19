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

#endif
