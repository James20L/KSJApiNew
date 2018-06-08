#ifndef __KSJ_API_INTERNAL_H__
#define __KSJ_API_INTERNAL_H__

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

	// 发送控制命令，各字节符合USB协议
	// MSB( IoBlockEx.uOffset ) = SETUPDAT[3], LSB( IoBlockEx.uOffset ) =  SETUPDAT[2] 
	// MSB( IoBlockEx.uIndex )  = SETUPDAT[5], LSB( IoBlockEx.uIndex )  =  SETUPDAT[4] 
	// MSB( IoBlockEx.uLength ) = SETUPDAT[7], LSB( IoBlockEx.uLength ) =  SETUPDAT[6] 
	// wValue  = uOffset
	// wIndex  = uIndex
	// wLength = uLength
#pragma pack ( 1 )
	typedef struct _tagUSBCmd
	{
		unsigned char  bmRequest;
		unsigned char  bRequest;
		unsigned short wValue;
		unsigned short wIndex;
		unsigned short wLength;					// 表示发送或接受数据的长度
	}USBCMD, *PUSBCMD;
#pragma pack ( 8 )

	KSJ_API   KSJ_VendorCmd(int nIndex, PUSBCMD pUsbCmd, unsigned char *pBuf);
	
	// 参考“凯视佳工业相机内部API函数说明书”
	// 单次读写字节数必须不大于64字节
	KSJ_API  KSJ_WrSensorReg(int nIndex, unsigned char btRegAddress, unsigned short wValue);
	KSJ_API  KSJ_RdSensorReg(int nIndex, unsigned char btRegAddress, unsigned short *pwValue);
	KSJ_API  KSJ_WrSensorRegX(int nIndex, unsigned short wRegAdress, unsigned short wValue);
	KSJ_API  KSJ_RdSensorRegX(int nIndex, unsigned short wRegAdress, unsigned short *pwValue);
	KSJ_API  KSJ_FrameRestart(int nIndex); 	// 当曝光改变后无法立刻生效时，需调用此函数。
	KSJ_API  KSJ_WrFpgaReg(int nIndex, unsigned char btRegAddress, unsigned short wValue);
	KSJ_API  KSJ_RdFpgaReg(int nIndex, unsigned char btRegAddress, unsigned short *pwValue);
	KSJ_API  KSJ_ResetFpga(int nIndex, bool bReset);
	KSJ_API  KSJ_WrFx2Sfr(int nIndex, unsigned short uRegAdress, unsigned char btValue);
	KSJ_API  KSJ_RdFx2Sfr(int nIndex, unsigned short uRegAdress, unsigned char *pbtValue);
	KSJ_API  KSJ_WrFx2Reg(int nIndex, unsigned short uRegAdress, unsigned char btValue);
	KSJ_API  KSJ_RdFx2Reg(int nIndex, unsigned short uRegAdress, unsigned char *pbtValue);
	KSJ_API  KSJ_ResetFx2(int nIndex, bool bReset);
	KSJ_API  KSJ_WrAD9923A(int nIndex, unsigned short wRegAddress, unsigned long dwValue);
	KSJ_API  KSJ_WrTMP101(int nIndex, unsigned char btRegAddress, unsigned short wValue);
	KSJ_API  KSJ_RdTMP101(int nIndex, unsigned char btRegAddress, unsigned short *pwValue);

	KSJ_API  KSJ_ResetDevice(int nIndex);
	KSJ_API  KSJ_ReconnectDevice(int nIndex);
	KSJ_API  KSJ_ResetHost(int nIndex);
	KSJ_API  KSJ_SendPktEnd(int nIndex);
	KSJ_API  KSJ_CancelIo(int nIndex);

	// nIndex is the index of DeviceList in KSJApi internally, not driver symbol name index. It will open first device which not opened as DeviceList[nIndex]
	KSJ_API   KSJ_Open(int nIndex);
	// nIndex is the index of DeviceList in KSJApi internally
	KSJ_API   KSJ_Close(int nIndex);


#ifdef __cplusplus
}
#endif

#endif