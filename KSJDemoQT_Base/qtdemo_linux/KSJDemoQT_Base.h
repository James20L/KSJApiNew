#ifndef KSJDEMOQT_BASE_H
#define KSJDEMOQT_BASE_H

#include <QtWidgets/QDialog>
#include "ui_KSJDemoQT_Base.h"

#include"KSJPreviewWidget.h"

#include "KSJApi.h"

#include "KSJ_GS.H"
#include "KSJPreviewWidget.h"



#define MAX_DEVICE    64

struct DEVICEINFO
{
	int                 nIndex;
	KSJ_DEVICETYPE		DeviceType;
	int					nSerials;
    unsigned short				wFirmwareVersion;
    unsigned short                wFpgaVersion;
};

class KSJDemoQT_Base : public QDialog
{
    Q_OBJECT

public:
    explicit KSJDemoQT_Base(QWidget *parent = 0);
    ~KSJDemoQT_Base();


private slots:
    void on_DEVICE_LIST_ComboBox_currentIndexChanged(int index);

    void on_ExposureTimeSpinBox_valueChanged(int arg1);

    void on_ExposureLinesSpinBox_valueChanged(int arg1);

    void on_GainSpinBox_valueChanged(int arg1);

    void on_SetPreviewFovButton_clicked();


    void on_PreviewButton_clicked();

    void on_SetCaptureFovButton_clicked();

    void on_CaptureButton_clicked();

private:
    Ui::KSJDemoQT_Base *ui;
	int m_nDeviceNum;
	int m_nDeviceCurSel;
	DEVICEINFO    m_DeviceInfo[MAX_DEVICE];
	QVector<QRgb> m_vcolorTable; //生成灰度颜色表
	bool  m_bPreview;
	QColor m_col;

    CKSJPreviewWidget  *m_pKsjpreviewwidget;


public:
	void UpdateInterfaceFunction();
	void UpdateInterface();
	void UpdateDeviceList();
	void GetRealExposureTime();
	void ConvetData(unsigned char *pData, int nWidth, int nHeight, int nBitCount, unsigned char* pConvertData);

	void ShowErrorInfo(int nRet);
};

#endif // KSJDEMOQT_BASE_H
