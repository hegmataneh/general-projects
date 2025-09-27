#pragma once

//-------------------------------------------------------------------------
extern const char EOS; // 0
//const WCHAR WEOS = _WT('\0'); // 0
extern const char LF ; // line feed 10
extern const char CR ; // carriage return 13

extern const char _newLineFeed[] ;

extern const char _fileNameDelimiter[] ;

extern const char _SPFD ; // Standard path file delimiter

extern const char _UTF8Sign[] ;
extern const char _UTF16LSign[] ; // little-endian . Unicode in Microsoft terminology
extern const char _UTF16BSign[] ; // big-endian . Unicode in Microsoft terminology
extern const char _UTF32LSign[] ; // little-endian
extern const char _UTF32BSign[] ; // big-endian

//-------------------------------------------------------------------------
enum
{
	NEXT_GENERAL_ERROR_VALUE = -18,
	errOverflow = -17, // if cannot fit and grow not allowed
	one_time_echo_err = -16, // origin -> this err just use for condition where at app level one M_V_END_RET show err msg and outer caller should ignore it
	errCreation = -15,
	errSocket = -14,
	errDevice = -13,
	errNotFound = -12,
	errNoPeer = -11, // origin -> tcp peer disconnected
	errMaximumExceeded = -10,
	errOutofRanje = -9,
	errPeerClosed = -8,
	errTimeout = -7,
	errArg = -6, // origin -> bad argument
	errSyntax = -5,
	errCanceled = -4,  // indeed this indicates a normal abort and is not an d_error condition
	errInvalidString = -3,
	errMemoryLow = -2,
	errGeneral = -1,
	errOK = 0,
	errSystem = 1, // this error is not public error value
};

//-------------------------------------------------------------------------
//extern uchar _ErrLvl; // does not dependable in multi thread app

#define MAX_PACKET_SIZE 6000


#define SEND_1470_Byte_IN_UDP_in_nanosec 12300
#define EACH_VOLATILE_LOOP_in_nano_sec 0.3825
#define EACH_SECOND_in_nano_Sec 1000000000

#define AUTO_MODERATE_BUFFER_CAPACITY 10 /*simple array that must realloc x more room if size reach capacity*/


#define DEFAULT_BT_BUF_SZ 16 /*basic type size scale*/
#define DEFAULT_SFS_BUF_SZ 64 /*small formatted string size scale*/
#define DEFAULT_MFS_BUF_SZ 256 /*medium formatted string size scale*/
#define DEFAULT_LFS_BUF_SZ 1024 /*large formatted string size scale*/

typedef enum
{
	
	head_2_tail ,
	tail_2_head ,
	e_dir_default = head_2_tail
} e_direction;
