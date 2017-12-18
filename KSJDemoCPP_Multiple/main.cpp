#include <stdio.h>
#include "stdlib.h"
#if 1
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
int main(int argc,void ** argv)
{





	int ret = 0;
HEAD: 
	KSJ_Init();

 	struct timeval start,stop,diff;

    memset(&start,0,sizeof(struct timeval));

    memset(&stop,0,sizeof(struct timeval));

    memset(&diff,0,sizeof(struct timeval));




while(0)
{

ret = KSJ_Init();

printf(" %s %s %d     %d   initdone \n",__FILE__,__FUNCTION__,__LINE__,ret);

ret = KSJ_DeviceGetCount();


printf(" %s %s %d   count  %d    \n",__FILE__,__FUNCTION__,__LINE__,ret);

ret = KSJ_UnInit();

printf(" %s %s %d     %d   uninitdone \n",__FILE__,__FUNCTION__,__LINE__,ret);
}

int nColStart = 0;
int nRowStart = 0;
int ggx;
int nColSize = 0;
int ggsx;
int nRowSize = 0;
KSJ_ADDRESSMODE ColAddressMode;
KSJ_ADDRESSMODE RowAddressMode;

KSJ_CaptureGetDefaultFieldOfView(0,(int*)&nColStart,(int*)&nRowStart,(int *)&nColSize,(int *)&nRowSize,&ColAddressMode,&RowAddressMode);


printf(" =====================%s %s %d       nColStart = %d \n",__FILE__,__FUNCTION__,__LINE__,nColStart);
printf(" =====================%s %s %d       nRowStart = %d \n",__FILE__,__FUNCTION__,__LINE__,nRowStart);
printf(" =====================%s %s %d       nColSize = %d \n",__FILE__,__FUNCTION__,__LINE__,nColSize);
printf(" =====================%s %s %d       nRowSize = %d \n",__FILE__,__FUNCTION__,__LINE__,nRowSize);

int width = nColSize;
int height = nRowSize;


KSJ_CaptureSetFieldOfView(0,0,0,width,height,KSJ_SKIPNONE,KSJ_SKIPNONE);
KSJ_CaptureSetFieldOfView(1,0,0,width,height,KSJ_SKIPNONE,KSJ_SKIPNONE);


KSJ_CaptureGetFieldOfView(0,&nColStart,&nRowStart,&nColSize,&nRowSize,&ColAddressMode,&RowAddressMode);


printf(" =====================%s %s %d       nColStart = %d \n",__FILE__,__FUNCTION__,__LINE__,nColStart);
printf(" =====================%s %s %d       nRowStart = %d \n",__FILE__,__FUNCTION__,__LINE__,nRowStart);
printf(" =====================%s %s %d       nColSize = %d \n",__FILE__,__FUNCTION__,__LINE__,nColSize);
printf(" =====================%s %s %d       nRowSize = %d \n",__FILE__,__FUNCTION__,__LINE__,nRowSize);



float nExposureTime = 4.0;


printf(" %s %s %d       nExposureTime = %f \n",__FILE__,__FUNCTION__,__LINE__,nExposureTime);

KSJ_ExposureTimeSet(0,nExposureTime);

KSJ_ExposureTimeSet(1,nExposureTime);
printf(" %s %s %d       nExposureTime = %f \n",__FILE__,__FUNCTION__,__LINE__,nExposureTime);
KSJ_ExposureTimeGet(0,&nExposureTime);


printf(" %s %s %d       nExposureTime = %f \n",__FILE__,__FUNCTION__,__LINE__,nExposureTime);

KSJ_CaptureGetSize(0,&width,&height);
printf(" %s %s %d     %d   width %d\n",__FILE__,__FUNCTION__,__LINE__,width);
printf(" %s %s %d     %d   height %d\n",__FILE__,__FUNCTION__,__LINE__,height);

KSJ_CCM_MODE ccs_mode;

KSJ_ColorCorrectionSet(0,KSJ_HCCM_PRESETTINGS);
KSJ_ColorCorrectionGet(0,&ccs_mode);


printf(" %s %s %d     ccs_mode = %d\n",__FILE__,__FUNCTION__,__LINE__,ccs_mode);

if(argc == 2 )
{
printf("test capture RGB data \n");
RGBFLAG = 1;
}else
{
printf("test capture raw data \n");
RGBFLAG = 0;

}

//KSJ_SetParam(0, KSJ_FLIP, 0);


#if 0
	KSJ_TRIGGERMODE mode = KSJ_TRIGGER_SOFTWARE;

printf(" %s %s %d     %d   KSJ_SetTriggerMode  KSJ_TRIGGER_SOFTWARE  \n",__FILE__,__FUNCTION__,__LINE__,mode);
	ret = KSJ_SetTriggerMode(0,mode);
	
printf(" %s %s %d     %d   KSJ_SetTriggerMode \n",__FILE__,__FUNCTION__,__LINE__,ret);
	ret = KSJ_GetTriggerMode(0,&mode);

printf(" %s %s %d     %d   KSJ_GetTriggerMode ret  \n",__FILE__,__FUNCTION__,__LINE__,ret);

printf(" %s %s %d     %d   KSJ_GetTriggerMode  mode \n",__FILE__,__FUNCTION__,__LINE__,mode);


printf("\n\n\n\n\n");


	mode = KSJ_TRIGGER_INTERNAL;


printf(" %s %s %d     %d   KSJ_SetTriggerMode  KSJ_TRIGGER_EXTERNAL  \n",__FILE__,__FUNCTION__,__LINE__,mode);
	ret = KSJ_SetTriggerMode(0,mode);
	

printf(" %s %s %d     %d   KSJ_SetTriggerMode \n",__FILE__,__FUNCTION__,__LINE__,ret);
	ret = KSJ_GetTriggerMode(0,&mode);

printf(" %s %s %d     %d   KSJ_GetTriggerMode ret  \n",__FILE__,__FUNCTION__,__LINE__,ret);

printf(" %s %s %d     %d   KSJ_GetTriggerMode  mode \n",__FILE__,__FUNCTION__,__LINE__,mode);

#endif

#if  1
unsigned char * buf0 = (unsigned char *)malloc(3*width*height);

unsigned char * buf1 = (unsigned char *)malloc(3*width*height);

int i = 0;
#if 1


IplImage* img0=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,3);
img0->imageData = (char*)buf0;
Mat	mtx0(img0);


IplImage* img1=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,3);
img1->imageData = (char*)buf1;
Mat	mtx1(img1);

#endif


while(1)
{

if(i == 0)
{


    gettimeofday(&start,0);

}
if(RGBFLAG)
	ret =  KSJ_CaptureRgbData(0,buf0);
else
	
	ret =  KSJ_CaptureRawData(0,buf0);
i++;
if(i==100)
{

   i=0;


    gettimeofday(&stop,0);

    time_substract(&diff,&start,&stop);
    printf("100 frame  Total time : %d s,%d us\n",(int)diff.tv_sec,(int)diff.tv_usec);
    printf("fps = %f  \n",100.000/(float)(diff.tv_sec+(float)(diff.tv_usec/1000000.00)));

break;

}







//	ret =  KSJ_CaptureRawData(0,buf0);

//	printf(" 0 %s  %s %d    ret =  %d  KSJ_CaptureRgbData \n",__FILE__,__FUNCTION__,__LINE__,ret);
//



//sleep(1);

//	ret =  KSJ_CaptureRgbData(1,buf1);

//	printf(" 1 %s  %s %d     %d   KSJ_CaptureRgbData \n",__FILE__,__FUNCTION__,__LINE__,ret);

#if 0
	ret = KSJ_SoftStartCapture(0);

	printf(" 0 %s  %s %d    ret =  %d  KSJ_SoftStartCapture \n",__FILE__,__FUNCTION__,__LINE__,ret);


	ret=KSJ_ReadRgbDataAfterStart(0,buf0);


	printf(" 0 %s  %s %d    ret =  %d  ret=KSJ_ReadRgbDataAfterStart  \n",__FILE__,__FUNCTION__,__LINE__,ret);
#endif
//	ret=KSJ_ReadDataAfterStart(0,buf0);



//    	cv::cvtColor(mtx0,mtx1,CV_BayerRG2RGB);



//printf("%d %s \n",__LINE__,__FILE__);




//	imshow("camear 0",mtx0);

//	imshow("camear 1",mtx1);


//i--;
 //   waitKey(1);

}

//cvReleaseImage( &img0 );
//cvReleaseImage( &img1 );

free(buf0);
free(buf1);


ret =KSJ_UnInit();


printf(" %s %s %d     %d   uninit \n",__FILE__,__FUNCTION__,__LINE__,ret);

#endif
goto HEAD;
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

    cvReleaseImage( &img1 );


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


