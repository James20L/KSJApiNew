#ifndef _WIN32_DEF_H___
#define _WIN32_DEF_H___



#ifdef _WIN32
#    include <Windows.h>
#    include <process.h>
#    include <shlobj.h>
#    include <string.h>
#    include <stdio.h>



//#ifdef _UNICODE
//#else
//#endif


typedef unsigned (WINAPI *PBEGINTHREADEX_FUNC)(LPVOID lpParam);
typedef unsigned *PBEGINTHREADEX_ID;

#endif

#ifdef linux

#include <string.h>
#include <math.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <dlfcn.h>

typedef void *HANDLE;


#define SHCreateDirectoryEx(a,b,c) CreateDir(b)


#define __max(a,b)  (((a) > (b)) ? (a) : (b))
#define __min(a,b)  (((a) < (b)) ? (a) : (b))


#define ERROR_SUCCESS  0

#define ERROR_FILE_EXISTS  -1

#define ERROR_ALREADY_EXISTS  -2


#ifndef __stdcall
#define __stdcall
#endif


#ifndef WINAPI
#define WINAPI
#endif

#ifndef OVERLAPPED
#define OVERLAPPED int
#endif

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE (HANDLE)(-1)
#endif




#ifndef IN
#    define IN
#endif

#ifndef OUT
#    define OUT
#endif


#ifndef HMODULE
#    define HMODULE int
#endif


#ifndef OPTIONAL
#    define OPTIONAL
#endif

#define PUCHAR unsigned char *
#define UCHAR unsigned char

#define USHORT unsigned short
#define ULONG unsigned long



typedef unsigned int       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef long				LONG;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL            *PBOOL;
typedef BOOL             *LPBOOL;
typedef BYTE            *PBYTE;
typedef BYTE             *LPBYTE;
typedef int             *PINT;
typedef int              *LPINT;
typedef WORD            *PWORD;
typedef WORD             *LPWORD;
typedef int             *LPLONG;
typedef DWORD           *PDWORD;
typedef DWORD           *LPDWORD;
typedef void             *LPVOID;
typedef const void       *LPCVOID;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
DECLARE_HANDLE(HWND);

//#define __T(x)      x

//#define _T(x)       __T(x)
#define _TEXT(x)    __T(x)

#ifdef  UNICODE 
#    ifndef _TCHAR_DEFINED
     typedef wchar_t TCHAR, *PTCHAR;
	 typedef wchar_t TBYTE , *PTBYTE ;
     #define _TCHAR_DEFINED
#    endif /* !_TCHAR_DEFINED */
#else   /* UNICODE */               // r_winnt

#    ifndef _TCHAR_DEFINED
     typedef char TCHAR, *PTCHAR;
     typedef unsigned char TBYTE, *PTBYTE;
     #define _TCHAR_DEFINED
#    endif /* !_TCHAR_DEFINED */
#endif /*UNICODE*/

#define BI_RGB        0L

	 typedef struct tagBITMAPFILEHEADER {
		 WORD    bfType;
		 DWORD   bfSize;
		 WORD    bfReserved1;
		 WORD    bfReserved2;
		 DWORD   bfOffBits;
	 } BITMAPFILEHEADER,  *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

	 typedef struct tagBITMAPINFOHEADER{
		 DWORD      biSize;
		 LONG       biWidth;
		 LONG       biHeight;
		 WORD       biPlanes;
		 WORD       biBitCount;
		 DWORD      biCompression;
		 DWORD      biSizeImage;
		 LONG       biXPelsPerMeter;
		 LONG       biYPelsPerMeter;
		 DWORD      biClrUsed;
		 DWORD      biClrImportant;
	 } BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

	 typedef struct tagRGBQUAD {
		 BYTE    rgbBlue;
		 BYTE    rgbGreen;
		 BYTE    rgbRed;
		 BYTE    rgbReserved;
	 } RGBQUAD;

	 typedef struct tagBITMAPINFO {
		 BITMAPINFOHEADER    bmiHeader;
		 RGBQUAD             bmiColors[1];
	 } BITMAPINFO, *LPBITMAPINFO, *PBITMAPINFO;


#ifdef ANDROID
#include<android/log.h>
#define TAG "catchbest-jni" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型

#define ksjcamprint LOGF
#else
#define ksjcamprint printf
#endif



#pragma pack(4)

	 typedef struct ksj_in_para
	 {
		 unsigned char  cmd_type;
		 unsigned char requesttype;
		 unsigned short value;
		 unsigned short index;
		 //sensor_reg_t data;
		 int  datasize;
		 void * data;
	 } ksj_in_para_t;

#pragma pack()
#define BOOL unsigned char

	 struct usb_device_id {
		 int vid;
		 int pid;

	 };

#endif /*linux*/

#  ifdef _UNICODE
#    define tstring         wstring
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
#    define _tfopen     _wfopen

#    define _T(x)   L##x
#  else
#    define tstring         string
#    define _tcscpy         strcpy  
#    define _tcslen         strlen
#    define _tcscat         strcat
#    define _tfopen_s       fopen_s
#    define _ftprintf       fprintf
#    define _tsplitpath_s   _splitpath_s
#    define _vstprintf_s    vsprintf_s
#    define _stprintf_s     sprintf_s
#    define _T(x)   x
#    define _tcschr         strchr
#    define _tcsrchr        strrchr
#    define _vsntprintf_s   _vsnprintf_s
#    define _tfopen         fopen

#  endif


#endif
