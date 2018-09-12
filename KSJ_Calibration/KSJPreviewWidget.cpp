#include "KSJPreviewWidget.h"
#include<QPainter>
#include <QImage>
#include <qdebug.h>
#include "KSJApi.h"
#include "KSJLog.h"

CKSJPreviewWidget::CKSJPreviewWidget(QWidget *parent) : QWidget(parent)
{

    m_pPreview_Thread =NULL;
    m_pBuf = NULL;
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
        QThread::msleep(500);
        delete m_pPreview_Thread;
        m_pPreview_Thread =NULL;
    }
}



void CKSJPreviewWidget:: On_UpdateSignal(unsigned char * pData)
{
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

    this->repaint();

}

void CKSJPreviewWidget:: paintEvent(QPaintEvent *event)
{
    if(m_pBuf==NULL) return;

    QImage *pImg =NULL;
    QPainter painter(this);


    m_mMutex.lock();
    switch (m_nColormode) {
    case 0:
    {
        pImg = new QImage(m_pBuf,m_nImageW,m_nImageH,QImage::Format_RGB888);
        painter.drawImage(0, 0, pImg->rgbSwapped().scaled(this->width(),this->height()), 0, 0, -1, -1,Qt::AutoColor);
    }
        break;
    case 1:
    {
        pImg = new QImage(m_pBuf,m_nImageW,m_nImageH,QImage::Format_Grayscale8);
        painter.drawImage(0, 0, pImg->scaled(this->width(),this->height()), 0, 0, -1, -1,Qt::AutoColor);
    }
        break;
    case 2:
    {

    }
        break;
    default:
        break;
    }


    m_mMutex.unlock();
    if(pImg!=NULL)
        delete pImg;
}
