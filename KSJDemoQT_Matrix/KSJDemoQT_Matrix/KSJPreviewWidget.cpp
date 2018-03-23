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

    int info = 0;
    KSJ_QueryFunction(0, KSJ_PROPERTY_MONO_DEVICE, &info);

    if(info==0)
    {
        m_nColormode =1;
    }else
    {
        m_nColormode =0;
    }

    if(m_nColormode){
        KSJ_BAYERMODE mode;

        KSJ_BayerGetDefaultMode(0, &mode);
        qDebug(" %s %s %d   KSJ_BayerGetDefaultMode= %d \n",__FILE__,__FUNCTION__,__LINE__,mode);

        KSJ_BayerSetMode(0, KSJ_BAYERMODE(mode+4));

        KSJ_BayerGetMode(0, &mode);
        qDebug(" %s %s %d   KSJ_BayerGetMode  = %d \n",__FILE__,__FUNCTION__,__LINE__,mode);
    }

}

void CKSJPreviewWidget:: CreatCaptureThread()
{
    if(m_pPreview_Thread==NULL)
    {
        m_pPreview_Thread = new CKSJPreviewThread(&m_mMutex);

        connect(m_pPreview_Thread,SIGNAL(UpdateImage(unsigned char * )),this,SLOT(On_UpdateSignal(unsigned char * )),Qt::BlockingQueuedConnection);

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

    int nRet= 0;

    nRet = KSJ_CaptureGetSizeEx(0,&m_nImageW,&m_nImageH,&m_nBitCount);


    m_pBuf = pData;

    m_nWindowX =0;
    m_nWindowY = 0;

    m_nImageX =0;
    m_nImageY = 0;

    this->repaint();

}

bool CKSJPreviewWidget:: IsPreviewing()
{
    if(m_pPreview_Thread != NULL )
        return true;
    else
        return false;

}

void CKSJPreviewWidget:: SaveImageBuf(QImage *pImg)
{
    static int nSaveIndex = 0;

    QString name = "captured" ;

    QString filename =name+QString(nSaveIndex)+".bmp";

    pImg->save(filename);
    nSaveIndex++;

}

void CKSJPreviewWidget:: paintEvent(QPaintEvent *event)
{
    if(m_pBuf==NULL) return;

    QImage *pImg =NULL;
    QPainter painter(this);


    m_mMutex.lock();
    QImage swapedImage ;
    QImage resizedImage ;

    switch (m_nColormode) {
    case 1:
    {
        pImg = new QImage(m_pBuf,m_nImageW,m_nImageH,QImage::Format_RGB888);

        swapedImage =pImg->rgbSwapped();

        resizedImage =pImg->scaled(this->width(),this->height());

        painter.drawImage(m_nWindowX, m_nWindowY, swapedImage, 0, 0, resizedImage.width(), resizedImage.height(),Qt::AutoColor);

    }
        break;

    case 0:
    {

        pImg = new QImage(m_pBuf,m_nImageW,m_nImageH,m_nImageW,QImage::Format_Grayscale8);

        resizedImage =pImg->scaled(this->width(),this->height());

        painter.drawImage(m_nWindowX, m_nWindowY, resizedImage, 0, 0, m_nImageW, m_nImageH,Qt::AutoColor);

    }
        break;

    default:
        break;
    }

    m_mMutex.unlock();
    if(pImg!=NULL)
        delete pImg;
}

