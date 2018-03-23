#include "KSJPreviewThread.h"
#include "KSJApi.h"
#include <QMutex>
#include "KSJRemap.h"
CKSJPreviewThread::CKSJPreviewThread(QMutex *bufmutex)
{
    int nRet = 0;
    m_bIsrunning = 1;
    m_nWidth = 0;
    m_nHeight =0;
    m_nBitCount =0;
    m_pMutex = bufmutex;
    m_bCalibration = false;
    m_pMapX = 0;
    m_pMapY = 0;
    m_nMapWidth=0;
    m_nMapHeight=0;
    m_nFracPrecision=0;

    KSJ_ReadMapfromFile("Map12",m_nMapWidth,m_nMapHeight,m_nFracPrecision,m_pMapX,m_pMapY);
    nRet = KSJ_CaptureGetSizeEx(0,&m_nWidth,&m_nHeight,&m_nBitCount);

    if(m_nMapWidth!=m_nWidth || m_nMapHeight!=m_nHeight)
    {
        if(m_pMapX !=NULL ) delete[] m_pMapX;
        if(m_pMapY !=NULL ) delete[] m_pMapY;
        m_pMapX=0;
        m_pMapY=0;
    }

    qDebug("%d %d %d %d",m_nMapWidth,m_nMapHeight,m_pMapX,m_pMapY);
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

//此处buf[0]存的是原始图,是与QImage相反的,buf[1]转换成正的

void CKSJPreviewThread:: run()
{

    int nRet = 0;

    while(m_bIsrunning)
    {
        m_pMutex->lock();
        nRet=KSJ_CaptureRgbData(0,m_pBuf[0]);
        ConvetData(m_pBuf[0],m_nWidth,m_nHeight,m_nBitCount,m_pBuf[1]);
        if(m_bCalibration && m_pMapX!=NULL)
        {
            KSJ_Remap(m_pBuf[1],m_nBitCount>>3,m_nMapWidth,m_nMapHeight,m_nFracPrecision,m_pBuf[0],m_pMapX,m_pMapY);
            m_pMutex->unlock();
            emit(UpdateImage(m_pBuf[0]));
            usleep(1000);
        }
        else
        {
            m_pMutex->unlock();
            emit(UpdateImage(m_pBuf[1]));
            usleep(1000);
        }

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
    if(m_pMapX !=NULL ) delete[] m_pMapX;
    if(m_pMapY !=NULL ) delete[] m_pMapY;
}
//这个函数把倒图变正，相当于Flip
void ConvetData(unsigned char *pData, int nWidth, int nHeight, int nBitCount, unsigned char* pConvertData)
{
    int nPixelBytes = nBitCount >> 3;
    int nWidthBytes = nWidth * nPixelBytes;
    unsigned char *pImageDataTmp;
    for (int i = 0; i < nHeight; i++)
    {
        pImageDataTmp = pData + (nHeight - 1 - i) * nWidthBytes;
        for (int j = 0; j < nWidthBytes; j++)
        {
            *(pConvertData + i * nWidthBytes + j) = *(pImageDataTmp + j);
        }
    }
}
