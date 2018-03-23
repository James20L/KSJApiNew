#include "KSJPreviewThread.h"
#include "KSJApi.h"
#include <QMutex>
CKSJPreviewThread::CKSJPreviewThread(QMutex *bufmutex)
{
    int nRet = 0;
    m_bIsrunning = 1;
    m_nWidth = 0;
    m_nHeight =0;
    m_nBitCount =0;
    m_pMutex = bufmutex;


    nRet = KSJ_CaptureGetSizeEx(0,&m_nWidth,&m_nHeight,&m_nBitCount);
    m_pBuf[0]=(unsigned char *)malloc(m_nWidth*m_nHeight*m_nBitCount/8);
    m_pBuf[1]=(unsigned char *)malloc(m_nWidth*m_nHeight*m_nBitCount/8);

}

int CKSJPreviewThread:: GetBufIndex()
{
    return 0;

}

int CKSJPreviewThread:: StopCapture()
{

    m_bIsrunning = 0;

    return 0;

}


void CKSJPreviewThread :: CreatSampleImage(unsigned char * pBuf,int nWidth,int nHeight,int nBitCounts)
{
    int nByteCounts = nBitCounts >>3;

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


int CKSJPreviewThread:: CheckData( unsigned char * buf)
{

    for(int index = 0;index<m_nWidth*m_nHeight*m_nBitCount/8;index++)
    {



        if(*buf < 127)
        {
            *buf = 0;
            qDebug(" %s %s %d  index = %d\n",__FILE__,__FUNCTION__,__LINE__,index);
            break;
        }

        buf++;
    }
    return 0;

}


void CKSJPreviewThread:: run()
{

    int nRet = 0;

    int info = 0;
    KSJ_QueryFunction(0, KSJ_PROPERTY_MONO_DEVICE, &info);
    int nColormode = 0;

    if(info==0)
    {
        nColormode =1;
    }else
    {
        nColormode =0;
    }

    while(m_bIsrunning)
    {
        m_pMutex->lock();

        if(nColormode)
            nRet=KSJ_CaptureRgbData(0,m_pBuf[GetBufIndex()]);
        else
            nRet=KSJ_CaptureRawData(0,m_pBuf[GetBufIndex()]);



//        CreatSampleImage(m_pBuf[GetBufIndex()],m_nWidth,m_nHeight,m_nBitCount);
//        CheckData(m_pBuf[GetBufIndex()]);

        m_pMutex->unlock();
        emit(UpdateImage(m_pBuf[0]));
        usleep(100);

    }

}

void CKSJPreviewThread::SetFpRunBody(const fpRunBody &fpRunBody)
{
    m_fpRunBody = fpRunBody;
}

CKSJPreviewThread:: ~CKSJPreviewThread()
{

    if(m_pBuf[0]!=NULL) free(m_pBuf[0]);
    if(m_pBuf[1]!=NULL) free(m_pBuf[1]);
    qDebug(" %s %s %d \n",__FILE__,__FUNCTION__,__LINE__);
}
