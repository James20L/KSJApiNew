#include "KSJPreviewWidget.h"
#include<QPainter>
#include <QImage>
#include <qdebug.h>
#include "KSJApi.h"
#include "KSJLog.h"
#include <unistd.h>

CKSJPreviewWidget::CKSJPreviewWidget(QWidget *parent) : QWidget(parent)
{

    m_pPreview_Thread =NULL;
    m_pBuf = NULL;


    int nRet = 0;

    int num =  KSJ_DeviceGetCount( );

    if(num < 1)
    {

        qDebug(" %s %s %d    KSJ_DeviceGetCount()  no cam found = %d \n",__FILE__,__FUNCTION__,__LINE__,num);
        exit(-1);
    }



    KSJ_BayerSetMode(0, KSJ_BGGR_BGR24);

    nRet = KSJ_CaptureGetSizeEx(0,&m_nWindowW,&m_nWindowH,&m_nBitCount);

    this->setGeometry(0,0,m_nWindowW,m_nWindowH);


    KSJ_WhiteBalanceSet(0,KSJ_HWB_AUTO_CONITNUOUS);



    \
}

void CKSJPreviewWidget:: CreatCaptureThread()
{
    if(m_pPreview_Thread==NULL)
    {
        m_pPreview_Thread = new CKSJPreviewThread(&m_mMutex);

        connect(m_pPreview_Thread,SIGNAL(UpdateImage(unsigned char * )),this,SLOT(On_UpdateSignal(unsigned char * )));

        m_pPreview_Thread->start();


    }

}

void CKSJPreviewWidget:: DestoryCaptureThread()
{
    if(m_pPreview_Thread!=NULL)
    {
        m_pPreview_Thread->StopCapture();
        disconnect(m_pPreview_Thread,SIGNAL(UpdateImage(unsigned char * )),this,SLOT(On_UpdateSignal(unsigned char * )));
        usleep(500000);
        delete m_pPreview_Thread;
        m_pPreview_Thread =NULL;

    }

}







void CKSJPreviewWidget:: On_UpdateSignal(unsigned char * pData)
{
    qDebug(" %s %s %d  \n",__FILE__,__FUNCTION__,__LINE__);

    int nRet= 0;

    nRet = KSJ_CaptureGetSizeEx(0,&m_nImageW,&m_nImageH,&m_nBitCount);

    if(m_nBitCount==8)
    {
        m_nColormode =1;
    }else
    {
        m_nColormode =0;
    }
    m_pBuf = pData;

    m_nWindowX =0;
    m_nWindowY = 0;

    m_nImageX =0;
    m_nImageY = 0;

    this->repaint();

}

void CKSJPreviewWidget:: paintEvent(QPaintEvent *event)
{
    qDebug(" %s %s %d  \n",__FILE__,__FUNCTION__,__LINE__);
    if(m_pBuf==NULL) return;

    QImage *pImg =NULL;
    QPainter painter(this);


    m_mMutex.lock();
    switch (m_nColormode) {
    case 0:
    {
        pImg = new QImage(m_pBuf,m_nImageW,m_nImageH,QImage::Format_RGB888);
    }
        break;
    case 1:
    {
        pImg = new QImage(m_pBuf,m_nImageW,m_nImageH,QImage::Format_Grayscale8);
    }
        break;
    case 2:
    {

    }
        break;
    default:
        break;
    }

    painter.drawImage(m_nWindowX, m_nWindowY, *pImg, m_nImageX, m_nImageY, m_nImageW, m_nImageH,Qt::AutoColor);
    m_mMutex.unlock();
    if(pImg!=NULL)
        delete pImg;
    qDebug(" %s %s %d  \n",__FILE__,__FUNCTION__,__LINE__);
}

