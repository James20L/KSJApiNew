#ifndef KSJPREVIEWTHREAD_H
#define KSJPREVIEWTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "KSJPreviewThread.h"

typedef 	unsigned int (*fpRunBody)(void * pParam);


class CKSJPreviewThread : public QThread
{
    Q_OBJECT
public:

    CKSJPreviewThread(QMutex *bufmutex);
    virtual void run();
    unsigned char * m_pBuf[4];

    void SetFpRunBody(const fpRunBody &fpRunBody);
    int  StopCapture();
    bool m_bCalibration;
    void *m_pMapX;
    void *m_pMapY;
    int m_nMapWidth;
    int m_nMapHeight;
    int m_nFracPrecision;
    ~CKSJPreviewThread();
private:
    fpRunBody m_fpRunBody;



private:
    int m_bIsrunning;
    int m_nWidth;
    int m_nHeight;
    int m_nBitCount;
    QMutex * m_pMutex;


signals:
    void UpdateImage(unsigned char* pData);

private:
    int GetBufIndex();



};

void ConvetData(unsigned char *pData, int nWidth, int nHeight, int nBitCount, unsigned char* pConvertData);

#endif // KSJPREVIEWTHREAD_H
