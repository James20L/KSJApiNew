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


void CKSJPreviewThread:: run()
{

    int nRet = 0;

    while(m_bIsrunning)
    {
        m_pMutex->lock();
         qDebug(" %s %s %d \n",__FILE__,__FUNCTION__,__LINE__);
        nRet=KSJ_CaptureRgbData(0,m_pBuf[GetBufIndex()]);
        qDebug(" %s %s %d \n",__FILE__,__FUNCTION__,__LINE__);
                m_pMutex->unlock();
        emit(UpdateImage(m_pBuf[GetBufIndex()]));
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
