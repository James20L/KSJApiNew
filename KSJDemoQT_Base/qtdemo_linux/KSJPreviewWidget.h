#ifndef CKSJPREVIEWWIDGET_H
#define CKSJPREVIEWWIDGET_H

#include <QObject>
#include <QWidget>
#include "KSJPreviewThread.h"

#include <QMutex>

class CKSJPreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CKSJPreviewWidget(QWidget *parent = 0);
    virtual  void  paintEvent(QPaintEvent *event);

    void CreatCaptureThread();
    void  DestoryCaptureThread();

private:
    int m_nColormode;

    unsigned char * m_pBuf;
    int m_nLen;
    int m_nWindowX;
    int m_nWindowY;
    int m_nWindowW;
    int m_nWindowH;

    int m_nImageX;
    int m_nImageY;
    int m_nImageW;
    int m_nImageH;
    CKSJPreviewThread * m_pPreview_Thread;

    QMutex m_mMutex;
    int m_nBitCount;



signals:

public slots:
    void On_UpdateSignal(unsigned char * pData);


};

#endif // CKSJPREVIEWWIDGET_H
