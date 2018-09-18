#ifndef _KSJLOG_H__
#define _KSJLOG_H__

#ifdef _WIN32
#ifdef KSJLOG_EXPORTS                                             // ���붯̬���ӿ�ʱ����
#    pragma message("export dynamic ksjlog lib")
#    define KSJLOG_API __declspec(dllexport) int __stdcall    
#elif  defined KSJLOG_STATIC                                      // ���뾲̬���ӿ�ʱ����ʹ�þ�̬���ӿ�ʱ����
#    pragma message("export or import static ksjlog lib")
#    define KSJLOG_API int __stdcall
#else
#    pragma message("import dynamic ksjlog lib")
#    define KSJLOG_API __declspec(dllimport) int __stdcall           // Ĭ��ʹ�ö�̬���ӿ�
#endif

#elif defined (__GNUC__) && (__GNUC__ >= 4) && (defined (__linux__) || defined (__APPLE__))
#    define KSJLOG_API int
#else
#    error Unknown platform, file needs adaption
#endif

#define		KSJLOG_SUCCESS  0
#define		KSJLOG_FAIL		1

#ifdef __cplusplus
// extern "C"{
#else
#endif



#ifdef linux

typedef void *HANDLE;

#ifndef IN
#    define IN
#endif

#ifndef OUT
#    define OUT
#endif


#ifndef HMODULE
#    define HMODULE int
#endif

#define TCHAR char
#define DWORD short
#define  LONG int
#define TRUE 1
#define FALSE 0
#define _T(x) x

#define BOOL unsigned char


#endif




// bFile�����Ƿ�д�뵽�ļ�, �����д�뵽�ļ�, bPrintOut�����Ƿ񵯳���ʾ�Ի���
// ���bFile��bPrintOut��ΪTRUE,�����ȼ�bFile>bPrintOut,ֻд�ļ������ᵯ���Ի���
// ���bFile=FALSE,bPrintOut=FALSE,��ʹ��OutputDebugString���������
// szFullPathNameΪLog��־�ļ�������·����
KSJLOG_API KSJLOG_Init  (HANDLE *hLog, TCHAR* szFullPathName, BOOL bFile, BOOL bPrintOut );

// bFile�����Ƿ�д�뵽�ļ�, �����д�뵽�ļ�, bPrintOut�����Ƿ񵯳���ʾ�Ի���
// ���bFile=FALSE,bPrintOut=FALSE,��ʹ��OutputDebugString���������
// ���bFile��bPrintOut��ΪTRUE,�����ȼ�bFile>bPrintOut,ֻд�ļ������ᵯ���Ի���
// szLogFileNameΪLog��־�ļ��������szFolder=NULL,�ڵ�������ͬĿ¼���Զ�����һ������ΪKSJLog���ļ��У�������szLogFileName����־�ļ���
// szFolder���һ���ַ�����Ҫ'\'��Ӧ�����ļ��е�����·����
// ����ļ�������ͬ���ļ�������ļ�������������ӡ�
KSJLOG_API KSJLOG_InitEx(HANDLE *hLog, IN HMODULE hModule, TCHAR* szFolder, TCHAR* szLogFileName, BOOL bFile, BOOL bPrintOut);

KSJLOG_API KSJLOG_UnInit(HANDLE *hLog);

// ���������Ϣ, nDebugLevel>=0���������
KSJLOG_API KSJLOG_OutputError(HANDLE hLog, DWORD dwLastError, int nDebugLevel);

// ���������Ϣ, nDebugLevel>=0���������
KSJLOG_API KSJLOG_OutputLastError(HANDLE hLog, int nDebugLevel);

// nDebugLevelֻҪ���ڵ���UDBG_LEVEL=0���������־�����
// ���bFile = TRUE, ��д�ļ���
// ���bPrintOut = TRUE,�򵯳��Ի���
// ������OutputDebugString����ӡ
KSJLOG_API KSJLOG_OutputString(HANDLE hLog, const TCHAR szItem[], int nDebugLevel);

KSJLOG_API KSJLOG_OutputStringEx(HANDLE hLog, int nDebugLevel, TCHAR* lpszFormat, ...);

#ifdef __cplusplus
//}
#endif

#endif
