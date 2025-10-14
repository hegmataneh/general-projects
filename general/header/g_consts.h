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
	NEXT_GENERAL_ERROR_VALUE = -25,
	errShutdown = -24,
	errTooManyAttempt = -23,
	errEmpty = -22,
	errRetry = -21,
	errPath = -20,
	errResource = -19,
	errCorrupted = -18,
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

#define ETHERNET_MTU 1500
#define MAX_NFUDP_PAYLOAD 1472
#define MAX_UDP_PAYLOAD 65527
#define MAX_PAYLOAD_PER_FRAGMENT 1480

#define SIZE_ETHERNET 14 // Ethernet header is always 14 bytes (for non-VLAN frames)

#define UDP_LAYER_HDR_SZ 8

#define MAX_PACKET_SIZE 2000


#define SEND_1470_Byte_IN_UDP_in_nanosec 12300
#define EACH_VOLATILE_LOOP_in_nano_sec 0.3825
#define EACH_SECOND_in_nano_Sec 1000000000

#define AUTO_MODERATE_BUFFER_CAPACITY 10 /*simple array that must realloc x more room if size reach capacity*/


#define DEFAULT_BT_BUF_SZ 16 /*basic type size scale*/
#define DEFAULT_SFS_BUF_SZ 64 /*small formatted string size scale*/
#define DEFAULT_MFS_BUF_SZ 256 /*medium formatted string size scale*/
#define DEFAULT_LFS_BUF_SZ 1024 /*large formatted string size scale*/

#define INVALID_FD (-1)

#define CALLSTACK_LOG_LENGH 128

typedef enum
{
	
	head_2_tail ,
	tail_2_head ,
	e_dir_default = head_2_tail
} e_direction;

// --------------------

/*
High entropy( not all 0s or all 1s )
Not printable ASCII
Not common control codes( 0x00–0x1F )
Not typical alignment / filler( 0xFF )
mutual Hamming distance greater than 4
| Marker | Hex  | Binary     | Bit Count |
| M0     | 0x3C | `00111100` | 4         |
| M1     | 0xC3 | `11000011` | 4         |
| M2     | 0x5A | `01011010` | used indbg|
| M3     | 0xA5 | `10100101` | 4         |
| M4     | 0x96 | `10010110` | 4         |
| M5     | 0x69 | `01101001` | 4         |
| M6     | 0xF0 | `11110000` | 4         |
| M7     | 0x0F | `00001111` | 4         |
maximally separated and rare bytes
*/

extern const unsigned char MSB_MARKERS[8];

// --------------------

