#pragma once

//-------------------------------------------------------------------------
_EXTERN const char EOS; // 0
//const WCHAR WEOS = _WT('\0'); // 0
_EXTERN const char LF ; // line feed 10
_EXTERN const char CR ; // carriage return 13

_EXTERN const char _newLineFeed[] ;

_EXTERN const char _fileNameDelimiter[] ;

_EXTERN const char _SPFD ; // Standard path file delimiter

_EXTERN const char _UTF8Sign[] ;
_EXTERN const char _UTF16LSign[] ; // little-endian . Unicode in Microsoft terminology
_EXTERN const char _UTF16BSign[] ; // big-endian . Unicode in Microsoft terminology
_EXTERN const char _UTF32LSign[] ; // little-endian
_EXTERN const char _UTF32BSign[] ; // big-endian

_EXTERN const sockfd invalid_fd;

#define PRECISION_OF_DOUBLE 1e-10

//-------------------------------------------------------------------------

#define ERR_NEXT_VALUE() -(__COUNTER__ - __first_err_const__) // donot shuffle list just add at the end
enum
{
	errSystem = 1, // this error is not public error value
	errOK = 0,
	__first_err_const__ = __COUNTER__ , /*donot use this item*/

	errGeneral = ERR_NEXT_VALUE() ,
	errMemoryLow = ERR_NEXT_VALUE(),
	errInvalidString = ERR_NEXT_VALUE(),
	errCanceled = ERR_NEXT_VALUE(),  // indeed this indicates a normal abort and is not an d_error condition
	errSyntax = ERR_NEXT_VALUE(),
	errArg = ERR_NEXT_VALUE(), // origin -> bad argument
	errTimeout = ERR_NEXT_VALUE(),
	errPeerClosed = ERR_NEXT_VALUE(), // closed state is bold
	errOutofRanje = ERR_NEXT_VALUE(),
	errMaximumExceeded = ERR_NEXT_VALUE(), /*10*/
	errNoPeer = ERR_NEXT_VALUE(), // origin -> tcp peer state unknown or callback peer no respond
	errNotFound = ERR_NEXT_VALUE(),
	errDevice = ERR_NEXT_VALUE(),
	errSocket = ERR_NEXT_VALUE(),
	errCreation = ERR_NEXT_VALUE(),
	one_time_echo_err = ERR_NEXT_VALUE(), // origin -> this err just use for condition where at app level one M_V_END_RET show err msg and outer caller should ignore it
	errOverflow = ERR_NEXT_VALUE(), // if cannot fit and grow not allowed
	errCorrupted = ERR_NEXT_VALUE(),
	errResource = ERR_NEXT_VALUE(),
	errPath = ERR_NEXT_VALUE(), /*20*/
	errRetry = ERR_NEXT_VALUE(),
	errEmpty = ERR_NEXT_VALUE(),
	errTooManyAttempt = ERR_NEXT_VALUE(),
	errShutdown = ERR_NEXT_VALUE(),
	errBind = ERR_NEXT_VALUE(),
	errsockopt = ERR_NEXT_VALUE(),
	errListen = ERR_NEXT_VALUE(),
	errSelect = ERR_NEXT_VALUE(),
	errAccept = ERR_NEXT_VALUE(),
	errNoConnection = ERR_NEXT_VALUE(),
	errConnect = ERR_NEXT_VALUE(),
	errOpen = ERR_NEXT_VALUE(),
	errACK = ERR_NEXT_VALUE(), /*no ack in time window*/
	errNoCountinue = ERR_NEXT_VALUE(), /*just use for its name purpose*/
	errMapped = ERR_NEXT_VALUE(), /*just use for its name purpose*/
	errDoneAlready = ERR_NEXT_VALUE(),
	errButContinue = ERR_NEXT_VALUE(),

	// ADD extra error here

	NEXT_GENERAL_ERROR_VALUE = ERR_NEXT_VALUE(),
	_internal_err_count = -NEXT_GENERAL_ERROR_VALUE,
};

//-------------------------------------------------------------------------
//_EXTERN uchar _ErrLvl; // does not dependable in multi thread app

#define ETHERNET_MTU 1500
#define MAX_NFUDP_PAYLOAD 1472
#define MAX_UDP_PAYLOAD 65527
#define MAX_PAYLOAD_PER_FRAGMENT 1480

#define SIZE_ETHERNET 14 // Ethernet header is always 14 bytes (for non-VLAN frames)

#define UDP_LAYER_HDR_SZ_B 8

#define MAX_PACKET_SIZE 10000 /*i see 6300 byte udp so far*/


#define SEND_1470_Byte_IN_UDP_in_nanosec 12300
#define EACH_VOLATILE_LOOP_in_nano_sec 0.3825
#define EACH_SECOND_in_nano_Sec 1000000000

#define AUTO_MODERATE_BUFFER_CAPACITY 10 /*simple array that must realloc x more room if size reach capacity*/

#define ACK_TIMEOUT_ms  100 // Local network (same datacenter): 50–100 ms
#define SEND_TIMEOUT_ms 5
#define RETRY_MECHANISM 3

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

_EXTERN const unsigned char MSB_MARKERS[8];

#define GUARD_HEAD 0xABCDEF0123456789ULL
#define GUARD_TAIL 0x9876543210FEDCBAULL

// --------------------

#define DEFAULT_BAD_NETWORK_HANDSHAKE_TIMEOUT 10 /*sec*/

//-------------------------------------------------------------------------
#define G_SCHAR_MIN  ((schar)0x80)				// -128
#define G_SCHAR_MAX  ((schar)-1^G_SCHAR_MIN)	// 127
#define G_UCHAR_MIN  ((uchar)0x00)				// 0
#define G_UCHAR_MAX  ((uchar)-1^G_UCHAR_MIN)	// 255

#define G_SSHORT_MIN ((sshort)0x8000)			// -32768
#define G_SSHORT_MAX ((sshort)-1^G_SSHORT_MIN)	// 32767
#define G_USHORT_MIN ((ushort)0x0000)			// 0
#define G_USHORT_MAX ((ushort)-1^G_USHORT_MIN)	// 65535
#define G_USHORT_COUNT 65536

#define G_SLONG_MIN  ((slong)0x80000000L)		// -2,147,483,648
#define G_SLONG_MAX  ((slong)-1^G_SLONG_MIN)	// 2,147,483,647
#define G_ULONG_MIN  ((ulong)0x00000000L)		// 0
#define G_ULONG_MAX  ((ulong)-1^G_ULONG_MIN)	// 4,294,967,295

#define G_SLONGLONG_MIN  ((sint64)0x8000000000000000i64)	// -9,223,372,036,854,775,808
#define G_SLONGLONG_MAX  ((sint64)-1^G_SLONGLONG_MIN)		// 9,223,372,036,854,775,807
#define G_ULONGLONG_MIN  ((uint64)0x0000000000000000i64)	// 0
#define G_ULONGLONG_MAX  ((uint64)-1^G_ULONGLONG_MIN)		// 18,446,744,073,709,551,615

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef M_2_PI
#define M_2_PI		6.283185307179586476925286766559
#endif
#define PI_2		1.57079632679489662
#define PI_180		0.01745329251994329
#define M_3_PI_2	4.71238898038468985

#define PHI			1.618

#define MIN_SYSERR_BUF_SZ 256 /*at least this size should be prodided to store system error strings*/
#define MIN_SYSERR_BUF_CAPACITY ( MIN_SYSERR_BUF_SZ - 1 )

#define LAST_VALID_FP_NOT_NOISY_DIGIT_OF_DOUBLE 14 /*according to GPT*/
