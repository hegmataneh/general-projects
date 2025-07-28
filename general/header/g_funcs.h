//#pragma once

//-------------------------------------------------------------------------
char *newStr(const char *); // donot use it directly use NEWSTR instead


//-------------------------------------------------------------------------
LPCSTR internalErrorStr(status errValue);
status internalErrorVal(LPCSTR errStr);


//const char * __msg( char * msg_holder , size_t size_of_msg_holder , const char * msg , int line_number );
const char * __snprintf( char * msg_holder , size_t size_of_msg_holder , const char * format , ... );
void _close_socket( int * socket_id );
const char * read_file( const char * path , char * pInBuffer /*= if NULL alloc memory so release deligate to caller*/ );

const char * make_msg_appnd_sys_err( char * msg_holder , size_t size_of_msg_holder , const char * cst_msg );

const char * format_pps( char * buf , size_t buflen , ubigint pps , int number_of_float /*=2*/ , const char * unit_name /*= "pps"*/);

void format_clock_time( const struct timespec * ts , char * buffer , size_t buf_size );

void round_up_to_next_interval( struct timespec * now , int min_val , int interval , struct timespec * result ); // use min as least value to sleep and interval to round up to iteration of that value

FILE* create_unique_file(const char* path, const char* filename /*=NULL(app+date)*/ );

void format_elapsed_time( time_t start , time_t end , char * buffer , size_t buf_size );