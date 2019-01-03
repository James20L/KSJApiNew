#pragma once
#pragma execution_character_set("utf-8")

// 这个文件中添加一些windows下的数据类型定义名称在linux下对应的定义
// 比如windows下的HANDLE,linux没有此数据类型，需要在这个文件下进行定义。

// 注意一下，linux下的long数据类型是64位的，windows下的long数据类型是32位的。

#ifdef _WIN32
typedef  unsigned __int64 U64;
typedef  unsigned int     U32;
#elif defined(linux)
typedef  unsigned long    U64;
typedef  unsigned int     U32;
#endif

#ifdef linux

#ifdef __cplusplus
extern "C" {
#endif
#define ksjcamprint printf
typedef void                *HANDLE;
typedef void                *HMODULE;
typedef void                *HINSTANCE;
typedef void                *PVOID;
typedef void                *HWND;

typedef unsigned int        DWORD;     
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned char       UCHAR;
typedef unsigned char *     PUCHAR;
typedef unsigned short      WORD;
typedef long		    LONG;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL                *PBOOL;
typedef BOOL                *LPBOOL;
typedef BYTE                *PBYTE;
typedef BYTE                *LPBYTE;
typedef int                 *PINT;
typedef int                 *LPINT;
typedef WORD                *PWORD;
typedef WORD                *LPWORD;
typedef int                 *LPLONG;
typedef DWORD               *PDWORD;
typedef DWORD               *LPDWORD;
typedef void                *LPVOID;
typedef const void          *LPCVOID;
typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;
typedef unsigned short      USHORT;
typedef unsigned short      *PUSHORT;


#if defined(linux64)
typedef long          INT_PTR, *PINT_PTR;
typedef unsigned long UINT_PTR, *PUINT_PTR;

typedef long          LONG_PTR, *PLONG_PTR;
typedef unsigned long ULONG_PTR, *PULONG_PTR;
#else
typedef int           INT_PTR, *PINT_PTR;
typedef unsigned int  UINT_PTR, *PUINT_PTR;

typedef int           LONG_PTR, *PLONG_PTR;
typedef unsigned int  ULONG_PTR, *PULONG_PTR;
#endif


#ifndef _ERRNO_T_DEFINED
#define _ERRNO_T_DEFINED
typedef int errno_t;
#endif  /* _ERRNO_T_DEFINED */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// tchar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef char    CHAR;
typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character

typedef CHAR *NPSTR, *LPSTR, *PSTR;

#ifdef _UNICODE
#    ifndef _TCHAR_DEFINED
          typedef wchar_t TCHAR, *PTCHAR;
#    define _TCHAR_DEFINED
#    endif /* !_TCHAR_DEFINED */
#else  /* _UNICODE */
#    ifndef _TCHAR_DEFINED
          typedef char TCHAR, *PTCHAR;
#    define _TCHAR_DEFINED
#    endif /* !_TCHAR_DEFINED */
#endif

typedef TCHAR *LPTSTR;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// struct in windows
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _OVERLAPPED {
	ULONG_PTR Internal;
	ULONG_PTR InternalHigh;
	union {
		struct {
			DWORD Offset;
			DWORD OffsetHigh;
		} DUMMYSTRUCTNAME;
		PVOID Pointer;
	} DUMMYUNIONNAME;

	HANDLE  hEvent;
} OVERLAPPED, *LPOVERLAPPED;



typedef struct tagBITMAPFILEHEADER {
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
}  __attribute__((packed)) BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
	DWORD      biSize;
	int       biWidth;
	int       biHeight;
	WORD       biPlanes;
	WORD       biBitCount;
	DWORD      biCompression;
	DWORD      biSizeImage;
	int       biXPelsPerMeter;
	int       biYPelsPerMeter;
	DWORD      biClrUsed;
	DWORD      biClrImportant;
}  __attribute__((packed)) BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagRGBQUAD {
	BYTE    rgbBlue;
	BYTE    rgbGreen;
	BYTE    rgbRed;
	BYTE    rgbReserved;
} __attribute__((packed)) RGBQUAD;

typedef struct tagBITMAPINFO {
	BITMAPINFOHEADER    bmiHeader;
	RGBQUAD             bmiColors[1];
} BITMAPINFO, *LPBITMAPINFO, *PBITMAPINFO;

typedef struct _SYSTEMTIME {
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;





#ifdef __cplusplus
}
#endif

#endif
