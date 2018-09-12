#ifndef KSJDEMOQT_BASE_H
#define KSJDEMOQT_BASE_H

#include <QtWidgets/QDialog>
#include "ui_KSJDemoQT_Base.h"
#include "KSJ_GS.H"
#include "KSJApi.h"
#include "KSJPreviewWidget.h"



#define MAX_DEVICE    64
#define NUM_IMGFORMAP 6
struct DEVICEINFO
{
	int                 nIndex;
	KSJ_DEVICETYPE		DeviceType;
	int					nSerials;
    unsigned short		wFirmwareVersion;
    unsigned short      wFpgaVersion;
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

    void on_PreviewButton_clicked();

    void on_SetCaptureFovButton_clicked();

    void on_CaptureButton_clicked();

    void on_CreateMapButton_clicked();

    void on_CaliCheckBox_clicked(bool bState);

private:
    Ui::KSJDemoQT_Base *ui;
	int m_nDeviceNum;
	int m_nDeviceCurSel;
	DEVICEINFO    m_DeviceInfo[MAX_DEVICE];
	QVector<QRgb> m_vcolorTable; //生成灰度颜色表
	bool  m_bPreview;
	QColor m_col;

    CKSJPreviewWidget  *m_pKsjpreviewwidget;

    unsigned char      *m_pImgForMap[NUM_IMGFORMAP];
    CKSJPreviewWidget  *m_pWidgetForMap[NUM_IMGFORMAP];
    int                 m_nIndexForMap;


public:
	void UpdateInterfaceFunction();
	void UpdateInterface();
	void UpdateDeviceList();
	void GetRealExposureTime();
	void ShowErrorInfo(int nRet);
};

#endif // KSJDEMOQT_BASE_H
