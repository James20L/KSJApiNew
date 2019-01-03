#pragma once
#pragma execution_character_set("utf-8")

#ifdef linux

#ifdef __cplusplus
extern "C" {
#endif

#include "windatatype_linux.h"
#include <string.h>

// 这里是windows定义的一些常量，在linux下没有的，需要定义一下
#ifndef NULL
#    ifdef __cplusplus
#        define NULL    0
#    else
#        define NULL    ((void *)0)
#    endif
#endif

#ifndef FALSE
#    define FALSE               0
#endif

#ifndef TRUE
#    define TRUE                1
#endif

#ifndef IN
#    define IN
#endif

#ifndef OUT
#    define OUT
#endif

#ifndef OPTIONAL
#    define OPTIONAL
#endif

#define far
#define near

#define FAR                 far
#define NEAR                near
#define CONST               const

typedef struct tagPOINT
{
	int  x;
	int  y;
} POINT, *PPOINT,  *NPPOINT,  *LPPOINT;

#define __stdcall

#define CALLBACK    __stdcall
#define WINAPI      __stdcall

#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)
#endif

#define MAX_PATH              260

#define ERROR_SUCCESS         0
#define ERROR_FILE_EXISTS     -1
#define ERROR_ALREADY_EXISTS  -2

#define BI_RGB                0L

#ifdef _UNICODE
#    define _T(x)         L ## x
#    define TEXT(quote)   L##quote
#else  
#    define _T(x)         x
#    define TEXT(quote)   quote   
#endif

#define __max(a,b)  (((a) > (b)) ? (a) : (b))
#define __min(a,b)  (((a) < (b)) ? (a) : (b))

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _UNICODE
#    define _tcscpy         wcscpy  
#    define _tcslen         wcslen
#    define _tcscat         wcscat
#    define _tfopen_s       _wfopen_s
#    define _ftprintf       fwprintf
#    define _tsplitpath_s   _wsplitpath_s
#    define _vstprintf_s    vswprintf_s
#    define _stprintf_s     swprintf_s
#    define _tcschr         wcschr
#    define _tcsrchr        wcsrchr
#    define _vsntprintf_s   _vsnwprintf_s
#    define _tfopen         _wfopen
#else  /* _UNICODE */
#    define _tcscpy         strcpy  
#    define _tcslen         strlen
#    define _tcscat         strcat
#    define _tfopen_s       fopen_s
#    define _ftprintf       fprintf
#    define _tsplitpath_s   _splitpath_s
#    define _vstprintf_s    vsprintf_s
#    define _stprintf_s     sprintf_s
#    define _tcschr         strchr
#    define _tcsrchr        strrchr
#    define _vsntprintf_s   _vsnprintf_s
#    define _tfopen         fopen
#endif


#ifdef __cplusplus
}
#endif


#endif