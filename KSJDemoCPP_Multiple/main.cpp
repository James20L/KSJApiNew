#include <stdio.h>
#include "stdlib.h"
#include <pthread.h>
#include <sys/types.h>

#ifdef USING_OPENCV
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
using namespace cv;
#endif

#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#include "KSJApi.h"
using namespace std;

int    time_substract(struct timeval *result, struct timeval *begin,struct timeval *end);


int RGBFLAG = 0;
int SHOWFLAG = 0;
int PICNUM = 0;
int CAPTURETWOSTEPFLAG = 0;




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
        KSJ_CaptureGetDefaultFieldOfView(nIndex,(int*)&nColStart,(int*)&nRowStart,(int *)&nColSize,(int *)&nRowSize,&ColAddressMode,&RowAddressMode);

#define printf  //
        printf(" =====================%s %s %d       nColStart = %d \n",__FILE__,__FUNCTION__,__LINE__,nColStart);
        printf(" =====================%s %s %d       nRowStart = %d \n",__FILE__,__FUNCTION__,__LINE__,nRowStart);
        printf(" =====================%s %s %d       nColSize = %d \n",__FILE__,__FUNCTION__,__LINE__,nColSize);
        printf(" =====================%s %s %d       nRowSize = %d \n",__FILE__,__FUNCTION__,__LINE__,nRowSize);

        int width = nColSize;
        int height = nRowSize;


        KSJ_CaptureSetFieldOfView(nIndex,0,0,width,height,KSJ_SKIPNONE,KSJ_SKIPNONE);

        KSJ_CaptureGetFieldOfView(nIndex,&nColStart,&nRowStart,&nColSize,&nRowSize,&ColAddressMode,&RowAddressMode);


        printf(" =====================%s %s %d       nColStart = %d \n",__FILE__,__FUNCTION__,__LINE__,nColStart);
        printf(" =====================%s %s %d       nRowStart = %d \n",__FILE__,__FUNCTION__,__LINE__,nRowStart);
        printf(" =====================%s %s %d       nColSize = %d \n",__FILE__,__FUNCTION__,__LINE__,nColSize);
        printf(" =====================%s %s %d       nRowSize = %d \n",__FILE__,__FUNCTION__,__LINE__,nRowSize);


#if 0
        float nExposureTime = 10.0;
        printf(" %s %s %d       nExposureTime = %f \n",__FILE__,__FUNCTION__,__LINE__,nExposureTime);

        KSJ_ExposureTimeSet(nIndex,nExposureTime);
        printf(" %s %s %d       nExposureTime = %f \n",__FILE__,__FUNCTION__,__LINE__,nExposureTime);
#endif
        int nlines = 0;

        KSJ_SetParam(nIndex,KSJ_EXPOSURE_LINES,500);


        //KSJ_GetParam(nIndex,KSJ_EXPOSURE_LINES,&nlines);

        //printf(" =====================%s %s %d       nlines = %d \n",__FILE__,__FUNCTION__,__LINE__,nlines);


        KSJ_CaptureGetSize(nIndex,&g_fov[nIndex].nWidth,&g_fov[nIndex].nHeight);
        printf(" %s %s %d      FOVS[%d].nWidth %d\n",__FILE__,__FUNCTION__,__LINE__,nIndex,g_fov[nIndex].nWidth);
        printf(" %s %s %d      FOVS[%d].nHeight %d\n",__FILE__,__FUNCTION__,__LINE__,nIndex,g_fov[nIndex].nHeight);

        KSJ_CCM_MODE ccs_mode;
        KSJ_ColorCorrectionSet(0,KSJ_HCCM_PRESETTINGS);
        KSJ_ColorCorrectionGet(0,&ccs_mode);

        KSJ_TRIGGERMODE mode = KSJ_TRIGGER_SOFTWARE;

        printf(" %s %s %d     %d   KSJ_SetTriggerMode  KSJ_TRIGGER_SOFTWARE  \n",__FILE__,__FUNCTION__,__LINE__,mode);
        nRet = KSJ_TriggerModeSet(0,mode);

        printf(" %s %s %d     %d   KSJ_SetTriggerMode \n",__FILE__,__FUNCTION__,__LINE__,nRet);
        nRet = KSJ_TriggerModeGet(0,&mode);

        printf(" %s %s %d     %d   KSJ_GetTriggerMode nRet  \n",__FILE__,__FUNCTION__,__LINE__,nRet);

        printf(" %s %s %d     %d   KSJ_GetTriggerMode  mode \n",__FILE__,__FUNCTION__,__LINE__,mode);

        mode = KSJ_TRIGGER_INTERNAL;

        printf(" %s %s %d     %d   KSJ_SetTriggerMode  KSJ_TRIGGER_EXTERNAL  \n",__FILE__,__FUNCTION__,__LINE__,mode);
        nRet = KSJ_TriggerModeSet(0,mode);

        printf(" %s %s %d     %d   KSJ_SetTriggerMode \n",__FILE__,__FUNCTION__,__LINE__,nRet);
        nRet = KSJ_TriggerModeGet(0,&mode);

        printf(" %s %s %d     %d   KSJ_GetTriggerMode nRet  \n",__FILE__,__FUNCTION__,__LINE__,nRet);

        printf(" %s %s %d     %d   KSJ_GetTriggerMode  mode \n",__FILE__,__FUNCTION__,__LINE__,mode);



#define printf printf

    }



}


void *KSJ_CaptureLoop(void * loopargs)
{
    struct timeval start,stop,diff;
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
    img0=cvCreateImage(cvSize(g_fov[0].nWidth,g_fov[0].nHeight),IPL_DEPTH_8U,1);
    img0->imageData = (char*) buf;
    Mat	mtx0(img0);

#endif


    int nCount = 0;
    int nRet = 0;
    int nCountFpsUnit = 1000;

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


#ifdef USING_OPENCV

            //    	cv::cvtColor(mtx0,mtx1,CV_BayerRG2RGB);

            imshow("camear 0",mtx0);
            waitKey(1);

#endif

        }else
        {
            printf("nRet = %d  serial inloop = %d \n",nRet,serialinloop);

        }



        if(nCount==nCountFpsUnit)
        {

            nCount=0;
            gettimeofday(&stop,0);
            time_substract(&diff,&start,&stop);

            printf("index = %d  serialinloop = %d fps = %f ",index,serialinloop, nCountFpsUnit/(float)(diff.tv_sec+(float)(diff.tv_usec/1000000.00)));
            printf("tid = %lu   %d frame  Total time : %d s,%d us \n",pthread_self(),nCountFpsUnit,(int)diff.tv_sec,(int)diff.tv_usec);

            *stopflag = 0;

        }



    }

    //  printf("tid = %lu   exited  \n",pthread_self());


#ifdef USING_OPENCV
    cvReleaseImage( &img0 );
#endif

}





int main(int argc,void ** argv)
{
#if 1
    char ch;

    while((ch = getopt(argc, (char *const *)argv, "c:g:t:h")) != -1){
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
            printf("选项是%m, 选项内容: %s\n", ch, optarg);
            if(1==atoi(optarg))
            {
                CAPTURETWOSTEPFLAG = 1;
                printf("two step \n");
            }
            else
            {
                CAPTURETWOSTEPFLAG = 0;
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
        case 'h':
            printf("选项是%c\n", ch);
            printf("-c 1 or 0 RGB or RAW\n");
            printf("-g 1 or 0 show or not show\ \n");
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

    //  printf(" %s %s %d   count  %d    \n",__FILE__,__FUNCTION__,__LINE__,nCamCount);


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
time_substract(&diff,&start,&stop);
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


time_substract(&diff,&start,&stop);


yy    printf("cv Total time : %d s,%d us\n",(int)diff.tv_sec,(int)diff.tv_usec);


imshow("cv-outimage",image2);
waitKey(1);
//    cvSaveImage("capture.bmp",img1);
//    close(fd);




#endif



int    time_substract(struct timeval *result, struct timeval *begin,struct timeval *end)

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


