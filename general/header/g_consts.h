//#pragma once

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
	NEXT_GENERAL_ERROR_VALUE=-6,
	errSyntax,
	errCanceled,  // indeed this indicates a normal abort and is not an d_error condition
	errInvalidString,
	errMemoryLow,
	errGeneral,
	errOK,
	errSystem = 1, // this error is not public error value
};

//-------------------------------------------------------------------------
//extern uchar _ErrLvl; // does not dependable in multi thread app

#define MAX_PACKET_SIZE 2048
