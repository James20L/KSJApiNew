#include <stdio.h>
#include "stdlib.h"
#include <pthread.h>
#include <sys/types.h>

#ifdef USING_OPENCV
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/contrib/contrib.hpp"
using namespace cv;
#endif

#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#include "KSJApi.h"
using namespace std;

int    TimeSubstract(struct timeval *result, struct timeval *begin,struct timeval *end);


int RGBFLAG = 0;
int SHOWFLAG = 0;
int PICNUM = 0;
int CAPTURETWOSTEPFLAG = 0;
int MAP = 0;




int KSJ_Check_Ret_Warn(int nRet)
{
    if(nRet!=RET_SUCCESS)
    {
        printf("warn nRet = %d \n",nRet);

        return -1;
    }
    return 0;
}


int KSJ_Check_Ret_Fatal(int nRet)
{
    if(nRet!=RET_SUCCESS)
    {
        printf("fatal nRet = %d \n",nRet);

        exit(-1);

    }
    return 0;
}
int CHKRET = 0;

#define CHECK_RET(X)  do { \
    CHKRET=X; \
    if(CHKRET!=RET_SUCCESS) printf("%s  error code= %d \n",#X,CHKRET); \
    }while(0)



int KSJ_CaptureDataTwoStep(int index,unsigned char*buf)
{
    int nRet = RET_SUCCESS;

    nRet = KSJ_SoftStartCapture(index);

    printf(" 0 %s  %s %d    nRet =  %d  KSJ_SoftStartCapture \n",__FILE__,__FUNCTION__,__LINE__,nRet);

    nRet=KSJ_ReadRgbDataAfterStart(index,buf);


    printf(" 0 %s  %s %d    nRet =  %d  KSJ_ReadRgbDataAfterStart  \n",__FILE__,__FUNCTION__,__LINE__,nRet);
}



typedef struct Thread_Param
{
    int index;
    char serial;
    unsigned char* buf;
    int *stopflag;

} T_Thread_Param;


typedef struct Fov_param
{
    int nWidth;
    int nHeight;

} T_Fov_param;



pthread_t t_id[16];

int nCamCount = 0;
unsigned char * g_buf[16] = {0};

T_Fov_param g_fov[16];

T_Thread_Param paramarray[16];

int g_stopflag[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1};


int KSJ_CreatBufs(int number,T_Fov_param * fovs)
{

    for(int nCamIndex = 0;nCamIndex < number;nCamIndex++)
    {
        int nCount = fovs[nCamIndex].nWidth*fovs[nCamIndex].nHeight*4;

        g_buf[nCamIndex] = (unsigned char*)malloc(nCount);

    }

}


int KSJ_ReleaseBufs(int number,T_Fov_param * fovs)
{

    for(int nCamIndex = 0;nCamIndex < number;nCamIndex++)
    {

        free(g_buf[nCamIndex]);

    }

}

int KSJ_SetCamsParam_jd(int CAMERA_NUMBER)
{
    int nRet = 0;
    int nColStart = 0;
    int nRowStart = 0;
    int ggx;
    int nColSize = 0;
    int nRowSize = 0;
    KSJ_ADDRESSMODE ColAddressMode;
    KSJ_ADDRESSMODE RowAddressMode;

    for(int nIndex = 0;nIndex<CAMERA_NUMBER;nIndex++)
    {
        KSJ_CaptureGetDefaultFieldOfView(nIndex,(int*)&nColStart,(int*)&nRowStart,(int *)&nColSize,(int *)&nRowSize,&ColAddressMode,&RowAddressMode);

        printf(" %s %s %dnColStart = %d \n ",__FILE__,__FUNCTION__,__LINE__,nColStart);
        printf(" %s %s %dnRowStart = %d \n ",__FILE__,__FUNCTION__,__LINE__,nRowStart);
        printf(" %s %s %dnColSize = %d \n ",__FILE__,__FUNCTION__,__LINE__,nColSize);
        printf(" %s %s %dnRowSize = %d \n ",__FILE__,__FUNCTION__,__LINE__,nRowSize);
        int width = nColSize;
        int height = nRowSize;
        width =3096;//1280;
        height = 2080;//960;
        //      KSJ_CaptureSetFieldOfView(nIndex,0,0,width,height,KSJ_SKIP2,KSJ_SKIP2);
        //        KSJ_BayerSetMode(nIndex, KSJ_GRBG_BGR24_FLIP);

        int nlines = 0;
        //        KSJ_SetParam(nIndex,KSJ_EXPOSURE_LINES,1000);
        //        KSJ_GetParam(nIndex,KSJ_EXPOSURE_LINES,&nlines);
        float nExposureTime = .0;
        nExposureTime = 20.0;
        printf(" %s %s %d       nExposureTime = %f \n ",__FILE__,__FUNCTION__,__LINE__,nExposureTime);
        KSJ_ExposureTimeSet(nIndex,nExposureTime);
        KSJ_WB_MODE wbmode;
        wbmode = KSJ_HWB_AUTO_CONITNUOUS;  //KSJ_HWB_PRESETTINGS,
        KSJ_WhiteBalanceSet(nIndex,wbmode);
        //        KSJ_WhiteBalancePresettingSet(nIndex,KSJ_CT_5000K)
        KSJ_WhiteBalanceGet(nIndex,&wbmode);
        KSJ_SetParam(nIndex,KSJ_RED,100);
        KSJ_SetParam(nIndex,KSJ_GREEN,100);
        KSJ_SetParam(nIndex,KSJ_BLUE,100);
        printf(" KSJ_WhiteBalanceGet =====================%s %s %d       nlines = %d \n ",__FILE__,__FUNCTION__,__LINE__,wbmode);
#if 0
        KSJ_SENSITIVITYMODE  smode  =  KSJ_HIGH;
        KSJ_SensitivitySetMode(nIndex,smode);
#endif
        KSJ_ColorCorrectionSet(0, KSJ_HCCM_PRESETTINGS);

        KSJ_ColorCorrectionPresettingSet(0, KSJ_CT_5000K);


    }
    return 0;
}


int KSJ_SetCamsParam(int camcount)
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


        CHECK_RET(KSJ_DeviceGetInformationEx( nIndex,  &pusDeviceType,  &pnSerials,  &pusFirmwareVersion,  &pusFpgaVersion));

        printf(" =====================%s %s %d       pusDeviceType = %d \n",__FILE__,__FUNCTION__,__LINE__,pusDeviceType);
        printf(" =====================%s %s %d       pnSerials = %d \n",__FILE__,__FUNCTION__,__LINE__,pnSerials);
        int fmaj = pusFirmwareVersion>>8;
        int fmin = pusFirmwareVersion&0x00ff;
        printf(" =====================%s %s %d       pusFirmwareVersion  fmaj = %d    fmin = %d \n",__FILE__,__FUNCTION__,__LINE__,fmaj,fmin);

        int fpgamaj= pusFpgaVersion>>8;
        int fpgamin= pusFpgaVersion&0x00ff;

        printf(" =====================%s %s %d       pusFpgaVersion  fpgamaj = %d    fpgamin = %d \n",__FILE__,__FUNCTION__,__LINE__,fpgamaj,fpgamin);







        CHECK_RET(KSJ_CaptureGetDefaultFieldOfView(nIndex,(int*)&nColStart,(int*)&nRowStart,(int *)&nColSize,(int *)&nRowSize,&ColAddressMode,&RowAddressMode));

        //#define printf  //
        printf(" =====================%s %s %d       nColStart = %d \n",__FILE__,__FUNCTION__,__LINE__,nColStart);
        printf(" =====================%s %s %d       nRowStart = %d \n",__FILE__,__FUNCTION__,__LINE__,nRowStart);
        printf(" =====================%s %s %d       nColSize = %d \n",__FILE__,__FUNCTION__,__LINE__,nColSize);
        printf(" =====================%s %s %d       nRowSize = %d \n",__FILE__,__FUNCTION__,__LINE__,nRowSize);

        int width = nColSize;

        int height = nRowSize;

        KSJ_CaptureGetFieldOfView(nIndex,&nColStart,&nRowStart,&nColSize,&nRowSize,&ColAddressMode,&RowAddressMode);


        printf(" =====================%s %s %d       nColStart = %d \n",__FILE__,__FUNCTION__,__LINE__,nColStart);
        printf(" =====================%s %s %d       nRowStart = %d \n",__FILE__,__FUNCTION__,__LINE__,nRowStart);
        printf(" =====================%s %s %d       nColSize = %d \n",__FILE__,__FUNCTION__,__LINE__,nColSize);
        printf(" =====================%s %s %d       nRowSize = %d \n",__FILE__,__FUNCTION__,__LINE__,nRowSize);






        //        width =1280;
        //        height = 1024;
#if 1

        KSJ_CaptureSetFieldOfView(nIndex,0,0,width,height,KSJ_SKIPNONE,KSJ_SKIPNONE);
        //          KSJ_CaptureSetFieldOfView(nIndex,0,0,width,height,KSJ_SKIPNONE,KSJ_SKIPNONE);

        KSJ_CaptureGetFieldOfView(nIndex,&nColStart,&nRowStart,&nColSize,&nRowSize,&ColAddressMode,&RowAddressMode);


        printf(" =====================%s %s %d       nColStart = %d \n",__FILE__,__FUNCTION__,__LINE__,nColStart);
        printf(" =====================%s %s %d       nRowStart = %d \n",__FILE__,__FUNCTION__,__LINE__,nRowStart);
        printf(" =====================%s %s %d       nColSize = %d \n",__FILE__,__FUNCTION__,__LINE__,nColSize);
        printf(" =====================%s %s %d       nRowSize = %d \n",__FILE__,__FUNCTION__,__LINE__,nRowSize);

        //        CHECK_RET(KSJ_LutSetEnable(nIndex, false));

        //        KSJ_FILTERMODE bayermode;
        //        KSJ_FilterGetMode(nIndex,&bayermode);

        KSJ_BAYERMODE bayermode;

        CHECK_RET(KSJ_BayerGetMode(nIndex, &bayermode));
        printf(" =====================%s %s %d       bayermode = %d \n",__FILE__,__FUNCTION__,__LINE__,bayermode);




        //        CHECK_RET(KSJ_BayerSetMode(nIndex, KSJ_BGGR_BGR24_FLIP));

#if 1
        float nExposureTime = .0;
        KSJ_ExposureTimeGet(nIndex,&nExposureTime);

        printf(" %s %s %d       nExposureTime = %f \n",__FILE__,__FUNCTION__,__LINE__,nExposureTime);

        nExposureTime = 30;
        printf(" %s %s %d       nExposureTime = %f \n",__FILE__,__FUNCTION__,__LINE__,nExposureTime);

        KSJ_ExposureTimeSet(nIndex,nExposureTime);


        KSJ_ExposureTimeGet(nIndex,&nExposureTime);

        printf(" %s %s %d       nExposureTime = %f \n",__FILE__,__FUNCTION__,__LINE__,nExposureTime);






#endif
#if 1
        int nlines = 0;

        CHECK_RET(KSJ_SetParam(nIndex,KSJ_EXPOSURE_LINES,200));


        CHECK_RET(KSJ_GetParam(nIndex,KSJ_EXPOSURE_LINES,&nlines));

        printf(" KSJ_GetParam =====================%s %s %d       nlines = %d \n",__FILE__,__FUNCTION__,__LINE__,nlines);

        KSJ_WB_MODE wbmode;
        wbmode = KSJ_HWB_AUTO_CONITNUOUS;


        KSJ_WhiteBalanceSet(nIndex,wbmode);

        KSJ_WhiteBalanceGet(nIndex,&wbmode);
        printf(" KSJ_WhiteBalanceGet =====================%s %s %d       nlines = %d \n",__FILE__,__FUNCTION__,__LINE__,wbmode);


#endif

        CHECK_RET(KSJ_CaptureGetSize(nIndex,&g_fov[nIndex].nWidth,&g_fov[nIndex].nHeight));
        printf(" %s %s %d      FOVS[%d].nWidth %d\n",__FILE__,__FUNCTION__,__LINE__,nIndex,g_fov[nIndex].nWidth);
        printf(" %s %s %d      FOVS[%d].nHeight %d\n",__FILE__,__FUNCTION__,__LINE__,nIndex,g_fov[nIndex].nHeight);


    if(MAP == 1){

        int nRet = KSJ_CaptureSetCalibration(nIndex,true);
        bool bCali = false;
        nRet = KSJ_CaptureGetCalibration(nIndex,&bCali);
        printf(" %s %s %d   bCali =   %d    KSJ_CaptureGetCalibration  \n",__FILE__,__FUNCTION__,__LINE__,bCali);

        nRet = KSJ_LoadCalibrationMapFile(nIndex,"Map.cmf");
        printf(" %s %s %d   nRet =   %d    KSJ_LoadCalibrationMapFile  \n",__FILE__,__FUNCTION__,__LINE__,nRet);


        nRet = KSJ_SetCalibrationMapMode(nIndex,KSJ_MM_NEARESTNEIGHBOR);

        printf(" %s %s %d   nRet =   %d    KSJ_SetCalibrationMapMode  \n",__FILE__,__FUNCTION__,__LINE__,nRet);


        }





#if 0
        KSJ_CCM_MODE ccs_mode;
        CHECK_RET(KSJ_ColorCorrectionSet(0,KSJ_HCCM_PRESETTINGS));
        CHECK_RET(KSJ_ColorCorrectionGet(0,&ccs_mode));

        KSJ_TRIGGERMODE mode = KSJ_TRIGGER_SOFTWARE;

        printf(" %s %s %d     %d   KSJ_SetTriggerMode  KSJ_TRIGGER_SOFTWARE  \n",__FILE__,__FUNCTION__,__LINE__,mode);
        CHECK_RET(KSJ_TriggerModeSet(0,mode));

        printf(" %s %s %d     %d   KSJ_SetTriggerMode \n",__FILE__,__FUNCTION__,__LINE__,CHKRET);
        nRet = KSJ_TriggerModeGet(0,&mode);

        printf(" %s %s %d     %d   KSJ_GetTriggerMode nRet  \n",__FILE__,__FUNCTION__,__LINE__,nRet);

        printf(" %s %s %d     %d   KSJ_GetTriggerMode  mode \n",__FILE__,__FUNCTION__,__LINE__,mode);

        mode = KSJ_TRIGGER_INTERNAL;

        printf(" %s %s %d     %d   KSJ_SetTriggerMode  KSJ_TRIGGER_EXTERNAL  \n",__FILE__,__FUNCTION__,__LINE__,mode);
        CHECK_RET(KSJ_TriggerModeSet(0,mode));

        printf(" %s %s %d     %d   KSJ_SetTriggerMode \n",__FILE__,__FUNCTION__,__LINE__,CHKRET);
        CHECK_RET(KSJ_TriggerModeGet(0,&mode));

        printf(" %s %s %d     %d   KSJ_GetTriggerMode nRet  \n",__FILE__,__FUNCTION__,__LINE__,CHKRET);

        printf(" %s %s %d     %d   KSJ_GetTriggerMode  mode \n",__FILE__,__FUNCTION__,__LINE__,mode);
#endif

#endif

#define printf printf

    }



}

void CreatSampleImage(unsigned char * pBuf,int nWidth,int nHeight,int nBitCounts)
{
    printf(" %s %s %d    CreatSampleImage     nWidth= %d   \n",__FILE__,__FUNCTION__,__LINE__,nWidth);
    printf(" %s %s %d    CreatSampleImage     nHeight= %d   \n",__FILE__,__FUNCTION__,__LINE__,nHeight);
    printf(" %s %s %d    CreatSampleImage     nBitCounts= %d   \n",__FILE__,__FUNCTION__,__LINE__,nBitCounts);

    int nByteCounts = nBitCounts >>3;
    printf(" %s %s %d    CreatSampleImage     nByteCounts= %d   \n",__FILE__,__FUNCTION__,__LINE__,nByteCounts);


    for(int rowindex = 0;rowindex < nHeight; rowindex++)
    {
        for(int colindex = 0;colindex <nWidth; colindex++)
        {

            for(int bytesindx = 0; bytesindx <nByteCounts; bytesindx++)
            {
                if(bytesindx==0)
                    *(pBuf+rowindex*nWidth*nByteCounts+colindex*nByteCounts+bytesindx)= 0;
                if(bytesindx==1)
                    *(pBuf+rowindex*nWidth*nByteCounts+colindex*nByteCounts+bytesindx)= 0;
                if(bytesindx==2)
                    *(pBuf+rowindex*nWidth*nByteCounts+colindex*nByteCounts+bytesindx)= colindex%255*(rowindex/255);;


            }
        }
    }

}



struct timeval start,stop,diff;
void *KSJ_CaptureLoop(void * loopargs)
{
    bool lutflag = false;


    memset(&start,0,sizeof(struct timeval));

    memset(&stop,0,sizeof(struct timeval));

    memset(&diff,0,sizeof(struct timeval));


    T_Thread_Param *param = (T_Thread_Param *)loopargs;



    int index=param->index ;
    unsigned char* buf = param->buf;
    int serialinloop = param->serial;
    int * stopflag =param->stopflag;

    //    printf("KSJ_CaptureLoop param->index =  %d  serialinloop = %d  \n",param->index,serialinloop);

#ifdef USING_OPENCV
    IplImage* img0;

    int width = 0;
    int height = 0;
    int bitscount = 0;
    int channelnum = 0;

    KSJ_CaptureGetSizeEx(index,&width,&height,&bitscount);
    printf("bitscount = %d \n",bitscount);
    char windowname[128];
    sprintf(windowname,"camera serialinloop = %d index = %d",serialinloop,index);

    switch(bitscount)
    {

    case 8:
        channelnum = 1;
        break;
    case 24:
        channelnum = 3;
        break;
    case 32:
        channelnum = 4;
        break;
    default:
        printf("not support");
    }

    int info ;
    KSJ_QueryFunction(index, KSJ_PROPERTY_MONO_DEVICE, &info);
    if(info>0) channelnum =1;




    img0=cvCreateImage(cvSize(g_fov[0].nWidth,g_fov[0].nHeight),IPL_DEPTH_8U,channelnum);
    img0->imageData = (char*) buf;
    Mat mtx0 = cv::cvarrToMat(img0) ;

    //	Mat	mtx0(img0);

#endif


    int nCount = 0;
    int nRet = 0;
    int nCountFpsUnit = 200;


    while(*stopflag)
    {

        if(nCount == 0)
        {
            gettimeofday(&start,0);

        }
        if(RGBFLAG)
        {

            if(CAPTURETWOSTEPFLAG)
            {
                nRet =  KSJ_CaptureDataTwoStep(index,buf);


            }else
            {
                nRet =  KSJ_CaptureRgbData(index,buf);
            }
        }
        else
        {
            if(CAPTURETWOSTEPFLAG)
            {

            }else
            {
                nRet =  KSJ_CaptureRawData(index,buf);;
            }


        }
        if(nRet==0)
        {
            nCount++;


            //            CreatSampleImage(buf,width,height,bitscount);

#ifdef USING_OPENCV

            //    	cv::cvtColor(mtx0,mtx1,CV_BayerRG2RGB);




            if(SHOWFLAG)
            {
                Size dsize =Size(mtx0.cols*0.25,mtx0.rows*0.25);

                Mat mtx1;
                resize(mtx0,mtx1,dsize);

                imshow(windowname,mtx1);
                waitKey(1);
            }
#endif

        }else
        {
            printf("nRet = %d  serial inloop = %d \n",nRet,serialinloop);

        }



        if((nCount%nCountFpsUnit)==0)
        {


            gettimeofday(&stop,0);
            TimeSubstract(&diff,&start,&stop);

            printf("index = %d  serialinloop = %d fps = %f \n",index,serialinloop, nCountFpsUnit/(float)(diff.tv_sec+(float)(diff.tv_usec/1000000.00)));
            //            lutflag =!lutflag;
            //            bool luttemp ;
            //            CHECK_RET(KSJ_LutGetEnable(index,&luttemp));
            //            printf("get luttemp = %d lutflag = %d \n",luttemp,lutflag);
            //            CHECK_RET(KSJ_LutSetEnable(index, lutflag));
            //            CHECK_RET(KSJ_LutGetEnable(index,&luttemp));
            //            printf("get again luttemp = %d \n",luttemp);

            printf("tid = %lu   %d frame  Total time : %d s,%d us \n",pthread_self(),nCountFpsUnit,(int)diff.tv_sec,(int)diff.tv_usec);

            gettimeofday(&start,0);



        }

        if(  nCount>10000 )  *stopflag=0;


    }

    //  printf("tid = %lu   exited  \n",pthread_self());


#ifdef USING_OPENCV
    cvReleaseImage( &img0 );
#endif

}





int main(int argc,void ** argv)
{

#if 1
    int ch;

    while((ch = getopt(argc, (char *const *)argv, "c:g:t:m:h:p:l")) != -1){
        printf("ch = 0x%x\n",ch );
        printf("optind = %d, optopt = %d\n", optind, optopt);

        switch(ch){
        case 'c':
            printf("选项是%c, 选项内容: %s\n", ch, optarg);
            if(1==atoi(optarg))
            {
                RGBFLAG = 1;
                printf("test capture RGB data \n");
            }
            else
            {
                RGBFLAG = 0;
                printf("test capture raw data \n");
            }

            break;
        case 'g':
            printf("选项是%c, 选项内容: %s\n", ch, optarg);
            if(1==atoi(optarg))
            {
                SHOWFLAG = 1;
                printf("has gui \n");
            }
            else
            {
                SHOWFLAG = 0;
                printf("no gui \n");
            }


            break;

        case 'm':
            printf("选项是%c, 选项内容: %s\n", ch, optarg);
            if(1==atoi(optarg))
            {
                CAPTURETWOSTEPFLAG = 0;
                printf("two step \n");
            }
            else if(2==atoi(optarg))
            {
                CAPTURETWOSTEPFLAG = 1;
                printf("one  setp\n");
            }


            break;


        case 't':
            printf("选项是%c, 选项内容: %s\n", ch, optarg);
            if(atoi(optarg)>0)
            {
                PICNUM = atoi(optarg);
                printf("test PICNUM = %d  \n",PICNUM);
            }
            else
            {

                printf("test picnum  = 0 \n");
            }

            break;

        case 'p':
            printf("选项是%c, 选项内容: %s\n", ch, optarg);
            if(atoi(optarg)>0)
            {
                MAP = atoi(optarg);
                printf("test MAP = %d  \n",MAP);
            }
            else
            {

                printf("test picnum  = 0 \n");
            }

            break;


        case 'h':
            printf("选项是%c\n", ch);
            printf("-c 1 or 0 RGB or RAW\n");
            printf("-g 1 or 0 show or not show\ \n");
            printf("-m 1 or 2 1 or 2 steps\ \n");

            printf("-t pic  count  \n");
            printf("-h");
            exit(0);
            break;
        default:
            printf("other option: %c\n", ch);
            break;
        }
    }


#endif

HEAD:
    int nRet = 0;

    nRet = KSJ_Init();

    //    KSJ_LogSet(1,"zhanwei");


    //    printf(" %s %s %d     %d   initdone \n",__FILE__,__FUNCTION__,__LINE__,nRet);

    nCamCount = KSJ_DeviceGetCount();


    if(nCamCount<=0)
    {
        printf(" %s %s %d   count  %d    \n",__FILE__,__FUNCTION__,__LINE__,nCamCount);
        exit(1);
    }

    KSJ_SetCamsParam(nCamCount);


    KSJ_CreatBufs(nCamCount,g_fov);


    int nCount = 0;



#if 1
    T_Thread_Param param;
    param.buf = g_buf[0];
    param.index = 0;
    param.stopflag = g_stopflag;

    KSJ_CaptureLoop(&param);

    KSJ_ReleaseBufs(nCamCount,g_fov);



#else


    short unsigned devtype;
    int serial;
    short unsigned fwversion;

    for(int nThreadNum = 0; nThreadNum <nCamCount;nThreadNum++)
    {
        nRet = KSJ_DeviceGetInformation(nThreadNum,&devtype,&serial,&fwversion);

        if(nRet !=RET_SUCCESS)
        {
            printf("KSJ_DeviceGetInformation error!\n");

            break;
        }


        paramarray[nThreadNum].index = nThreadNum;
        paramarray[nThreadNum].buf = g_buf[nThreadNum];
        paramarray[nThreadNum].serial =serial;
        paramarray[nThreadNum].stopflag = &g_stopflag[nThreadNum];


        /*创建线程一*/
        nRet=pthread_create(&t_id[nThreadNum],NULL,&KSJ_CaptureLoop,&paramarray[nThreadNum]);
        usleep(200);

        if(nRet!=0)
        {
            printf("Create pthread error!\n");
            return -1;
        }else
        {
            //            printf("Create  index = %d   ok t_id = %lu \n",nThreadNum,t_id[nThreadNum]);
        }

    }
#endif



    for(int nThreadNum = 0; nThreadNum <nCamCount;nThreadNum++)
    {
        pthread_join(t_id[nThreadNum],NULL);
        //          printf("pthread_join   ok t_id = %lu \n",t_id[nThreadNum]);

    }

    nRet = KSJ_UnInit();


    printf(" %s %s %d     %d   uninitdone \n",__FILE__,__FUNCTION__,__LINE__,nRet);

    //    goto HEAD; //test init

    return 0;
}



#if 0
gettimeofday(&start,0);
cl_entrance("bayer2",width,height,(unsigned char*)buf,(unsigned char*)outbuf);
gettimeofday(&stop,0);
TimeSubstract(&diff,&start,&stop);
printf("cl Total time : %d s,%d us\n",(int)diff.tv_sec,(int)diff.tv_usec);

IplImage* img0=cvCreateImage(cvSize(width-1,height),IPL_DEPTH_8U,3);
img0->imageData = (char*)outbuf;
Mat	mtx0(img0);
imshow("cl-outimage",mtx0);
#endif




#if 0



gettimeofday(&start,0);
//bayer
IplImage* img1=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
img1->imageData = (char*)buf;
Mat	mtx1(img1);

Size dsize =Size(mtx1.cols*0.25,mtx1.rows*0.25);

Mat image2 = Mat(dsize,CV_8U);


// cv::threshold(image2,image2,127,255,CV_THRESH_BINARY);

#if 1
cv::cvtColor(mtx1,mtx1,CV_BayerRG2RGB);
#endif

resize(mtx1,image2,dsize);

gettimeofday(&stop,0);


TimeSubstract(&diff,&start,&stop);


printf("cv Total time : %d s,%d us\n",(int)diff.tv_sec,(int)diff.tv_usec);


imshow("cv-outimage",image2);
waitKey(1);
//    cvSaveImage("capture.bmp",img1);
//    close(fd);




#endif
int    TimeSubstract(struct timeval *result, struct timeval *begin,struct timeval *end);

int StartTimeCounter()
{
    static struct timeval start,stop,diff;
    memset(&start,0,sizeof(struct timeval));

    memset(&stop,0,sizeof(struct timeval));

    memset(&diff,0,sizeof(struct timeval));
    gettimeofday(&start,0);

}

float CheckTimePassedSec()
{
    gettimeofday(&stop,0);
    TimeSubstract(&diff,&start,&stop);
    printf("cv Total time : %d s,%d us\n",(int)diff.tv_sec,(int)diff.tv_usec);
    return (float)(diff.tv_sec+(float)(diff.tv_usec/1000000.00));
}

int CheckTimePassedMilSec()
{
    gettimeofday(&stop,0);
    TimeSubstract(&diff,&start,&stop);
    printf("cv Total time : %d s,%d us\n",(int)diff.tv_sec,(int)diff.tv_usec);
    return (diff.tv_sec*1000+(int)(diff.tv_usec/1000));
}

int TimeSubstract(struct timeval *result, struct timeval *begin,struct timeval *end)

{

    if(begin->tv_sec > end->tv_sec)    return -1;

    if((begin->tv_sec == end->tv_sec) && (begin->tv_usec > end->tv_usec))    return -2;

    result->tv_sec    = (end->tv_sec - begin->tv_sec);

    result->tv_usec    = (end->tv_usec - begin->tv_usec);

    if(result->tv_usec < 0)

    {

        result->tv_sec--;

        result->tv_usec += 1000000;

    }

    return 0;
}


