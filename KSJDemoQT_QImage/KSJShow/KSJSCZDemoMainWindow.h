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

signals:                                     //自定义信号
	void explains();            //假如要自定义槽和信号, explains信号是必须的
	void sigWBADone(float, float, float);

protected slots:

	void on_PreViewPushButton_clicked();
	void on_RefreshPushButton_clicked();
	void on_SnapImagePushButton_clicked();

	void on_DevicesComboBox_currentIndexChanged(int);

	void on_TrigetModeComboBox_currentIndexChanged(int);
	void on_TrigetMethodComboBox_currentIndexChanged(int);

	void on_FixFrameRateDoubleSpinBox_valueChanged(double);
	void on_CaptureTimeoutSpinBox_valueChanged(int);

	void on_SkipComboBox_currentIndexChanged(int);

	void on_ApplyFovPushButton_clicked();

	void on_ExpoureLineSpinBox_valueChanged(int);
	void on_ExposureTimeSpinBox_valueChanged(double);

	void on_MirrorCheckBox_stateChanged(int);
	void on_FlipCheckBox_stateChanged(int);

	void on_SensitivityComboBox_currentIndexChanged(int);

	void on_GainRedSpinBox_valueChanged(int);
	void on_GainGreenSpinBox_valueChanged(int);
	void on_GainBlueSpinBox_valueChanged(int);

	void on_ProgramLutPushButton_clicked();

	void on_WBModeComboBox_currentIndexChanged(int);
	void on_WBPresettingComboBox_currentIndexChanged(int);
	void on_WBPHiSpinBox_valueChanged(int);
	void on_WBRedSpinBox_valueChanged(double);
	void on_WBGreenSpinBox_valueChanged(double);
	void on_WBBlueSpinBox_valueChanged(double);

	void on_ProgramWBMatrixPushButton_clicked();

	void OnWBADone(float, float, float);

	void on_CCMModeComboBox_currentIndexChanged(int);
	void on_CCMPresettingComboBox_currentIndexChanged(int);

	void on_CCM00SpinBox_valueChanged(double);
	void on_CCM01SpinBox_valueChanged(double);
	void on_CCM02SpinBox_valueChanged(double);
	void on_CCM10SpinBox_valueChanged(double);
	void on_CCM11SpinBox_valueChanged(double);
	void on_CCM12SpinBox_valueChanged(double);
	void on_CCM20SpinBox_valueChanged(double);
	void on_CCM21SpinBox_valueChanged(double);
	void on_CCM22SpinBox_valueChanged(double);

	void on_ProgramCCMMatrixPushButton_clicked();

	void on_PragramSettingsPushButton_clicked();
	void on_ClearSettingsPushButton_clicked();

	void on_EmptyBufferPushButton_clicked();
	void on_CaptureRGBPushButton_clicked();
	void on_CaptureRawPushButton_clicked();


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

	void InitCnotrol();
	void UpdateDeviceInfo();

	void UpdateWhiteBalanceMatrix();
	void EnableWhiteBalanceMatrixControls(bool bEnable);

	void UpdateColorCorrectionMatrix();
	void EnableColorCorrectionMatrixControls(bool bEnable);

	bool StopPreview();
	bool StartPreview();

public:
	void WBACallback(float fMatrix[3]);

protected:
	int  m_nCamareIndex;
	bool m_bIsCapturing;
	bool m_bStopCaptureThread;
	bool m_bCapturingThreadIsWorking;

	bool m_bSaveImage;
	int  m_nSnapCount;
	QString m_strImagePath;
	QString m_strImagePreFix;

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
	void ProcessCaptureData(unsigned char* pImageData, int w, int h, int bc);


protected:
	// 采集到的图像
	QImage* m_pImage;

	// 保留图像显示位置的信息
    QRect  m_rcClient;
};

#endif // __KSJSCZDEMO_MAINWINDOWS_H_
