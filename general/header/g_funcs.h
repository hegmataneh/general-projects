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
