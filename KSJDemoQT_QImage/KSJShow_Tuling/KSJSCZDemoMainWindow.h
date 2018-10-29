#ifndef __KSJSCZDEMO_MAINWINDOW_H_
#define __KSJSCZDEMO_MAINWINDOW_H_

#include <QTimer>
#include <QDialog>
#include <QLabel>
#include <QScrollArea>
#include <QMatrix>

#ifdef _WIN32
#include <windows.h>
#endif


namespace Ui { class KSJSCZDemoMainWindow; }

class CKSJSCZDemoMainWindow : public QDialog
{
	Q_OBJECT

public:
	explicit CKSJSCZDemoMainWindow(QWidget *parent = 0);
	~CKSJSCZDemoMainWindow();

private:
	Ui::KSJSCZDemoMainWindow *ui;

protected slots:
	void OnCapture();
	void OnRefreshDevice();
	void OnSetFovPushButton();

	void OnTrigetModeChanged(int);

    void OnMirrorChkBoxStateChanged(int);
    void OnFlipChkBoxStateChanged(int);
    void OnLutChkBoxStateChanged(int);

	void OnFixFrameRateDoubleSpinBoxChanged(double);

protected:
	virtual void paintEvent(QPaintEvent *);
	virtual void mousePressEvent(QMouseEvent * e);
	virtual void mouseReleaseEvent(QMouseEvent * e);
	virtual void mouseMoveEvent(QMouseEvent * e);
	virtual void mouseDoubleClickEvent(QMouseEvent * e);
	virtual void wheelEvent(QWheelEvent * event);

protected:
	bool StartCaptureThread();
	bool KillCaptureThread();

	void RefreshDevice();
	void SelectDevice(int nIndex);
	void UpdateDeviceInfo();

protected:
	bool m_bIsCapturing;
	bool m_bStopCaptureThread;
	bool m_bCapturingThreadIsWorking;

protected:
#ifdef _WIN32
	HANDLE        m_hCapturingThread;
	unsigned int  m_nCapturingThreadId;
	static unsigned int __stdcall ThreadForCaptureData(LPVOID arg);
#else
	unsigned long m_nCapturingThreadId;
	static void* ThreadForCaptureData(void *arg);
#endif

protected:
	void ProcessCaptureData(unsigned char* pImageData, int w, int h, int bc, int nIndex);


protected:
	// 采集到的图像
	QImage* m_pImage0;
	QImage* m_pImage1;
};

#endif // __KSJSCZDEMO_MAINWINDOWS_H_
