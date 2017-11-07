#ifndef KSJAPI_TRIGGERMODE_H__
#define KSJAPI_TRIGGERMODE_H__

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
extern "C" {
#endif

#include "cross_platform_def.h"
///////////////////////////////////////////////////////////////////////////////////////////
// Trigger Mode
enum KSJ_TRIGGERMODE
{
	KSJ_TRIGGER_INTERNAL,
	KSJ_TRIGGER_EXTERNAL,
	KSJ_TRIGGER_SOFTWARE,
	KSJ_TRIGGER_FIXFRAMERATE
};

// Set Trigger Mode
KSJ_API  KSJ_SetTriggerMode(IN int nIndex, IN  KSJ_TRIGGERMODE TriggerMode);
KSJ_API  KSJ_GetTriggerMode(IN int nIndex, OUT KSJ_TRIGGERMODE *pTriggerMode);

enum KSJ_TRIGGERSTATUS
{
	KSJ_NO,
	KSJ_HAVE
};

// Get Frame Buffer Status. Has One or more Frame or None. Maybe return RET_NOTSUPPORT.
// When Fpga Version is not 0.0, This Function should support.
KSJ_API  KSJ_TriggerStatusGet(int nIndex, KSJ_TRIGGERSTATUS *pTriggerStatus);

// Empty Frame Buffer. Not Support When In KSJ_TRIGGER_SOFTWARE Mode.
KSJ_API  KSJ_EmptyFrameBuffer(int nIndex);





#ifdef __cplusplus
}
#endif

#endif