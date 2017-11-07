#ifndef __KSJ_API_3A_H__
#define __KSJ_API_3A_H__

#if defined (_WIN32)
#  ifdef KSJAPI_EXPORTS
#    define KSJAPI_IMPORT_EXPORT __declspec(dllexport)
#  else
#    define KSJAPI_IMPORT_EXPORT __declspec(dllimport)
#  endif

#  if defined (_M_IX86) || defined (__i386__)
#    define KSJAPI_CALLTYPE __stdcall
#  else
#    define KSJAPI_CALLTYPE  
#  endif

#elif defined (__GNUC__) && (__GNUC__ >= 4) && (defined (__linux__) || defined (__APPLE__))
#  define KSJAPI_IMPORT_EXPORT __attribute__((visibility("default")))
#  if defined (__i386__)
#    define KSJAPI_CALLTYPE __attribute__((stdcall))
#  else
#    define KSJAPI_CALLTYPE /* default */
#  endif
#  ifndef EXTERN_C
#    define EXTERN_C extern "C"
#  endif

#else
#  error Unknown platform, file needs adaption
#endif


#ifndef _WIN32
#define KSJ_API int
#else
#define KSJ_API KSJAPI_IMPORT_EXPORT int KSJAPI_CALLTYPE
#endif

#ifdef __cplusplus
extern "C"{
#endif

#include "cross_platform_def.h"

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 设置自动白平衡区域，这个区域是相对预览图像的左上角像素位置
	KSJ_API  KSJ_AWBSetRegion(int nIndex, int nX, int nY, int nW, int nH);
	// 设置自动白平衡区域，这个区域是相对预览图像的左上角像素位置,是否显示区域
	KSJ_API  KSJ_AWBShowRegion(int nIndex, bool bShow);
	KSJ_API  KSJ_AWBGetRegion(int nIndex, int *pnX, int *pnY, int *pnW, int *pnH, bool *pbShow);



#ifdef __cplusplus
}
#endif

#endif