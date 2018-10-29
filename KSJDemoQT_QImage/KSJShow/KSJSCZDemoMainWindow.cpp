

#include <QPainter>
#include <QResizeEvent>
#include <QScrollBar>
#include <QSettings>
#include <QDateTime>

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
, m_nSaveIndex(0)
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

	ui->TrigetComboBox->addItem("Internal");
	ui->TrigetComboBox->addItem("External");
	ui->TrigetComboBox->addItem("Software");
	ui->TrigetComboBox->addItem("Fixed Frame Rate");

	// 建立信号和槽的关联
	connect(ui->CapturePushButton, SIGNAL(clicked()), this, SLOT(OnCapture()));
	connect(ui->RefreshPushButton, SIGNAL(clicked()), this, SLOT(OnRefreshDevice()));
	connect(ui->SaveImagePushButton, SIGNAL(clicked()), this, SLOT(OnSaveImagePushButton()));
	connect(ui->SetFovPushButton, SIGNAL(clicked()), this, SLOT(OnSetFovPushButton()));

	connect(ui->ExitPushButton, SIGNAL(clicked()), qApp, SLOT(quit()));

	connect(ui->DevicesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSelectDevice(int)));
	connect(ui->TrigetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnTrigetModeChanged(int)));

	connect(ui->MirrorCheckBox, SIGNAL(stateChanged(int)), this, SLOT(OnMirrorChkBoxStateChanged(int)));
	connect(ui->FlipCheckBox, SIGNAL(stateChanged(int)), this, SLOT(OnFlipChkBoxStateChanged(int)));

	connect(ui->FixFrameRateDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(OnFixFrameRateDoubleSpinBoxChanged(double)));

	ui->CapturePushButton->setText(m_bIsCapturing ? "Stop" : "Start");

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

		int w = size().width() - 248;
		int h = size().height();

		int iw = m_pImage->width();
		int ih = m_pImage->height();

		float fw = ((float)(w)) / iw;
		float fh = ((float)(h)) / ih;
		float f = fw < fh ? fw : fh;

		int dw = (int)(f*iw);
		int dh = (int)(f*ih);

		painter.drawImage(QRect(232 + 8 + (w - dw) / 2, (h - dh) / 2, dw, dh), *m_pImage, QRect(0, 0, iw, ih));
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

void CKSJSCZDemoMainWindow::OnCapture()
{
	printf("===== %d =====\r\n", m_nCamareIndex);

	if (m_nCamareIndex >= 0)
	{
		// 开始采集
		m_bIsCapturing = !m_bIsCapturing;

		ui->CapturePushButton->setText(m_bIsCapturing ? "Stop" : "Start");
	}
}

void CKSJSCZDemoMainWindow::OnRefreshDevice()
{
	bool bIsCapturing = m_bIsCapturing;
	if (m_bIsCapturing) OnCapture();

	RefreshDevice();

	if (bIsCapturing) OnCapture();
}

void CKSJSCZDemoMainWindow::OnSaveImagePushButton()
{
	++m_nSaveIndex;
	m_strImagePath = m_strImagePreFix + QString("%1.bmp").arg(m_nSaveIndex, 3, 10, QChar('0'));
	m_bSaveImage = true;
}

void CKSJSCZDemoMainWindow::OnSelectDevice(int nIndex)
{
	SelectDevice(nIndex);
}

void CKSJSCZDemoMainWindow::OnTrigetModeChanged(int nIndex)
{
	bool bIsCapturing = m_bIsCapturing;
	if (m_bIsCapturing) OnCapture();

    KSJ_TriggerModeSet(m_nCamareIndex, KSJ_TRIGGERMODE(nIndex));	// 需要先切换到内触发模式，否则检测线程的Capture退不出来

	ui->FixFrameRateDoubleSpinBox->setEnabled((KSJ_TRIGGERMODE(nIndex)) == KSJ_TRIGGER_FIXFRAMERATE);

	if (bIsCapturing) OnCapture();
}

void CKSJSCZDemoMainWindow::OnMirrorChkBoxStateChanged(int value)
{
	bool bIsCapturing = m_bIsCapturing;
	if (m_bIsCapturing) OnCapture();

	int nMirror = (value == Qt::Checked) ? 1 : 0;

	printf("===== camare%d set mirror: %d =====\r\n", m_nCamareIndex, nMirror);
	KSJ_SetParam(m_nCamareIndex, KSJ_MIRROR, nMirror);

	if (bIsCapturing) OnCapture();
}

void CKSJSCZDemoMainWindow::OnFlipChkBoxStateChanged(int value)
{
	bool bIsCapturing = m_bIsCapturing;
	if (m_bIsCapturing) OnCapture();

	int nFlip = (value == Qt::Checked) ? 1 : 0;

	printf("===== camare%d set flip: %d =====\r\n", m_nCamareIndex, nFlip);
	KSJ_SetParam(m_nCamareIndex, KSJ_FLIP, nFlip);

	if (bIsCapturing) OnCapture();
}

void CKSJSCZDemoMainWindow::OnFixFrameRateDoubleSpinBoxChanged(double value)
{
	bool bIsCapturing = m_bIsCapturing;
	if (m_bIsCapturing) OnCapture();

	KSJ_SetFixedFrameRateEx(m_nCamareIndex, value);

	if (bIsCapturing) OnCapture();
}

void CKSJSCZDemoMainWindow::OnSetFovPushButton()
{
	bool bIsCapturing = m_bIsCapturing;
	if (m_bIsCapturing) OnCapture();

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

		KSJ_CaptureGetFieldOfViewEx(m_nCamareIndex, &nColStart, &nRowStart, &nColSize, &nRowSize, &AmCol, &AmRow, &nsMultiFrameNum);

		ui->ColStartSpinBox->setValue(nColStart);
		ui->ColSizeSpinBox->setValue(nRowStart);
		ui->RowStartSpinBox->setValue(nColSize);
		ui->RowSizeSpinBox->setValue(nRowSize);
		ui->MultiFrameNumberSpinBox->setValue(nsMultiFrameNum);
	}

	if (bIsCapturing) OnCapture();

}

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
            strCamare = QString("Camare%1: Serial(%2)-DeviceType(%3)-FW(%4)-PL(%5)").arg(i).arg(pnSerials).arg(pusDeviceType).arg(pusFirmwareVersion).arg(pusFpgaVersion);
		}

        ui->DevicesComboBox->addItem(strCamare);
	}

	SelectDevice(-1);
}

void CKSJSCZDemoMainWindow::SelectDevice(int nIndex)
{
	if (m_bIsCapturing) OnCapture();

	m_nCamareIndex = -1;

	if (ui->DevicesComboBox->count() <= 0) return;

	if (nIndex <= 0) m_nCamareIndex = 0;
	else m_nCamareIndex = nIndex;

	ui->DevicesComboBox->setCurrentIndex(m_nCamareIndex);

	UpdateDeviceInfo();
}

void CKSJSCZDemoMainWindow::UpdateDeviceInfo()
{
    if (m_nCamareIndex < 0) return;
    if (m_bIsCapturing) OnCapture();

	int nRet;

	int nTriggerModeIndex;

	nRet = KSJ_TriggerModeGet(m_nCamareIndex, (KSJ_TRIGGERMODE*)&nTriggerModeIndex);
	if (nRet != RET_SUCCESS) return;
	ui->TrigetComboBox->blockSignals(true);
	ui->TrigetComboBox->setCurrentIndex(nTriggerModeIndex);
	ui->TrigetComboBox->blockSignals(false);

	ui->FixFrameRateDoubleSpinBox->setEnabled((KSJ_TRIGGERMODE(nTriggerModeIndex)) == KSJ_TRIGGER_FIXFRAMERATE);

	int nMirror = 0;
	KSJ_GetParam(m_nCamareIndex, KSJ_MIRROR, &nMirror);
	if (nRet != RET_SUCCESS) return;
	ui->MirrorCheckBox->blockSignals(true);
	ui->MirrorCheckBox->setChecked(nMirror!=0);
	ui->MirrorCheckBox->blockSignals(false);

	int nFlip = 0;
	nRet = KSJ_GetParam(m_nCamareIndex, KSJ_FLIP, &nFlip);
	if (nRet != RET_SUCCESS) return;
	ui->FlipCheckBox->blockSignals(true);
	ui->FlipCheckBox->setChecked(nFlip != 0);
	ui->FlipCheckBox->blockSignals(false);

	int nColStart;
	int nRowStart;
	int nColSize;
	int nRowSize;
	unsigned short nsMultiFrameNum;
	KSJ_ADDRESSMODE AmCol;
	KSJ_ADDRESSMODE AmRow;

	nRet = KSJ_CaptureGetFieldOfViewEx(m_nCamareIndex, &nColStart, &nRowStart, &nColSize, &nRowSize, &AmCol, &AmRow, &nsMultiFrameNum);
	if (nRet != RET_SUCCESS) return;
	ui->ColStartSpinBox->setValue(nColStart);
	ui->ColSizeSpinBox->setValue(nRowStart);
	ui->RowStartSpinBox->setValue(nColSize);
	ui->RowSizeSpinBox->setValue(nRowSize);
	ui->MultiFrameNumberSpinBox->setValue(nsMultiFrameNum);

	float fFrameRate = 0;
	nRet = KSJ_GetFixedFrameRateEx(m_nCamareIndex, &fFrameRate);
	if (nRet != RET_SUCCESS) return;
	ui->FixFrameRateDoubleSpinBox->blockSignals(true);
	ui->FixFrameRateDoubleSpinBox->setValue(nColStart);
	ui->FixFrameRateDoubleSpinBox->blockSignals(false);

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
			ui->MessageInfoLabel->setWordWrap(true);
			ui->MessageInfoLabel->setText("Save successed: " + m_strImagePath);
		}
		else
		{
			ui->MessageInfoLabel->setWordWrap(true);
			ui->MessageInfoLabel->setText("Save failed: " + m_strImagePath);
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

