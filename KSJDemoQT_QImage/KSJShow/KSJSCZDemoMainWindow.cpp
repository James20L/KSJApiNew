

#include <QPainter>
#include <QResizeEvent>
#include <QScrollBar>
#include <QSettings>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>

#include "KSJSCZDemoMainWindow.h"
#include "ui_KSJSCZDemoMainWindow.h"

#include "KSJApi.h"

#ifndef _WIN32
#include <unistd.h>
#include <sys/time.h>
#endif

#ifdef _WIN32
#include <process.h>

#ifdef _DEBUG
#pragma comment( lib, "KSJApid.lib" )
#pragma comment(lib, "C:/Qt/Qt5.7.0/5.7/msvc2013/lib/Qt5Widgetsd.lib")
#pragma comment(lib, "C:/Qt/Qt5.7.0/5.7/msvc2013/lib/Qt5Guid.lib")
#pragma comment(lib, "C:/Qt/Qt5.7.0/5.7/msvc2013/lib/Qt5Cored.lib")
#pragma comment(lib, "C:/Qt/Qt5.7.0/5.7/msvc2013/lib/qtmaind.lib")
#else
#pragma comment( lib, "KSJApi.lib" )
#pragma comment(lib, "C:/Qt/Qt5.7.0/5.7/msvc2013/lib/Qt5Widgets.lib")
#pragma comment(lib, "C:/Qt/Qt5.7.0/5.7/msvc2013/lib/Qt5Gui.lib")
#pragma comment(lib, "C:/Qt/Qt5.7.0/5.7/msvc2013/lib/Qt5Core.lib")
#pragma comment(lib, "C:/Qt/Qt5.7.0/5.7/msvc2013/lib/qtmain.lib")
#endif

#endif

#ifdef _WIN32
unsigned int __stdcall CKSJSCZDemoMainWindow::ThreadForCaptureData(LPVOID arg)
#else
void* CKSJSCZDemoMainWindow::ThreadForCaptureData(void *arg)
#endif
{
	if (NULL == arg)
	{
#ifdef _WIN32
		return -1;
#else
		pthread_exit((char*)"ThreadForSignalMode");
#endif
	}

	CKSJSCZDemoMainWindow* pMainWindow = (CKSJSCZDemoMainWindow*)arg;

	pMainWindow->update();

	int nRet;	
	int nWidth;
	int nHeight;
	int nBitCount;
	int nBufferSize = 0;
	unsigned char* pImageBuffer = NULL;

	int nIndex = 0;

	while (!pMainWindow->m_bStopCaptureThread)
	{
		if (pMainWindow->m_bIsCapturing && pMainWindow->m_nCamareIndex >= 0)
		{
			nRet = KSJ_CaptureGetSizeEx(pMainWindow->m_nCamareIndex, &nWidth, &nHeight, &nBitCount);

			if (nRet == RET_SUCCESS)
			{
				if (nBufferSize < (nWidth*nHeight*nBitCount / 8))
				{
					if (pImageBuffer != NULL)
					{
						delete[]pImageBuffer;
						pImageBuffer = NULL;
					}

					nBufferSize = nWidth*nHeight*nBitCount / 8;
					pImageBuffer = new unsigned char[nBufferSize];
				}

				nRet = KSJ_CaptureRgbData(pMainWindow->m_nCamareIndex, pImageBuffer);

				if (nRet == RET_SUCCESS)
				{
					// 采集图像以后，将内存数据转换成QImage数据,这样pImageData的数据就被转移到QImage里面，以后可以自己进行算法操作
					pMainWindow->ProcessCaptureData(pImageBuffer, nWidth, nHeight, nBitCount);
					// 读取图像以后，一定要KSJSCZ_ReleaseBuffer，这样FPGA就把这个内存清空，可以重新将图像采集到这个内存区
				}
				else
				{
					printf("KSJ_CaptureRgbData failed : ret = %d, index = %d \n", nRet, nIndex);

				}
			}
			else
			{
				printf("KSJ_CaptureGetSizeEx failed : ret = %d, index = %d \n", nRet, nIndex);

			}

			++nIndex;
		}
		else
		{
#ifdef _WIN32
			Sleep(1);
#else
			usleep(1000);
#endif
		}
	}

#ifdef _WIN32
	return 0;
#else
	pthread_exit(0);
	return ((void *)0);
#endif
}


CKSJSCZDemoMainWindow::CKSJSCZDemoMainWindow(QWidget *parent) :
QDialog(parent)
, ui(new Ui::KSJSCZDemoMainWindow)
, m_pImage(NULL)
, m_nCamareIndex(0)
, m_bSaveImage(false)
, m_nSnapCount(0)
, m_bIsCapturing(false)
, m_bStopCaptureThread(false)
, m_bCapturingThreadIsWorking(false)
#ifdef _WIN32
, m_hCapturingThread (NULL)
, m_nCapturingThreadId(0)
#else
, m_nCapturingThreadId(0)
#endif
{
    ui->setupUi(this);
	setMouseTracking(true);

#ifndef _DEBUG
	ui->TestingTab->setHidden(true);
#endif

	InitCnotrol();

	connect(ui->ExitAppPushButton, SIGNAL(clicked()), qApp, SLOT(quit()));

	connect(this, SIGNAL(sigWBADone(float, float, float)), this, SLOT(OnWBADone(float, float, float)));

	// 初始化
	RefreshDevice();

	StartCaptureThread();

	QDateTime dt = QDateTime::currentDateTime();
	m_strImagePreFix = QCoreApplication::applicationDirPath() + "/"+ dt.toString("yyyy-MM-ddhhmmss-");

}

CKSJSCZDemoMainWindow::~CKSJSCZDemoMainWindow()
{
	KillCaptureThread();

    KSJ_UnInit();

	if (m_pImage != NULL)
	{
		delete m_pImage;
		m_pImage = NULL;
	}

	delete ui;
}

void CKSJSCZDemoMainWindow::paintEvent(QPaintEvent *)
{
	// 图像不为空，则开始显示
	if (m_pImage != NULL)
	{
		QPainter painter(this);

		int w = size().width() - 320;
		int h = size().height();

		int iw = m_pImage->width();
		int ih = m_pImage->height();

		float fw = ((float)(w)) / iw;
		float fh = ((float)(h)) / ih;
		float f = fw < fh ? fw : fh;

		int dw = (int)(f*iw);
		int dh = (int)(f*ih);

		painter.drawImage(QRect(320 + (w - dw) / 2, (h - dh) / 2, dw, dh), *m_pImage, QRect(0, 0, iw, ih));
	}
}

bool CKSJSCZDemoMainWindow::StartCaptureThread()
{
	if (m_bCapturingThreadIsWorking) return true;

	m_bCapturingThreadIsWorking = true;

#ifdef _WIN32
	m_hCapturingThread = (HANDLE)_beginthreadex(NULL, 0, ThreadForCaptureData, this, 0, &m_nCapturingThreadId);

	return (m_hCapturingThread != NULL);
#else
	m_bStopCaptureThread = false;

	pthread_attr_t  attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&m_nCapturingThreadId, &attr, ThreadForCaptureData, (void *)this);
	pthread_attr_destroy(&attr);
#endif

	return true;
}

bool CKSJSCZDemoMainWindow::KillCaptureThread()
{
#ifdef _WIN32
	if (m_hCapturingThread == NULL) return true;
	if (!m_bCapturingThreadIsWorking) return true;

	m_bStopCaptureThread = true;

	if (WaitForSingleObject(m_hCapturingThread, 10000) != WAIT_OBJECT_0)
	{
		TerminateThread(m_hCapturingThread, 0);
	}

	m_hCapturingThread = NULL;

	m_bCapturingThreadIsWorking = false;
#else
	m_bStopCaptureThread = true;
	m_bCapturingThreadIsWorking = false;
#endif
	return true;
}

void CKSJSCZDemoMainWindow::on_PreViewPushButton_clicked()
{
	if (m_nCamareIndex >= 0)
	{
		if (m_bIsCapturing) this->StopPreview();
		else                this->StartPreview();
	}
}

void CKSJSCZDemoMainWindow::on_RefreshPushButton_clicked()
{
	bool bIsCapturing = this->StopPreview();

	RefreshDevice();

	if (bIsCapturing) this->StartPreview();
}

void CKSJSCZDemoMainWindow::on_SnapImagePushButton_clicked()
{
	m_strImagePath = m_strImagePreFix + QString("%1.bmp").arg(m_nSnapCount, 3, 10, QChar('0'));
	m_bSaveImage = true;
}

#define KSJ_MSB(x)     ((x&0xff00) >> 8)
#define KSJ_LSB(x)     (x & 0x00ff)

void CKSJSCZDemoMainWindow::RefreshDevice()
{
	m_nCamareIndex = -1;

	ui->DevicesComboBox->clear();

	KSJ_UnInit();

#ifdef _WIN32
	Sleep(1);
#else
	usleep(1000);
#endif

	int nRet = KSJ_Init();

	if (nRet != RET_SUCCESS) return;

#ifdef OLD_KSJAPI
	KSJ_DEVICETYPE pusDeviceType;
#else
	unsigned short pusDeviceType;
#endif
	int            pnSerials;
	unsigned short pusFirmwareVersion;
	unsigned short pusFpgaVersion;

	int nCamCount = KSJ_DeviceGetCount();

	printf(" %s %s %d   count  %d    \n", __FILE__, __FUNCTION__, __LINE__, nCamCount);

	if (nCamCount <= 0)
	{
        return;
	}

	QString strCamare;

	for (int i = 0; i < nCamCount; ++i)
	{
		nRet = KSJ_DeviceGetInformationEx(i, &pusDeviceType, &pnSerials, &pusFirmwareVersion, &pusFpgaVersion);

		if (nRet != RET_SUCCESS)
		{
			strCamare = QString("Camare%1: Unknow").arg(i);
		}
		else
		{
			strCamare = QString("Camare%1: Serial(%2)-DeviceType(%3)-FW(%4.%5)-PL(%6.%7)").arg(i).arg(pnSerials).arg(pusDeviceType).arg(KSJ_MSB(pusFirmwareVersion)).arg(KSJ_LSB(pusFirmwareVersion)).arg(KSJ_MSB(pusFpgaVersion)).arg(KSJ_LSB(pusFpgaVersion));
		}

        ui->DevicesComboBox->addItem(strCamare);
	}

	SelectDevice(-1);
}

void CKSJSCZDemoMainWindow::SelectDevice(int nIndex)
{
	bool bIsCapturing = this->StopPreview();

	m_nCamareIndex = -1;

	if (ui->DevicesComboBox->count() <= 0) return;

	if (nIndex <= 0) m_nCamareIndex = 0;
	else             m_nCamareIndex = nIndex;

	ui->DevicesComboBox->setCurrentIndex(m_nCamareIndex);
	UpdateDeviceInfo();

	if (bIsCapturing) this->StartPreview();
}

void CKSJSCZDemoMainWindow::on_DevicesComboBox_currentIndexChanged(int nIndex)
{
	if (ui->DevicesComboBox->count() <= 0) return;

	bool bIsCapturing = this->StopPreview();

	SelectDevice(nIndex);
	UpdateDeviceInfo();

	if (bIsCapturing) this->StartPreview();
}

void CKSJSCZDemoMainWindow::on_TrigetModeComboBox_currentIndexChanged(int nIndex)
{
	if (m_nCamareIndex == -1) return;

	bool bIsCapturing = this->StopPreview();

	KSJ_TriggerModeSet(m_nCamareIndex, KSJ_TRIGGERMODE(nIndex));
	UpdateDeviceInfo();

	if (bIsCapturing) this->StartPreview();
}

void CKSJSCZDemoMainWindow::on_TrigetMethodComboBox_currentIndexChanged(int nIndex)
{
	if (m_nCamareIndex == -1) return;

	bool bIsCapturing = this->StopPreview();

	KSJ_TriggerMethodSet(m_nCamareIndex, KSJ_TRIGGERMETHOD(nIndex));
	UpdateDeviceInfo();

	if (bIsCapturing) this->StartPreview();
}

void CKSJSCZDemoMainWindow::on_FixFrameRateDoubleSpinBox_valueChanged(double value)
{
	if (m_nCamareIndex == -1) return;

	bool bIsCapturing = this->StopPreview();

	KSJ_SetFixedFrameRateEx(m_nCamareIndex, value);
	UpdateDeviceInfo();

	if (bIsCapturing) this->StartPreview();
}

void CKSJSCZDemoMainWindow::on_CaptureTimeoutSpinBox_valueChanged(int value)
{
	if (m_nCamareIndex == -1) return;

	bool bIsCapturing = this->StopPreview();

	KSJ_CaptureSetTimeOut(m_nCamareIndex, value);
	UpdateDeviceInfo();

	if (bIsCapturing) this->StartPreview();
}

void CKSJSCZDemoMainWindow::on_SkipComboBox_currentIndexChanged(int index)
{
	if (m_nCamareIndex == -1) return;

	bool bIsCapturing = this->StopPreview();

	int nRet;
	int nColStart;
	int nRowStart;
	int nColSize;
	int nRowSize;
	unsigned short nsMultiFrameNum;
	KSJ_ADDRESSMODE AmCol;
	KSJ_ADDRESSMODE AmRow;

	nRet = KSJ_CaptureGetFieldOfViewEx(m_nCamareIndex, &nColStart, &nRowStart, &nColSize, &nRowSize, &AmCol, &AmRow, &nsMultiFrameNum);

	if (nRet == RET_SUCCESS)
	{
		AmCol = (KSJ_ADDRESSMODE)index;
		AmRow = (KSJ_ADDRESSMODE)index;

		KSJ_CaptureSetFieldOfViewEx(m_nCamareIndex, nColStart, nRowStart, nColSize, nRowSize, AmCol, AmRow, nsMultiFrameNum);
	}
	UpdateDeviceInfo();

	if (bIsCapturing) this->StartPreview();
}

void CKSJSCZDemoMainWindow::on_ApplyFovPushButton_clicked()
{
	if (m_nCamareIndex == -1) return;

	bool bIsCapturing = this->StopPreview();

	int nRet;
	int nColStart;
	int nRowStart;
	int nColSize;
	int nRowSize;
	unsigned short nsMultiFrameNum;
	KSJ_ADDRESSMODE AmCol;
	KSJ_ADDRESSMODE AmRow;

	nRet = KSJ_CaptureGetFieldOfViewEx(m_nCamareIndex, &nColStart, &nRowStart, &nColSize, &nRowSize, &AmCol, &AmRow, &nsMultiFrameNum);

	if (nRet == RET_SUCCESS)
	{
		nColStart = ui->ColStartSpinBox->value();
		nRowStart = ui->ColSizeSpinBox->value();
		nColSize = ui->RowStartSpinBox->value();
		nRowSize = ui->RowSizeSpinBox->value();
		nsMultiFrameNum = ui->MultiFrameNumberSpinBox->value();

		KSJ_CaptureSetFieldOfViewEx(m_nCamareIndex, nColStart, nRowStart, nColSize, nRowSize, AmCol, AmRow, nsMultiFrameNum);
	}
	UpdateDeviceInfo();

	if (bIsCapturing) this->StartPreview();
}

void CKSJSCZDemoMainWindow::on_ExpoureLineSpinBox_valueChanged(int value)
{
	if (m_nCamareIndex == -1) return;

	bool bIsCapturing = this->StopPreview();

	KSJ_SetParam(m_nCamareIndex, KSJ_EXPOSURE_LINES, value);
	UpdateDeviceInfo();

	if (bIsCapturing) this->StartPreview();
}

void CKSJSCZDemoMainWindow::on_ExposureTimeSpinBox_valueChanged(double value)
{
	if (m_nCamareIndex == -1) return;

	bool bIsCapturing = this->StopPreview();

	KSJ_ExposureTimeSet(m_nCamareIndex, value);
	UpdateDeviceInfo();

	if (bIsCapturing) this->StartPreview();
}

void CKSJSCZDemoMainWindow::on_MirrorCheckBox_stateChanged(int value)
{
	if (m_nCamareIndex == -1) return;

	bool bIsCapturing = this->StopPreview();

	KSJ_SetParam(m_nCamareIndex, KSJ_MIRROR, (value == Qt::Checked) ? 1 : 0);
	UpdateDeviceInfo();

	if (bIsCapturing) this->StartPreview();
}

void CKSJSCZDemoMainWindow::on_FlipCheckBox_stateChanged(int value)
{
	if (m_nCamareIndex == -1) return;

	bool bIsCapturing = this->StopPreview();

	KSJ_SetParam(m_nCamareIndex, KSJ_FLIP, (value == Qt::Checked) ? 1 : 0);
	UpdateDeviceInfo();

	if (bIsCapturing) this->StartPreview();
}

void CKSJSCZDemoMainWindow::on_SensitivityComboBox_currentIndexChanged(int index)
{
	if (m_nCamareIndex == -1) return;

	bool bIsCapturing = this->StopPreview();

	KSJ_SensitivitySetMode(m_nCamareIndex, (KSJ_SENSITIVITYMODE)index);
	UpdateDeviceInfo();

	if (bIsCapturing) this->StartPreview();

}

void CKSJSCZDemoMainWindow::on_GainRedSpinBox_valueChanged(int value)
{
	if (m_nCamareIndex == -1) return;

	bool bIsCapturing = this->StopPreview();

	KSJ_SetParam(m_nCamareIndex, KSJ_RED, value);
	UpdateDeviceInfo();

	if (bIsCapturing) this->StartPreview();
}

void CKSJSCZDemoMainWindow::on_GainGreenSpinBox_valueChanged(int value)
{
	if (m_nCamareIndex == -1) return;

	bool bIsCapturing = this->StopPreview();

	KSJ_SetParam(m_nCamareIndex, KSJ_GREEN, value);
	UpdateDeviceInfo();

	if (bIsCapturing) this->StartPreview();
}

void CKSJSCZDemoMainWindow::on_GainBlueSpinBox_valueChanged(int value)
{
	if (m_nCamareIndex == -1) return;

	bool bIsCapturing = this->StopPreview();

	KSJ_SetParam(m_nCamareIndex, KSJ_BLUE, value);
	UpdateDeviceInfo();

	if (bIsCapturing) this->StartPreview();
}

void CKSJSCZDemoMainWindow::on_ProgramLutPushButton_clicked()
{
	if (m_nCamareIndex < 0) return;

	QString strFilePath = QFileDialog::getOpenFileName(this, "Select lut data file", "", "ktb File(*.ktb)|*.ktb|All Files(*.*)|*.*||");

	if (!strFilePath.isEmpty())
	{
		int nRet = KSJ_LutFileDownload(m_nCamareIndex, strFilePath.toStdString().c_str());

        TCHAR szErrorInfo[512] = { '\0' };
		KSJ_GetErrorInfo(nRet, szErrorInfo);
		QMessageBox::about(this, "CatchBEST", szErrorInfo);
	}
}

bool CKSJSCZDemoMainWindow::StopPreview()
{
	if (m_nCamareIndex >= 0)
	{
		if (m_bIsCapturing)
		{
			m_bIsCapturing = false;
			ui->PreViewPushButton->setText(m_bIsCapturing ? "Stop" : "Start");
			return true;
		}
	}

	return false;
}

bool CKSJSCZDemoMainWindow::StartPreview()
{
	if (m_nCamareIndex >= 0)
	{
		if (!m_bIsCapturing)
		{
			m_bIsCapturing = true;
			ui->PreViewPushButton->setText(m_bIsCapturing ? "Stop" : "Start");
			return true;
		}
	}

	return false;
}

void CKSJSCZDemoMainWindow::InitCnotrol()
{
	ui->TrigetModeComboBox->blockSignals(true);
	ui->TrigetModeComboBox->addItem("Internal");
	ui->TrigetModeComboBox->addItem("External");
	ui->TrigetModeComboBox->addItem("Software");
	ui->TrigetModeComboBox->addItem("Fixed Frame Rate");
	ui->TrigetModeComboBox->blockSignals(false);

	ui->TrigetMethodComboBox->blockSignals(true);
	ui->TrigetMethodComboBox->addItem("Falling Edge");
	ui->TrigetMethodComboBox->addItem("Rising Edge");
	ui->TrigetMethodComboBox->addItem("High Level");
	ui->TrigetMethodComboBox->addItem("Low Level");
	ui->TrigetMethodComboBox->blockSignals(false);

	ui->SkipComboBox->blockSignals(true);
	ui->SkipComboBox->addItem("Normal");
	ui->SkipComboBox->addItem("2 x 2 Skip");
	ui->SkipComboBox->addItem("3 x 3 Skip");
	ui->SkipComboBox->addItem("4 x 4 Skip");
	ui->SkipComboBox->addItem("8 x 8 Skip");
	ui->SkipComboBox->blockSignals(false);

	ui->SensitivityComboBox->blockSignals(true);
	ui->SensitivityComboBox->addItem("Low");
	ui->SensitivityComboBox->addItem("Mid");
	ui->SensitivityComboBox->addItem("High");
	ui->SensitivityComboBox->addItem("Higher");
	ui->SensitivityComboBox->addItem("Highest");
	ui->SensitivityComboBox->blockSignals(false); 

	ui->WBModeComboBox->blockSignals(true);
	ui->WBModeComboBox->addItem("Disable");
	ui->WBModeComboBox->addItem("Software Presettings");
	ui->WBModeComboBox->addItem("Software Auto Once");
	ui->WBModeComboBox->addItem("Software Auto Continuous");
	ui->WBModeComboBox->addItem("Software Manual");
	ui->WBModeComboBox->addItem("Hardware Presettings");
	ui->WBModeComboBox->addItem("Hardware Auto Once");
	ui->WBModeComboBox->addItem("Hardware Auto Continuous");
	ui->WBModeComboBox->addItem("Hardware Manual");
	ui->WBModeComboBox->blockSignals(false);

	ui->WBPresettingComboBox->blockSignals(true);
	ui->WBPresettingComboBox->addItem("Color Temperature 5000K");
	ui->WBPresettingComboBox->addItem("Color Temperature 6500K");
	ui->WBPresettingComboBox->addItem("Color Temperature 2800K");
	ui->WBPresettingComboBox->blockSignals(false);

	ui->CCMModeComboBox->blockSignals(true);
	ui->CCMModeComboBox->addItem("Disable");
	ui->CCMModeComboBox->addItem("Software Presettings");
	ui->CCMModeComboBox->addItem("Software Manual");
	ui->CCMModeComboBox->addItem("Hardware Presettings");
	ui->CCMModeComboBox->addItem("Hardware Manual");
	ui->CCMModeComboBox->blockSignals(false);

	ui->CCMPresettingComboBox->blockSignals(true);
	ui->CCMPresettingComboBox->addItem("Color Temperature 5000K");
	ui->CCMPresettingComboBox->addItem("Color Temperature 6500K");
	ui->CCMPresettingComboBox->addItem("Color Temperature 2800K");
	ui->CCMPresettingComboBox->blockSignals(false);
}

void CKSJSCZDemoMainWindow::UpdateDeviceInfo()
{
	if (m_nCamareIndex == -1) return;

	int nRet;

	int nTriggerModeIndex = 0;
	nRet = KSJ_TriggerModeGet(m_nCamareIndex, (KSJ_TRIGGERMODE*)&nTriggerModeIndex);
	ui->TrigetModeComboBox->blockSignals(true);
	ui->TrigetModeComboBox->setCurrentIndex(nTriggerModeIndex);
	ui->TrigetModeComboBox->blockSignals(false);

	int nTriggerMethodIndex = 0;
	nRet = KSJ_TriggerMethodGet(m_nCamareIndex, (KSJ_TRIGGERMETHOD*)&nTriggerMethodIndex);
	ui->TrigetMethodComboBox->blockSignals(true);
	ui->TrigetMethodComboBox->setCurrentIndex(nTriggerMethodIndex);
	ui->TrigetMethodComboBox->blockSignals(false);


	float fFrameRate = 0;
	nRet = KSJ_GetFixedFrameRateEx(m_nCamareIndex, &fFrameRate);
	ui->FixFrameRateDoubleSpinBox->setValue(fFrameRate);

	ui->FixFrameRateDoubleSpinBox->setEnabled((KSJ_TRIGGERMODE(nTriggerModeIndex)) == KSJ_TRIGGER_FIXFRAMERATE);

#ifdef OLD_KSJAPI
	unsigned long nCaptureTimeout = 0;
#else
	unsigned int nCaptureTimeout = 0;
#endif
	nRet = KSJ_CaptureGetTimeOut(m_nCamareIndex, &nCaptureTimeout);
	ui->CaptureTimeoutSpinBox->setValue(nCaptureTimeout);

	int nColStart;
	int nRowStart;
	int nColSize;
	int nRowSize;
	unsigned short nsMultiFrameNum;
	KSJ_ADDRESSMODE AmCol;
	KSJ_ADDRESSMODE AmRow;

	nRet = KSJ_CaptureGetFieldOfViewEx(m_nCamareIndex, &nColStart, &nRowStart, &nColSize, &nRowSize, &AmCol, &AmRow, &nsMultiFrameNum);

	ui->ColStartSpinBox->blockSignals(true);
	ui->ColStartSpinBox->setValue(nColStart);
	ui->ColStartSpinBox->blockSignals(false);

	ui->ColSizeSpinBox->blockSignals(true);
	ui->ColSizeSpinBox->setValue(nRowStart);
	ui->ColSizeSpinBox->blockSignals(false);

	ui->RowStartSpinBox->blockSignals(true);
	ui->RowStartSpinBox->setValue(nColSize);
	ui->RowStartSpinBox->blockSignals(false);

	ui->RowSizeSpinBox->blockSignals(true);
	ui->RowSizeSpinBox->setValue(nRowSize);
	ui->RowSizeSpinBox->blockSignals(false);

	ui->MultiFrameNumberSpinBox->blockSignals(true);
	ui->MultiFrameNumberSpinBox->setValue(nsMultiFrameNum);
	ui->MultiFrameNumberSpinBox->blockSignals(false);

	ui->SkipComboBox->blockSignals(true);
	ui->SkipComboBox->setCurrentIndex((int)AmCol);
	ui->SkipComboBox->blockSignals(false);

	int nExposureLines = 0;
	nRet = KSJ_GetParam(m_nCamareIndex, KSJ_EXPOSURE_LINES, &nExposureLines);

	ui->ExpoureLineSpinBox->blockSignals(true);
	ui->ExpoureLineSpinBox->setValue(nExposureLines);
	ui->ExpoureLineSpinBox->blockSignals(false);

	float fExposureTime = 0;
	nRet = KSJ_ExposureTimeGet(m_nCamareIndex, &fExposureTime);
	ui->ExposureTimeSpinBox->blockSignals(true);
	ui->ExposureTimeSpinBox->setValue(fExposureTime);
	ui->ExposureTimeSpinBox->blockSignals(false);

	int nMirror = 0;
	KSJ_GetParam(m_nCamareIndex, KSJ_MIRROR, &nMirror);

	ui->MirrorCheckBox->blockSignals(true);
	ui->MirrorCheckBox->setChecked(nMirror!=0);
	ui->MirrorCheckBox->blockSignals(false);

	int nFlip = 0;
	nRet = KSJ_GetParam(m_nCamareIndex, KSJ_FLIP, &nFlip);

	ui->FlipCheckBox->blockSignals(true);
	ui->FlipCheckBox->setChecked(nFlip != 0);
	ui->FlipCheckBox->blockSignals(false);

	KSJ_SENSITIVITYMODE SensitivityMode = KSJ_LOW;
	nRet = KSJ_SensitivityGetMode(m_nCamareIndex, &SensitivityMode);

	ui->SensitivityComboBox->blockSignals(true);
	ui->SensitivityComboBox->setCurrentIndex((int)SensitivityMode);
	ui->SensitivityComboBox->blockSignals(false);

	int nValue = 0;
	nRet = KSJ_GetParam(m_nCamareIndex, KSJ_RED, &nValue);

	ui->GainRedSpinBox->blockSignals(true);
	ui->GainRedSpinBox->setValue((int)nValue);
	ui->GainRedSpinBox->blockSignals(false);

	nRet = KSJ_GetParam(m_nCamareIndex, KSJ_GREEN, &nValue);

	ui->GainGreenSpinBox->blockSignals(true);
	ui->GainGreenSpinBox->setValue((int)nValue);
	ui->GainGreenSpinBox->blockSignals(false);

	nRet = KSJ_GetParam(m_nCamareIndex, KSJ_BLUE, &nValue);

	ui->GainBlueSpinBox->blockSignals(true);
	ui->GainBlueSpinBox->setValue((int)nValue);
	ui->GainBlueSpinBox->blockSignals(false);

	nRet = KSJ_WhiteBalanceGet(m_nCamareIndex, (KSJ_WB_MODE*)&nValue);
	ui->WBModeComboBox->blockSignals(true);
	ui->WBModeComboBox->setCurrentIndex(nValue);
	ui->WBModeComboBox->blockSignals(false);

	KSJ_WhiteBalancePresettingGet(m_nCamareIndex, (KSJ_COLOR_TEMPRATURE*)&nValue);
	ui->WBPresettingComboBox->blockSignals(true);
	ui->WBPresettingComboBox->setCurrentIndex(nValue);
	ui->WBPresettingComboBox->blockSignals(false);

	UpdateWhiteBalanceMatrix();

	KSJ_ColorCorrectionGet(m_nCamareIndex, (KSJ_CCM_MODE*)&nValue);
	ui->CCMModeComboBox->blockSignals(true);
	ui->CCMModeComboBox->setCurrentIndex(nValue);
	ui->CCMModeComboBox->blockSignals(false);

	KSJ_ColorCorrectionPresettingGet(m_nCamareIndex, (KSJ_COLOR_TEMPRATURE*)&nValue);
	ui->CCMPresettingComboBox->blockSignals(true);
	ui->CCMPresettingComboBox->setCurrentIndex(nValue);
	ui->CCMPresettingComboBox->blockSignals(false);

	UpdateColorCorrectionMatrix();
}

void __stdcall WBACALLBACK(float fMatrix[3], void *lpContext)
{
	((CKSJSCZDemoMainWindow *)lpContext)->WBACallback(fMatrix);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////自动白平衡
void CKSJSCZDemoMainWindow::WBACallback(float fMatrix[3])
{
	emit sigWBADone(fMatrix[0], fMatrix[1], fMatrix[2]);
}


void CKSJSCZDemoMainWindow::on_WBModeComboBox_currentIndexChanged(int index)
{
	if (m_nCamareIndex == -1) return;

	KSJ_WhiteBalanceAutoSetCallBack(m_nCamareIndex, WBACALLBACK, this);

	KSJ_WhiteBalanceSet(m_nCamareIndex, (KSJ_WB_MODE)index);

	int nValue;
	KSJ_WhiteBalanceGet(m_nCamareIndex, (KSJ_WB_MODE*)&nValue);
	ui->WBModeComboBox->blockSignals(true);
	ui->WBModeComboBox->setCurrentIndex(nValue);
	ui->WBModeComboBox->blockSignals(false);

	UpdateWhiteBalanceMatrix();
}

void CKSJSCZDemoMainWindow::on_WBPresettingComboBox_currentIndexChanged(int index)
{
	if (m_nCamareIndex == -1) return;

	KSJ_WhiteBalancePresettingSet(m_nCamareIndex, (KSJ_COLOR_TEMPRATURE)index);

	int nValue;
	KSJ_WhiteBalancePresettingGet(m_nCamareIndex, (KSJ_COLOR_TEMPRATURE*)&nValue);
	ui->WBPresettingComboBox->blockSignals(true);
	ui->WBPresettingComboBox->setCurrentIndex(nValue);
	ui->WBPresettingComboBox->blockSignals(false);

	UpdateWhiteBalanceMatrix();
}

void CKSJSCZDemoMainWindow::on_WBPHiSpinBox_valueChanged(int value)
{
	if (m_nCamareIndex == -1) return;

	KSJ_WhiteBalanceAutoSet(m_nCamareIndex, value);
}

void CKSJSCZDemoMainWindow::on_WBRedSpinBox_valueChanged(double value)
{
	if (m_nCamareIndex == -1) return;

	float fMatrix[3];

	fMatrix[0] = ui->WBRedSpinBox->value();
	fMatrix[1] = ui->WBGreenSpinBox->value();
	fMatrix[2] = ui->WBBlueSpinBox->value();

	KSJ_WhiteBalanceMatrixSet(m_nCamareIndex, fMatrix);

}

void CKSJSCZDemoMainWindow::on_WBGreenSpinBox_valueChanged(double value)
{
	if (m_nCamareIndex == -1) return;

	float fMatrix[3];

	fMatrix[0] = ui->WBRedSpinBox->value();
	fMatrix[1] = ui->WBGreenSpinBox->value();
	fMatrix[2] = ui->WBBlueSpinBox->value();

	KSJ_WhiteBalanceMatrixSet(m_nCamareIndex, fMatrix);
}

void CKSJSCZDemoMainWindow::on_WBBlueSpinBox_valueChanged(double value)
{
	if (m_nCamareIndex == -1) return;

	float fMatrix[3];

	fMatrix[0] = ui->WBRedSpinBox->value();
	fMatrix[1] = ui->WBGreenSpinBox->value();
	fMatrix[2] = ui->WBBlueSpinBox->value();

	KSJ_WhiteBalanceMatrixSet(m_nCamareIndex, fMatrix);
}

void CKSJSCZDemoMainWindow::on_ProgramWBMatrixPushButton_clicked()
{
	if (m_nCamareIndex == -1) return;

	float fMatrix[3];

	fMatrix[0] = ui->WBRedSpinBox->value();
	fMatrix[1] = ui->WBGreenSpinBox->value();
	fMatrix[2] = ui->WBBlueSpinBox->value();

	KSJ_HWBMatrixProgram(m_nCamareIndex, fMatrix);
}

void CKSJSCZDemoMainWindow::OnWBADone(float r, float g, float b)
{
	ui->WBRedSpinBox->blockSignals(true);
	ui->WBGreenSpinBox->blockSignals(true);
	ui->WBBlueSpinBox->blockSignals(true);
	ui->WBRedSpinBox->setValue(r);
	ui->WBGreenSpinBox->setValue(g);
	ui->WBBlueSpinBox->setValue(b);
	ui->WBRedSpinBox->blockSignals(false);
	ui->WBGreenSpinBox->blockSignals(false);
	ui->WBBlueSpinBox->blockSignals(false);
}

void CKSJSCZDemoMainWindow::EnableWhiteBalanceMatrixControls(bool bEnable)
{
	ui->WBRedSpinBox->setEnabled(bEnable);
	ui->WBGreenSpinBox->setEnabled(bEnable);
	ui->WBBlueSpinBox->setEnabled(bEnable);
}

void CKSJSCZDemoMainWindow::UpdateWhiteBalanceMatrix()
{
	if (m_nCamareIndex == -1) return;

	int nWbMode = ui->WBModeComboBox->currentIndex();

	if (nWbMode == KSJ_WB_DISABLE)                EnableWhiteBalanceMatrixControls(false);
	else if (nWbMode == KSJ_SWB_AUTO_ONCE)        EnableWhiteBalanceMatrixControls(false);
	else if (nWbMode == KSJ_SWB_AUTO_CONITNUOUS)  EnableWhiteBalanceMatrixControls(false);
	else if (nWbMode == KSJ_SWB_PRESETTINGS)      EnableWhiteBalanceMatrixControls(false);
	else if (nWbMode == KSJ_SWB_MANUAL)           EnableWhiteBalanceMatrixControls(true);
	else if (nWbMode == KSJ_HWB_AUTO_ONCE)        EnableWhiteBalanceMatrixControls(false);
	else if (nWbMode == KSJ_HWB_AUTO_CONITNUOUS)  EnableWhiteBalanceMatrixControls(false);
	else if (nWbMode == KSJ_HWB_PRESETTINGS)      EnableWhiteBalanceMatrixControls(false);
	else if (nWbMode == KSJ_HWB_MANUAL)           EnableWhiteBalanceMatrixControls(true);

	if (nWbMode == KSJ_WB_DISABLE)                ui->WBPresettingComboBox->setEnabled(false);
	else if (nWbMode == KSJ_SWB_AUTO_ONCE)        ui->WBPresettingComboBox->setEnabled(false);
	else if (nWbMode == KSJ_SWB_AUTO_CONITNUOUS)  ui->WBPresettingComboBox->setEnabled(false);
	else if (nWbMode == KSJ_SWB_PRESETTINGS)      ui->WBPresettingComboBox->setEnabled(true);
	else if (nWbMode == KSJ_SWB_MANUAL)           ui->WBPresettingComboBox->setEnabled(false);
	else if (nWbMode == KSJ_HWB_AUTO_ONCE)        ui->WBPresettingComboBox->setEnabled(false);
	else if (nWbMode == KSJ_HWB_AUTO_CONITNUOUS)  ui->WBPresettingComboBox->setEnabled(false);
	else if (nWbMode == KSJ_HWB_PRESETTINGS)      ui->WBPresettingComboBox->setEnabled(true);
	else if (nWbMode == KSJ_HWB_MANUAL)           ui->WBPresettingComboBox->setEnabled(false);

	if (nWbMode == KSJ_WB_DISABLE)
	{
		ui->WBPHiSpinBox->setEnabled(false);
	}
	else if (nWbMode == KSJ_SWB_AUTO_ONCE)
	{
		ui->WBPHiSpinBox->setEnabled(true);
	}
	else if (nWbMode == KSJ_SWB_AUTO_CONITNUOUS)
	{
		ui->WBPHiSpinBox->setEnabled(true);
	}
	else if (nWbMode == KSJ_SWB_PRESETTINGS)
	{
		ui->WBPHiSpinBox->setEnabled(false);
	}
	else if (nWbMode == KSJ_SWB_MANUAL)
	{
		ui->WBPHiSpinBox->setEnabled(false);
	}
	else if (nWbMode == KSJ_HWB_AUTO_ONCE)
	{
		ui->WBPHiSpinBox->setEnabled(false);
	}
	else if (nWbMode == KSJ_HWB_AUTO_CONITNUOUS)
	{
		ui->WBPHiSpinBox->setEnabled(false);
	}
	else if (nWbMode == KSJ_HWB_MANUAL)
	{
		ui->WBPHiSpinBox->setEnabled(false);
	}

	float fWBMatrix[3];
	KSJ_WhiteBalanceMatrixGet(m_nCamareIndex, fWBMatrix);

	ui->WBRedSpinBox->blockSignals(true);
	ui->WBGreenSpinBox->blockSignals(true);
	ui->WBBlueSpinBox->blockSignals(true);
	ui->WBRedSpinBox->setValue(fWBMatrix[0]);
	ui->WBGreenSpinBox->setValue(fWBMatrix[1]);
	ui->WBBlueSpinBox->setValue(fWBMatrix[2]);
	ui->WBRedSpinBox->blockSignals(false);
	ui->WBGreenSpinBox->blockSignals(false);
	ui->WBBlueSpinBox->blockSignals(false);
}

void CKSJSCZDemoMainWindow::on_CCMModeComboBox_currentIndexChanged(int index)
{
	if (m_nCamareIndex == -1) return;

	KSJ_ColorCorrectionSet(m_nCamareIndex, (KSJ_CCM_MODE)index);

	int nValue;
	KSJ_ColorCorrectionGet(m_nCamareIndex, (KSJ_CCM_MODE*)&nValue);
	ui->CCMModeComboBox->blockSignals(true);
	ui->CCMModeComboBox->setCurrentIndex(nValue);
	ui->CCMModeComboBox->blockSignals(false);

	UpdateColorCorrectionMatrix();
}

void CKSJSCZDemoMainWindow::on_CCMPresettingComboBox_currentIndexChanged(int index)
{
	if (m_nCamareIndex == -1) return;

	KSJ_ColorCorrectionPresettingSet(m_nCamareIndex, (KSJ_COLOR_TEMPRATURE)index);

	int nValue;
	KSJ_ColorCorrectionPresettingGet(m_nCamareIndex, (KSJ_COLOR_TEMPRATURE*)&nValue);
	ui->CCMPresettingComboBox->blockSignals(true);
	ui->CCMPresettingComboBox->setCurrentIndex(nValue);
	ui->CCMPresettingComboBox->blockSignals(false);

	UpdateColorCorrectionMatrix();
}

void CKSJSCZDemoMainWindow::on_CCM00SpinBox_valueChanged(double)
{
	if (m_nCamareIndex == -1) return;

	float fMatrix[3][3];

	fMatrix[0][0] = ui->CCM00SpinBox->value();
	fMatrix[0][1] = ui->CCM01SpinBox->value();
	fMatrix[0][2] = ui->CCM02SpinBox->value();
	fMatrix[1][0] = ui->CCM10SpinBox->value();
	fMatrix[1][1] = ui->CCM11SpinBox->value();
	fMatrix[1][2] = ui->CCM12SpinBox->value();
	fMatrix[2][0] = ui->CCM20SpinBox->value();
	fMatrix[2][1] = ui->CCM21SpinBox->value();
	fMatrix[2][2] = ui->CCM22SpinBox->value();

	KSJ_ColorCorrectionMatrixSet(m_nCamareIndex, fMatrix);
}

void CKSJSCZDemoMainWindow::on_CCM01SpinBox_valueChanged(double)
{
	if (m_nCamareIndex == -1) return;

	float fMatrix[3][3];

	fMatrix[0][0] = ui->CCM00SpinBox->value();
	fMatrix[0][1] = ui->CCM01SpinBox->value();
	fMatrix[0][2] = ui->CCM02SpinBox->value();
	fMatrix[1][0] = ui->CCM10SpinBox->value();
	fMatrix[1][1] = ui->CCM11SpinBox->value();
	fMatrix[1][2] = ui->CCM12SpinBox->value();
	fMatrix[2][0] = ui->CCM20SpinBox->value();
	fMatrix[2][1] = ui->CCM21SpinBox->value();
	fMatrix[2][2] = ui->CCM22SpinBox->value();

	KSJ_ColorCorrectionMatrixSet(m_nCamareIndex, fMatrix);
}

void CKSJSCZDemoMainWindow::on_CCM02SpinBox_valueChanged(double)
{
	if (m_nCamareIndex == -1) return;

	float fMatrix[3][3];

	fMatrix[0][0] = ui->CCM00SpinBox->value();
	fMatrix[0][1] = ui->CCM01SpinBox->value();
	fMatrix[0][2] = ui->CCM02SpinBox->value();
	fMatrix[1][0] = ui->CCM10SpinBox->value();
	fMatrix[1][1] = ui->CCM11SpinBox->value();
	fMatrix[1][2] = ui->CCM12SpinBox->value();
	fMatrix[2][0] = ui->CCM20SpinBox->value();
	fMatrix[2][1] = ui->CCM21SpinBox->value();
	fMatrix[2][2] = ui->CCM22SpinBox->value();

	KSJ_ColorCorrectionMatrixSet(m_nCamareIndex, fMatrix);
}

void CKSJSCZDemoMainWindow::on_CCM10SpinBox_valueChanged(double)
{
	if (m_nCamareIndex == -1) return;

	float fMatrix[3][3];

	fMatrix[0][0] = ui->CCM00SpinBox->value();
	fMatrix[0][1] = ui->CCM01SpinBox->value();
	fMatrix[0][2] = ui->CCM02SpinBox->value();
	fMatrix[1][0] = ui->CCM10SpinBox->value();
	fMatrix[1][1] = ui->CCM11SpinBox->value();
	fMatrix[1][2] = ui->CCM12SpinBox->value();
	fMatrix[2][0] = ui->CCM20SpinBox->value();
	fMatrix[2][1] = ui->CCM21SpinBox->value();
	fMatrix[2][2] = ui->CCM22SpinBox->value();

	KSJ_ColorCorrectionMatrixSet(m_nCamareIndex, fMatrix);
}

void CKSJSCZDemoMainWindow::on_CCM11SpinBox_valueChanged(double)
{
	if (m_nCamareIndex == -1) return;

	float fMatrix[3][3];

	fMatrix[0][0] = ui->CCM00SpinBox->value();
	fMatrix[0][1] = ui->CCM01SpinBox->value();
	fMatrix[0][2] = ui->CCM02SpinBox->value();
	fMatrix[1][0] = ui->CCM10SpinBox->value();
	fMatrix[1][1] = ui->CCM11SpinBox->value();
	fMatrix[1][2] = ui->CCM12SpinBox->value();
	fMatrix[2][0] = ui->CCM20SpinBox->value();
	fMatrix[2][1] = ui->CCM21SpinBox->value();
	fMatrix[2][2] = ui->CCM22SpinBox->value();

	KSJ_ColorCorrectionMatrixSet(m_nCamareIndex, fMatrix);
}

void CKSJSCZDemoMainWindow::on_CCM12SpinBox_valueChanged(double)
{
	if (m_nCamareIndex == -1) return;

	float fMatrix[3][3];

	fMatrix[0][0] = ui->CCM00SpinBox->value();
	fMatrix[0][1] = ui->CCM01SpinBox->value();
	fMatrix[0][2] = ui->CCM02SpinBox->value();
	fMatrix[1][0] = ui->CCM10SpinBox->value();
	fMatrix[1][1] = ui->CCM11SpinBox->value();
	fMatrix[1][2] = ui->CCM12SpinBox->value();
	fMatrix[2][0] = ui->CCM20SpinBox->value();
	fMatrix[2][1] = ui->CCM21SpinBox->value();
	fMatrix[2][2] = ui->CCM22SpinBox->value();

	KSJ_ColorCorrectionMatrixSet(m_nCamareIndex, fMatrix);
}

void CKSJSCZDemoMainWindow::on_CCM20SpinBox_valueChanged(double)
{
	if (m_nCamareIndex == -1) return;

	float fMatrix[3][3];

	fMatrix[0][0] = ui->CCM00SpinBox->value();
	fMatrix[0][1] = ui->CCM01SpinBox->value();
	fMatrix[0][2] = ui->CCM02SpinBox->value();
	fMatrix[1][0] = ui->CCM10SpinBox->value();
	fMatrix[1][1] = ui->CCM11SpinBox->value();
	fMatrix[1][2] = ui->CCM12SpinBox->value();
	fMatrix[2][0] = ui->CCM20SpinBox->value();
	fMatrix[2][1] = ui->CCM21SpinBox->value();
	fMatrix[2][2] = ui->CCM22SpinBox->value();

	KSJ_ColorCorrectionMatrixSet(m_nCamareIndex, fMatrix);
}

void CKSJSCZDemoMainWindow::on_CCM21SpinBox_valueChanged(double)
{
	if (m_nCamareIndex == -1) return;

	float fMatrix[3][3];

	fMatrix[0][0] = ui->CCM00SpinBox->value();
	fMatrix[0][1] = ui->CCM01SpinBox->value();
	fMatrix[0][2] = ui->CCM02SpinBox->value();
	fMatrix[1][0] = ui->CCM10SpinBox->value();
	fMatrix[1][1] = ui->CCM11SpinBox->value();
	fMatrix[1][2] = ui->CCM12SpinBox->value();
	fMatrix[2][0] = ui->CCM20SpinBox->value();
	fMatrix[2][1] = ui->CCM21SpinBox->value();
	fMatrix[2][2] = ui->CCM22SpinBox->value();

	KSJ_ColorCorrectionMatrixSet(m_nCamareIndex, fMatrix);
}

void CKSJSCZDemoMainWindow::on_CCM22SpinBox_valueChanged(double)
{
	if (m_nCamareIndex == -1) return;

	float fMatrix[3][3];

	fMatrix[0][0] = ui->CCM00SpinBox->value();
	fMatrix[0][1] = ui->CCM01SpinBox->value();
	fMatrix[0][2] = ui->CCM02SpinBox->value();
	fMatrix[1][0] = ui->CCM10SpinBox->value();
	fMatrix[1][1] = ui->CCM11SpinBox->value();
	fMatrix[1][2] = ui->CCM12SpinBox->value();
	fMatrix[2][0] = ui->CCM20SpinBox->value();
	fMatrix[2][1] = ui->CCM21SpinBox->value();
	fMatrix[2][2] = ui->CCM22SpinBox->value();

	KSJ_ColorCorrectionMatrixSet(m_nCamareIndex, fMatrix);
}

void CKSJSCZDemoMainWindow::on_ProgramCCMMatrixPushButton_clicked()
{
	if (m_nCamareIndex == -1) return;

	float fMatrix[3][3];

	fMatrix[0][0] = ui->CCM00SpinBox->value();
	fMatrix[0][1] = ui->CCM01SpinBox->value();
	fMatrix[0][2] = ui->CCM02SpinBox->value();
	fMatrix[1][0] = ui->CCM10SpinBox->value();
	fMatrix[1][1] = ui->CCM11SpinBox->value();
	fMatrix[1][2] = ui->CCM12SpinBox->value();
	fMatrix[2][0] = ui->CCM20SpinBox->value();
	fMatrix[2][1] = ui->CCM21SpinBox->value();
	fMatrix[2][2] = ui->CCM22SpinBox->value();

	KSJ_HCCMMatrixProgram(m_nCamareIndex, fMatrix);
}

void CKSJSCZDemoMainWindow::UpdateColorCorrectionMatrix()
{
	if (m_nCamareIndex == -1) return;

	int nCcmMode = ui->CCMModeComboBox->currentIndex();

	if (nCcmMode == KSJ_CCM_DISABLE)           EnableColorCorrectionMatrixControls(false);
	else if (nCcmMode == KSJ_SCCM_PRESETTINGS) EnableColorCorrectionMatrixControls(false);
	else if (nCcmMode == KSJ_SCCM_MANUAL)      EnableColorCorrectionMatrixControls(true);
	else if (nCcmMode == KSJ_HCCM_PRESETTINGS) EnableColorCorrectionMatrixControls(false);
	else if (nCcmMode == KSJ_HCCM_MANUAL)      EnableColorCorrectionMatrixControls(true);

	if (nCcmMode == KSJ_CCM_DISABLE)           ui->CCMPresettingComboBox->setEnabled(false);
	else if (nCcmMode == KSJ_SCCM_PRESETTINGS) ui->CCMPresettingComboBox->setEnabled(true);
	else if (nCcmMode == KSJ_SCCM_MANUAL)      ui->CCMPresettingComboBox->setEnabled(false);
	else if (nCcmMode == KSJ_HCCM_PRESETTINGS) ui->CCMPresettingComboBox->setEnabled(true);
	else if (nCcmMode == KSJ_HCCM_MANUAL)      ui->CCMPresettingComboBox->setEnabled(false);

	float fMatrix[3][3];
	KSJ_ColorCorrectionMatrixGet(m_nCamareIndex, fMatrix);

	ui->CCM00SpinBox->blockSignals(true);
	ui->CCM01SpinBox->blockSignals(true);
	ui->CCM02SpinBox->blockSignals(true);
	ui->CCM10SpinBox->blockSignals(true);
	ui->CCM11SpinBox->blockSignals(true);
	ui->CCM12SpinBox->blockSignals(true);
	ui->CCM20SpinBox->blockSignals(true);
	ui->CCM21SpinBox->blockSignals(true);
	ui->CCM22SpinBox->blockSignals(true);
	ui->CCM00SpinBox->setValue(fMatrix[0][0]);
	ui->CCM01SpinBox->setValue(fMatrix[0][1]);
	ui->CCM02SpinBox->setValue(fMatrix[0][2]);
	ui->CCM10SpinBox->setValue(fMatrix[1][0]);
	ui->CCM11SpinBox->setValue(fMatrix[1][1]);
	ui->CCM12SpinBox->setValue(fMatrix[1][2]);
	ui->CCM20SpinBox->setValue(fMatrix[2][0]);
	ui->CCM21SpinBox->setValue(fMatrix[2][1]);
	ui->CCM22SpinBox->setValue(fMatrix[2][2]);
	ui->CCM00SpinBox->blockSignals(false);
	ui->CCM01SpinBox->blockSignals(false);
	ui->CCM02SpinBox->blockSignals(false);
	ui->CCM10SpinBox->blockSignals(false);
	ui->CCM11SpinBox->blockSignals(false);
	ui->CCM12SpinBox->blockSignals(false);
	ui->CCM20SpinBox->blockSignals(false);
	ui->CCM21SpinBox->blockSignals(false);
	ui->CCM22SpinBox->blockSignals(false);
}

void CKSJSCZDemoMainWindow::EnableColorCorrectionMatrixControls(bool bEnable)
{
	ui->CCM00SpinBox->setEnabled(bEnable);
	ui->CCM01SpinBox->setEnabled(bEnable);
	ui->CCM02SpinBox->setEnabled(bEnable);
	ui->CCM10SpinBox->setEnabled(bEnable);
	ui->CCM11SpinBox->setEnabled(bEnable);
	ui->CCM12SpinBox->setEnabled(bEnable);
	ui->CCM20SpinBox->setEnabled(bEnable);
	ui->CCM21SpinBox->setEnabled(bEnable);
	ui->CCM22SpinBox->setEnabled(bEnable);
}

void CKSJSCZDemoMainWindow::on_PragramSettingsPushButton_clicked()
{
	if (m_nCamareIndex == -1) return;

	bool bIsCapturing = this->StopPreview();

	int nRet = KSJ_ParamProgram(m_nCamareIndex);

	if (bIsCapturing) this->StartPreview();

    TCHAR szErrorInfo[512] = { '\0' };
	KSJ_GetErrorInfo(nRet, szErrorInfo);
	QMessageBox::about(this, "CatchBEST", szErrorInfo);
}

void CKSJSCZDemoMainWindow::on_ClearSettingsPushButton_clicked()
{
	if (m_nCamareIndex == -1) return;

	bool bIsCapturing = this->StopPreview();

	int nRet = KSJ_ParamErase(m_nCamareIndex);

	if (bIsCapturing) this->StartPreview();

    TCHAR szErrorInfo[512] = { '\0' };
	KSJ_GetErrorInfo(nRet, szErrorInfo);
	QMessageBox::about(this, "CatchBEST", szErrorInfo);
}

static QVector<QRgb> grayTable;

void CKSJSCZDemoMainWindow::ProcessCaptureData(unsigned char* pImageData, int w, int h, int bc)
{
	if (bc != 8 && bc != 24) return;

	// 如果图像大小有改变，把老的m_pImage删除掉
	if (m_pImage != NULL && (m_pImage->width() != w || m_pImage->height() != h))
	{
		delete m_pImage;
		m_pImage = NULL;
	}

	if (bc == 8)
	{
		if (m_pImage == NULL)
		{

			m_pImage = new QImage(w, h, QImage::Format_Indexed8);

			if (grayTable.size() <= 0)
			{
				for (int i = 0; i < 256; i++) grayTable.push_back(qRgb(i, i, i));
			}

			m_pImage->setColorTable(grayTable);
		}

		memcpy(m_pImage->bits(), pImageData, w*h*bc / 8);
	}
	else if (bc ==24)
	{
		if (m_pImage == NULL)
		{
			m_pImage = new QImage(w, h, QImage::Format_RGB888);
		}

		unsigned char* pData = m_pImage->bits();

		for (int j = 0; j < h; ++j)
		{
			for (int i = 0; i < w; ++i)
			{
                pData[(h - j - 1) * 3 * w + 3 * i + 0] = pImageData[j * 3 * w + 3 * i + 2];
                pData[(h - j - 1) * 3 * w + 3 * i + 1] = pImageData[j * 3 * w + 3 * i + 1];
                pData[(h - j - 1) * 3 * w + 3 * i + 2] = pImageData[j * 3 * w + 3 * i + 0];
			}
		}
	}

	if (m_bSaveImage)
	{
		if (m_pImage->save(m_strImagePath))
		{
			ui->SnapCountLabel->setText(QString::number(++m_nSnapCount));
		}

		m_bSaveImage = false;
	}

	update();
}

void CKSJSCZDemoMainWindow::mousePressEvent(QMouseEvent * e)
{
	if (e->button() == Qt::LeftButton)
	{
	}
}

void CKSJSCZDemoMainWindow::mouseReleaseEvent(QMouseEvent * e)
{
	if (e->button() == Qt::LeftButton)
	{
	}
}

void CKSJSCZDemoMainWindow::mouseMoveEvent(QMouseEvent * e)
{
}

void CKSJSCZDemoMainWindow::mouseDoubleClickEvent(QMouseEvent * e)
{
}

void CKSJSCZDemoMainWindow::wheelEvent(QWheelEvent * event)
{
}

void CKSJSCZDemoMainWindow::on_EmptyBufferPushButton_clicked()
{
	bool bIsCapturing = this->StopPreview();

	int nRet = KSJ_EmptyFrameBuffer(m_nCamareIndex);
	//UpdateDeviceInfo();

	//if (bIsCapturing) this->StartPreview();

	QMessageBox::about(this, "CatchBEST", QString("Result = %1").arg(nRet));
}

void CKSJSCZDemoMainWindow::on_CaptureRGBPushButton_clicked()
{
	bool bIsCapturing = this->StopPreview();

	int nRet;
	int nWidth;
	int nHeight;
	int nBitCount;
	int nBufferSize = 0;
	unsigned char* pImageBuffer = NULL;

	nRet = KSJ_CaptureGetSizeEx(m_nCamareIndex, &nWidth, &nHeight, &nBitCount);

	if (nRet == RET_SUCCESS)
	{
		pImageBuffer = new unsigned char[nWidth*nHeight*nBitCount / 8];

		nRet = KSJ_CaptureRgbData(m_nCamareIndex, pImageBuffer);

		if (nRet == RET_SUCCESS)
		{
			// 采集图像以后，将内存数据转换成QImage数据,这样pImageData的数据就被转移到QImage里面，以后可以自己进行算法操作
			ProcessCaptureData(pImageBuffer, nWidth, nHeight, nBitCount);
		}
	}

	QMessageBox::about(this, "CatchBEST", QString("Result = %1").arg(nRet));
}

void CKSJSCZDemoMainWindow::on_CaptureRawPushButton_clicked()
{
	bool bIsCapturing = this->StopPreview();

	int nRet;
	int nWidth;
	int nHeight;
	int nBitCount;
	int nBufferSize = 0;
	unsigned char* pImageBuffer = NULL;

	nRet = KSJ_CaptureGetSizeEx(m_nCamareIndex, &nWidth, &nHeight, &nBitCount);

	if (nRet == RET_SUCCESS)
	{
		pImageBuffer = new unsigned char[nWidth*nHeight*nBitCount / 8];

		nRet = KSJ_CaptureRawData(m_nCamareIndex, pImageBuffer);

		if (nRet == RET_SUCCESS)
		{
			// 采集图像以后，将内存数据转换成QImage数据,这样pImageData的数据就被转移到QImage里面，以后可以自己进行算法操作
			ProcessCaptureData(pImageBuffer, nWidth, nHeight, nBitCount);
		}
	}

	QMessageBox::about(this, "CatchBEST", QString("Result = %1").arg(nRet));
}


