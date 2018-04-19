#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <assert.h>
#include <iostream>
#include <sstream>
#include "KSJApi.h"
#include "cJSON.h"
#include <atomic>
//#include "memory_pool.h"  
//#include "ringq.h"  
//#include "melowLog.h"
//#include "fifomsg.h"  
//#include "Palm.h"
//#include "Finger.h"
//#include "max_curvature.h"
//#include "flycap.h"
//#include "image.h"
//#include "stack.h"
//#include "palmx.h"
//#include "BMP.h"


//配置参数结构定义
typedef struct ConFig_t
{
	float nExposureTime; //曝光时间单位ms
	int nExposureLines;//曝光行数
	int nGain; //增益
	int nTrigger; //触发模式 0 硬触发 1 软触发
	int nFlip; //0为不翻转 1为翻转
	int nMirror; //0为不镜像 1为镜像
	bool bIs3A; //是否设置3A模式
}CONFIG_T;




//相机对象
typedef struct CameraInfo
{
	int nIndex; //索引
    char nSerial;//相机序列号
    std::atomic<bool> bIsWorking;  //使用原子变量 bIsWorking; //是否工作
	int nWidth;//宽
    int nHeight;//高
	unsigned char * pBuffer;//图片buffer
	int nBitCount; //像素位数 黑白为8
	int nCameraType;
	CONFIG_T tConfig;
	pthread_t thread_id;//相机工作线程标识
	int nCount;
} T_CameraInfo;

T_CameraInfo g_Carmera[16] = {0}; //

//显示api错误信息
void ShowErrorInfo(char* szFunName, int nRet)
{
	TCHAR szErrorInfo[256] = { '\0' };
	KSJ_GetErrorInfo(nRet, szErrorInfo);
	printf("%s ShowErrorInfo %s\n",szFunName, szErrorInfo);
}


/*
功能:给每个相机创建4张图片的缓存	
*/
void KSJCreatBufs(int nCount,T_CameraInfo * pCameras)
{

    for(int nCamIndex = 0;nCamIndex < nCount;nCamIndex++)
    {
        int nSize = pCameras[nCamIndex].nWidth*pCameras[nCamIndex].nHeight*3;
        pCameras[nCamIndex].pBuffer = (unsigned char*)malloc(nSize);

    }

}

/*
功能:释放相机图片的缓存	
*/
void KSJ_ReleaseBufs(int number,T_CameraInfo * pCameras)
{
    for(int nCamIndex = 0;nCamIndex < number;nCamIndex++)
    {
        free(pCameras[nCamIndex].pBuffer);
    }

}

static char * g_pConfigBuffer = NULL;
void SetConfigBuffer(char *filename)
{
	FILE *fp;
	int flen;
	char *p;
	printf(" %s %s %d %s KSJCreatBufs\n",__FILE__,__FUNCTION__,__LINE__, filename);
	if ((fp = fopen(filename, "r")) == NULL)
	{
		printf("file open error\n");
		exit(0);
	}
	fseek(fp, 0, SEEK_END);
	flen = ftell(fp);
	g_pConfigBuffer = (char *)malloc(flen + 1);
	memset(g_pConfigBuffer, 0, flen + 1);
	fseek(fp, 0, SEEK_SET);
	fread(g_pConfigBuffer, flen, 1, fp);
	fclose(fp);
}

char *GetConfigBuffer()
{
	return g_pConfigBuffer;
}

void UnSetConfigBuffer()
{
	if(g_pConfigBuffer != NULL)
		free(g_pConfigBuffer);
	g_pConfigBuffer = NULL;
}


/*
功能:设置3A模式
参数：
nIndex 相机索引
bWork 是否工作3A模式
*/
void SetInterface3A(int nIndex, bool bWork)
{
	int nRet = 0; 
	if (nIndex == -1)    return;

	int nColStart;
	int nRowStart;
	int nColSize;
	int nRowSize;
	KSJ_ADDRESSMODE ColAddressMode;
	KSJ_ADDRESSMODE RowAddressMode;

	KSJ_CaptureGetDefaultFieldOfView(nIndex, &nColStart, &nRowStart, &nColSize, &nRowSize, &ColAddressMode, &RowAddressMode);

	int nSetColStart = (int)(nColStart + nColSize / 4);
	int nSetRowStart = (int)(nRowStart + nRowSize / 4);
	int nSetColSize  = nColSize / 2;
	int nSetRowSize  = nRowSize / 2;
	
	nRet = KSJ_AESetRegion(nIndex, nSetColStart, nSetRowStart, nSetColSize, nSetRowSize);
	ShowErrorInfo("KSJ_AESetRegion", nRet);
	bool bShow = true;
	KSJ_AEGetRegion(nIndex, &nColStart, &nRowStart, &nColSize, &nRowSize, &bShow);
	nRet = KSJ_AFSetRegion(nIndex, nSetColStart, nSetRowStart, nSetColSize, nSetRowSize);
	ShowErrorInfo("KSJ_AFSetRegion", nRet);	
	nRet = KSJ_AFStart(nIndex, bWork);
	ShowErrorInfo("KSJ_AFStart", nRet);	

}
/*
功能:从文件中读取配置参数
配置文件保存在sdcard的目录下
名称为config0.json, config1.json 分别对应
两个摄像头的配置文件
*/
int InitConfig(int nIndex, int nPusDeviceType)
{
	char filename[256] = {0};
	printf("nPusDeviceType = %d\n", nPusDeviceType);
	//sprintf(filename,"/sdcard/%d.json", nPusDeviceType);
	sprintf(filename,"%d.json", nPusDeviceType);
	SetConfigBuffer(filename);
	cJSON *jsonroot = cJSON_Parse(GetConfigBuffer());
	g_Carmera[nIndex].tConfig.nExposureLines = cJSON_GetObjectItem(jsonroot, "ExposureLines")->valuedouble;
	//g_Carmera[nIndex].tConfig.nExposureTime = cJSON_GetObjectItem(jsonroot, "ExposureTime")->valueint;
	g_Carmera[nIndex].tConfig.nGain = cJSON_GetObjectItem(jsonroot, "Gain")->valueint;
	g_Carmera[nIndex].tConfig.nTrigger = cJSON_GetObjectItem(jsonroot, "Trigger")->valueint;
	g_Carmera[nIndex].tConfig.bIs3A = (bool)cJSON_GetObjectItem(jsonroot, "Is3A")->valueint;
	g_Carmera[nIndex].tConfig.nFlip = (bool)cJSON_GetObjectItem(jsonroot, "Flip")->valueint;
	g_Carmera[nIndex].tConfig.nMirror = (bool)cJSON_GetObjectItem(jsonroot, "Mirror")->valueint;
	cJSON_Delete(jsonroot);
	UnSetConfigBuffer();
	return 1;
 }

/*
功能:设置摄像头参数
参数：
int camcount:相机总数
*/
static int KSJSetCamsParam(int camcount)
{
    int nRet = 0;

    int nColStart = 0;
    int nRowStart = 0;
    int ggx;
    int nColSize = 0;
    int ggsx;
    int nRowSize = 0;
    KSJ_ADDRESSMODE ColAddressMode;
    KSJ_ADDRESSMODE RowAddressMode;

	
    for(int nIndex = 0;nIndex<camcount;nIndex++)
    {

        unsigned short pusDeviceType;
        int pnSerials;
        unsigned short pusFirmwareVersion;
        unsigned short pusFpgaVersion;

		g_Carmera[nIndex].nIndex = nIndex;
        KSJ_DeviceGetInformationEx( nIndex,  &pusDeviceType,  &pnSerials,  &pusFirmwareVersion,  &pusFpgaVersion);
        printf(" =====================%s %s %d      nIndex = %d pusDeviceType = %d \n",__FILE__,__FUNCTION__,__LINE__, nIndex, pusDeviceType);
		g_Carmera[nIndex].nCameraType = pusDeviceType;
        printf(" =====================%s %s %d       pnSerials = %d \n",__FILE__,__FUNCTION__,__LINE__,pnSerials);
        int fmaj = pusFirmwareVersion>>8;
        int fmin = pusFirmwareVersion&0x00ff;


        int fpgamaj= pusFpgaVersion>>8;
        int fpgamin= pusFpgaVersion&0x00ff;


       
		//读取配置参数
		InitConfig(nIndex, pusDeviceType);

        KSJ_CaptureGetDefaultFieldOfView(nIndex,(int*)&nColStart,(int*)&nRowStart,(int *)&nColSize,(int *)&nRowSize,&ColAddressMode,&RowAddressMode);

        int width = nColSize;

        int height = nRowSize;

		g_Carmera[nIndex].nWidth = width;
		g_Carmera[nIndex].nHeight = height;

        KSJ_CaptureGetFieldOfView(nIndex,&nColStart,&nRowStart,&nColSize,&nRowSize,&ColAddressMode,&RowAddressMode);



        nRet = KSJ_CaptureSetFieldOfView(nIndex,0,0,width,height,KSJ_SKIPNONE,KSJ_SKIPNONE);
		printf(" %s %s %d      KSJ_CaptureSetFieldOfView is  %d\n",__FILE__,__FUNCTION__,__LINE__, nRet);
		
        KSJ_CaptureGetFieldOfView(nIndex,&nColStart,&nRowStart,&nColSize,&nRowSize,&ColAddressMode,&RowAddressMode);


        KSJ_BAYERMODE bayermode;

        KSJ_BayerGetMode(nIndex, &bayermode);

		//得到相机信息

		//KSJ_CaptureGetSizeEx(g_Carmera[nIndex].nIndex,&g_Carmera[nIndex].nWidth,&g_Carmera[nIndex].nHeight, &g_Carmera[nIndex].nBitCount);

        
		//去掉曝光时间设置
#if 0
        float nExposureTime = .0;
        KSJ_ExposureTimeGet(nIndex,&nExposureTime);

        printf(" %s %s %d  nExposureTime = %f \n",__FILE__,__FUNCTION__,__LINE__,nExposureTime);

        nRet = KSJ_ExposureTimeSet(nIndex,g_Carmera[nIndex].tConfig.nExposureTime);
		ShowErrorInfo("KSJ_ExposureTimeSet", nRet);

        nRet = KSJ_ExposureTimeGet(nIndex,&nExposureTime);
		ShowErrorInfo("KSJ_ExposureTimeGet", nRet);
#endif
		KSJ_FlashEnableSet(nIndex,true);

        int nlines = 0;

		
        nRet =KSJ_SetParam(nIndex,KSJ_EXPOSURE_LINES,g_Carmera[nIndex].tConfig.nExposureLines);
		//ShowErrorInfo("KSJ_EXPOSURE_LINES", nRet);
		printf(" %s %s %d      KSJ_EXPOSURE_LINES is  %d\n",__FILE__,__FUNCTION__,__LINE__, nRet);
		
        //KSJ_GetParam(nIndex,KSJ_EXPOSURE_LINES,&nlines);

        KSJ_WB_MODE wbmode;
        wbmode = KSJ_HWB_AUTO_CONITNUOUS;


        nRet = KSJ_WhiteBalanceSet(nIndex,wbmode);
		//ShowErrorInfo("KSJ_WhiteBalanceSet", nRet);
		
        //KSJ_WhiteBalanceGet(nIndex,&wbmode);


        KSJ_CaptureGetSize(nIndex,&g_Carmera[nIndex].nWidth,&g_Carmera[nIndex].nHeight);


		//设置增益
		
		KSJ_SetParam(nIndex, KSJ_RED, g_Carmera[nIndex].tConfig.nGain);
		KSJ_SetParam(nIndex, KSJ_GREEN, g_Carmera[nIndex].tConfig.nGain);
		KSJ_SetParam(nIndex, KSJ_BLUE, g_Carmera[nIndex].tConfig.nGain);
		

		int bValue;

#if 0
		KSJ_QueryFunction(nIndex, KSJ_SUPPORT_SNAP_BUTTON, &bValue);

		printf(" %s %s %d      KSJ_SUPPORT_SNAP_BUTTON is  %d\n",__FILE__,__FUNCTION__,__LINE__, bValue);

		KSJ_QueryFunction(nIndex, KSJ_SUPPORT_TRIGGER_MODE_EXTERNAL, &bValue);
		printf(" %s %s %d      KSJ_SUPPORT_TRIGGER_MODE_EXTERNA is  %d\n",__FILE__,__FUNCTION__,__LINE__, bValue);

		KSJ_QueryFunction(nIndex, KSJ_SUPPORT_TRIGGER_MODE_SOFTWARE, &bValue);
		printf(" %s %s %d      KSJ_SUPPORT_TRIGGER_MODE_SOFTWARE is  %d\n",__FILE__,__FUNCTION__,__LINE__, bValue);


		KSJ_QueryFunction(nIndex, KSJ_SUPPORT_EMPTY_FRAMEBUFFER, &bValue);
		printf(" %s %s %d      KSJ_SUPPORT_EMPTY_FRAMEBUFFER is  %d\n",__FILE__,__FUNCTION__,__LINE__, bValue);

		KSJ_QueryFunction(nIndex, KSJ_SUPPORT_GET_FRAME_BUFFER_STATE, &bValue);
		printf(" %s %s %d      KSJ_SUPPORT_GET_FRAME_BUFFER_STATE is  %d\n",__FILE__,__FUNCTION__,__LINE__, bValue);

		KSJ_QueryFunction(nIndex, KSJ_SUPPORT_GET_FRAME_BUFFER_STATE, &bValue);
		printf(" %s %s %d      KSJ_SUPPORT_GET_FRAME_BUFFER_STATE is  %d\n",__FILE__,__FUNCTION__,__LINE__, bValue);
#endif

		
		//设置镜像 翻转
		if(g_Carmera[nIndex].tConfig.nFlip == 1)
			KSJ_SetParam(nIndex, KSJ_FLIP, 1);
		else
			KSJ_SetParam(nIndex, KSJ_FLIP, 0);
#if 0
		if(g_Carmera[nIndex].tConfig.nMirror == 1)
			KSJ_SetParam(nIndex, KSJ_MIRROR, 1);
		else
			KSJ_SetParam(nIndex, KSJ_MIRROR, 0);
#endif

		
        if(1)
        {

            KSJ_WaterMarkSetMode(nIndex,KSJ_WMM_TIMESTAMP);

            KSJ_WaterMarkSetEnable(nIndex, true);
        }
		
		
		if(g_Carmera[nIndex].tConfig.nTrigger == 0) //硬触发模式 否则为软触发
		{
			
			
			//设置为阻塞的方式读取一帧
			unsigned int nTimeOut;
			nRet = KSJ_CaptureSetTimeOut(nIndex, 0xFFFFFFFF);
			printf(" %s %s %d      KSJ_CaptureSetTimeOut return  %d\n",__FILE__,__FUNCTION__,__LINE__, nRet );
			
			//设置上升沿触发
			KSJ_TRIGGERMETHOD nTriggerMethod;
		    nRet = KSJ_TriggerMethodSet(nIndex, KSJ_TRIGGER_RISINGEDGE);
			printf(" %s %s %d      KSJ_TriggerMethodSet return  %d\n",__FILE__,__FUNCTION__,__LINE__, nRet );
			if(nRet != 0)
			{
				return -1;
			}

			//nRet = KSJ_TriggerMethodGet(nIndex, &nTriggerMethod);
			//printf(" %s %s %d      KSJ_TriggerMethodGet return  %d\n",__FILE__,__FUNCTION__,__LINE__, nRet );
			//printf(" %s %s %d      KSJ_TriggerMethodGet is  %d\n",__FILE__,__FUNCTION__,__LINE__, nTriggerMethod );			
			
			//设置外部触发
			nRet = KSJ_TriggerModeSet(nIndex, KSJ_TRIGGER_EXTERNAL);

			
			printf(" %s %s %d      KSJ_TriggerModeSet return  %d\n",__FILE__,__FUNCTION__,__LINE__, nRet );
			if(nRet != 0)
			{
				return -1;
			}

			KSJ_TRIGGERMODE nTriggerMode;	
			nRet = KSJ_TriggerModeGet(nIndex, &nTriggerMode);
			printf(" %s %s %d      KSJ_TriggerModeGet return  %d\n",__FILE__,__FUNCTION__,__LINE__, nRet );
			printf(" %s %s %d      TriggerModeGet is  %d\n",__FILE__,__FUNCTION__,__LINE__, nTriggerMode );





	

		}

		//printf(" %s %s %d	   SetInterface3A is	%d\n",__FILE__,__FUNCTION__,__LINE__, g_Carmera[nIndex].tConfig.bIs3A );	
		//SetInterface3A(nIndex, g_Carmera[nIndex].tConfig.bIs3A);

		
		

    }
	return 0;


}

/*
功能:初始化康士佳摄像头

返回值：成功初始化话摄像头数量
*/
int KSJInitCapture() 
{
	int nCamCount = 0;
	int nRet = 0;
	nRet = KSJ_Init();
	usleep(100);
	//得到设备数量
	nCamCount = KSJ_DeviceGetCount();
	//设置设备参数

	while(KSJSetCamsParam(nCamCount) == -1);
	
	if(nCamCount<=0)
	{
		 return 0;
	}
	


	KSJCreatBufs(nCamCount, g_Carmera);
	return nCamCount;
	
}

/*
功能:设置某个相机工作
*/
void KSJSetCamsWork(int nIndex)
{	
	g_Carmera[nIndex].bIsWorking = true;
}

/*
功能:设置某个相机停止工作
*/
void KSJUnSetCamsWork(int nIndex)
{
	g_Carmera[nIndex].bIsWorking = false;
}

/*
功能:	注销相机
*/
void KSJUnInitCapture(int nCamCount) 
{
	KSJ_UnInit();
	KSJ_ReleaseBufs(nCamCount,g_Carmera);
}


/*
功能:相机循环工作线程
参数:T_CameraInfo* 相机对象
*/
static int my = 0;
void* KSJCaptureWork(void* loopargs) 
{

		//printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ KSJCaptureWork Create %d\n", my);
		//my++;
		T_CameraInfo * pCameraInfo = (T_CameraInfo *)loopargs;
	
		unsigned int nCount = 0;
		int nRet = 0;
		int nSize = pCameraInfo->nHeight* pCameraInfo->nWidth;
		unsigned short pwid = 0;
		unsigned long  TimestampLowPart = 0;
		unsigned long  TimestampHighPart = 0;
		while(pCameraInfo->bIsWorking)
		{
			/*
				这样定义的环形缓存区 现在 像素深度为8位 刚好一个字节 对于彩色需要修改 日后补充
				pCameraInfo->pBuffer[(pCameraInfo->nHeight * pCameraInfo->nWidth)*(nCount%4)]
			*/
			printf("---------------------------------pCAMERAINO = %d %d\n", pCameraInfo->nIndex, pCameraInfo->nCameraType);
			 unsigned char * pBuf = &pCameraInfo->pBuffer[(nSize)*(nCount%3)];	
			//注意nRet为0捕获数据才成功
			//int nRet = 0xff;
			
			nRet  =	KSJ_CaptureRawData(pCameraInfo->nIndex, pBuf);
			if(nRet == 0)
			{
				  
				pCameraInfo->nCount++;


				KSJ_WaterMarkGetInfo(pCameraInfo->nIndex, &pwid, &TimestampLowPart, &TimestampHighPart);

				printf("index = %d KSJ_WaterMarkGetInfo = %d KSJ_WaterMarkGetInfo = %ld KSJ_WaterMarkGetInfo = %ld \n",pCameraInfo->nIndex,pwid,TimestampLowPart,TimestampHighPart);


			}
			else
			{
				  printf("------------------------------Capture error!!!!!!!-----------------------\n");	 
			}
		
		usleep(100);	
		}	


}

/*
功能:相机工作测试函数
参数:T_CameraInfo* 相机对象
*/
//int testCaputreWork() 
int main() 
{
	int nCount = 0;
    int ret;
	void *status;
	
	nCount = KSJInitCapture();

	for (int i = 0; i < nCount ; i++)
	{

		KSJSetCamsWork(i); //设置摄像头开始工作
		ret = pthread_create(&g_Carmera[i].thread_id, NULL, &KSJCaptureWork, &g_Carmera[i]);
		usleep(200);
		
		if (ret != 0) 
		{
			return -1;
		}

		
	}

	/*
	默认创建的是分离线程 不需要join
	//等待一分钟捕捉
    for(int i = 0; i < nCount; i++)
    {
        ret = pthread_join(&g_Carmera[i].thread_id,&status);
		printf("pthread_join renturn value = %d\n", ret);

    }
    */
	int nSize0 = g_Carmera[0].nWidth * g_Carmera[0].nHeight;
	int nSize1 = g_Carmera[1].nWidth * g_Carmera[1].nHeight;
	int oldCount = 0;
	while(1)
	{
		if((nCount == 2) && ((g_Carmera[0].nCount % 3) == 0) && ((g_Carmera[1].nCount % 3) == 0) && g_Carmera[0].nCount != oldCount && g_Carmera[1].nCount != oldCount)
		{
			oldCount = g_Carmera[1].nCount;
			char strPath[256];
			sprintf(strPath, "/sdcard/%d-%d.bmp",g_Carmera[0].nCameraType,g_Carmera[0].nCount-2);	  
			KSJ_HelperSaveToBmp((g_Carmera[0].pBuffer + (nSize0 * 2)), g_Carmera[0].nWidth, g_Carmera[0].nHeight, 8, (const char*)strPath);
			
			sprintf(strPath, "/sdcard/%d-%d.bmp",g_Carmera[0].nCameraType,g_Carmera[0].nCount-1);	  
			KSJ_HelperSaveToBmp((g_Carmera[0].pBuffer + nSize0), g_Carmera[0].nWidth, g_Carmera[0].nHeight, 8, (const char*)strPath);
						
			sprintf(strPath, "/sdcard/%d-%d.bmp",g_Carmera[0].nCameraType,g_Carmera[0].nCount);	  
			KSJ_HelperSaveToBmp(g_Carmera[0].pBuffer, g_Carmera[0].nWidth, g_Carmera[0].nHeight, 8, (const char*)strPath);

						
			sprintf(strPath, "/sdcard/%d-%d.bmp",g_Carmera[1].nCameraType,g_Carmera[1].nCount-2);	  
			KSJ_HelperSaveToBmp(g_Carmera[1].pBuffer + (nSize1 * 2), g_Carmera[1].nWidth, g_Carmera[1].nHeight, 8, (const char*)strPath);
			
			sprintf(strPath, "/sdcard/%d-%d.bmp",g_Carmera[1].nCameraType,g_Carmera[1].nCount-1);	  
			KSJ_HelperSaveToBmp((g_Carmera[1].pBuffer + nSize1), g_Carmera[1].nWidth, g_Carmera[1].nHeight, 8, (const char*)strPath);
						
			sprintf(strPath, "/sdcard/%d-%d.bmp",g_Carmera[1].nCameraType,g_Carmera[1].nCount);	  
			KSJ_HelperSaveToBmp(g_Carmera[1].pBuffer, g_Carmera[1].nWidth, g_Carmera[1].nHeight, 8, (const char*)strPath);

		
			
			printf("------------------------------Capture0 nCount = %d  Capture1 nCount = %d!!!!!!!-----------------------\n", g_Carmera[0].nCount, g_Carmera[1].nCount);	
		}

	}
	
	KSJUnInitCapture(nCount);
	
	return 0;
}


