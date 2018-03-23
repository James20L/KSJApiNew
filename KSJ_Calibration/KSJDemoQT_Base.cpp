#include "KSJDemoQT_Base.h"
#include "ui_KSJDemoQT_Base.h"
#include"KSJPreviewWidget.h"
#include "KSJPreviewThread.h"
#include <QDebug>
#include "KSJLog.h"
#include "KSJCalibrartion.h"

KSJDemoQT_Base::KSJDemoQT_Base(QWidget *parent) :
    QDialog(parent),
    m_bPreview(false),
    ui(new Ui::KSJDemoQT_Base)
{
    ui->setupUi(this);
    m_pKsjpreviewwidget = NULL;
    KSJ_Init();

    for(int i=0;i<NUM_IMGFORMAP;i++)
    {
        m_pImgForMap[i] = NULL;
        m_pWidgetForMap[i] = NULL;
    }
    m_nIndexForMap=0;

    for(int i=0;i<NUM_IMGFORMAP;i++)
    {
        m_pWidgetForMap[i] = new CKSJPreviewWidget(this);
    }
    for(int i=0;i<3;i++)
    {
        m_pWidgetForMap[i]->setGeometry(511+i*110,401,100,100);
        m_pWidgetForMap[i]->show();
        m_pWidgetForMap[i+3]->setGeometry(511+i*110,401+110,100,100);
        m_pWidgetForMap[i+3]->show();
    }

    UpdateDeviceList();
    UpdateInterfaceFunction();
    QPalette pal = ui->PreviewButton->palette();
    QBrush brush = pal.background();
    m_col = brush.color();
    for (int i = 0; i < 256; i++)
    {
        m_vcolorTable.append(qRgb(i, i, i));
    }


    memset(m_DeviceInfo,0,sizeof(DEVICEINFO)*MAX_DEVICE);

}

KSJDemoQT_Base::~KSJDemoQT_Base()
{
    KSJ_UnInit();
    delete ui;
}


void KSJDemoQT_Base::on_DEVICE_LIST_ComboBox_currentIndexChanged(int index)
{
    m_nDeviceCurSel = index;
    UpdateInterface();
}

void KSJDemoQT_Base::GetRealExposureTime()
{
    float fExposureTimeMs = 0.0f;
    KSJ_ExposureTimeGet(m_nDeviceCurSel, &fExposureTimeMs);

    TCHAR   szExposureTimeMs[32] = { '\0' };
    sprintf(szExposureTimeMs, _T("Exp Time: %0.5f ms"), fExposureTimeMs);
    ui->EXPOSURE_TIME_REAL_Label->setText(szExposureTimeMs);
}

void KSJDemoQT_Base::on_ExposureTimeSpinBox_valueChanged(int arg1)
{
    KSJ_SetParam(m_nDeviceCurSel, KSJ_EXPOSURE, arg1);
    GetRealExposureTime();
}

void KSJDemoQT_Base::on_ExposureLinesSpinBox_valueChanged(int arg1)
{
    KSJ_SetParam(m_nDeviceCurSel, KSJ_EXPOSURE_LINES, arg1);
    GetRealExposureTime();
}

void KSJDemoQT_Base::on_GainSpinBox_valueChanged(int arg1)
{
    KSJ_SetParam(m_nDeviceCurSel, KSJ_RED, arg1);
    KSJ_SetParam(m_nDeviceCurSel, KSJ_GREEN, arg1);
    KSJ_SetParam(m_nDeviceCurSel, KSJ_BLUE, arg1);
}

void KSJDemoQT_Base::on_PreviewButton_clicked()
{
    if (m_nDeviceCurSel == -1)   return;

    this->setModal(false);

    if(m_pKsjpreviewwidget==NULL)
    {
        m_pKsjpreviewwidget= new CKSJPreviewWidget(this);

        m_pKsjpreviewwidget->setGeometry(0,0,ui->PREVIEWWND_Widget->width(),ui->PREVIEWWND_Widget->height());

        m_pKsjpreviewwidget->CreatCaptureThread();

        m_pKsjpreviewwidget->show();

    }else
    {

        m_pKsjpreviewwidget->DestoryCaptureThread();
        m_pKsjpreviewwidget->close();
        delete m_pKsjpreviewwidget;
        m_pKsjpreviewwidget = NULL;

    }
    QPalette pal = ui->PreviewButton->palette();
    if (m_bPreview)
    {
        m_bPreview = false;
        ui->groupBox_2->setEnabled(true);
        pal.setColor(QPalette::Button, m_col);
    }
    else
    {
        m_bPreview = true;
        ui->groupBox_2->setEnabled(false);
        pal.setColor(QPalette::Button, Qt::blue);
    }

    ui->PreviewButton->setPalette(pal);
    ui->PreviewButton->setAutoFillBackground(true);
}

void KSJDemoQT_Base::on_SetCaptureFovButton_clicked()
{
    int nRet =0;
    int nColStart;
    int nRowStart;
    int nColSize;
    int nRowSize;
    unsigned short usMultiFrame;
    KSJ_ADDRESSMODE ColAddressMode = KSJ_SKIPNONE;
    KSJ_ADDRESSMODE RowAddressMode = KSJ_SKIPNONE;

    nColStart = ui->CaptureColStartSpinBox->value();
    nRowStart = ui->CaptureRowStartSpinBox->value();
    nColSize = ui->CaptureColSizeSpinBox->value();
    nRowSize = ui->CaptureRowSizeSpinBox->value();
    usMultiFrame = ui->CaptureMultiFrameSpinBox->value();

    nRet = KSJ_CaptureSetFieldOfView(m_nDeviceCurSel, nColStart, nRowStart, nColSize, nRowSize, ColAddressMode, RowAddressMode);
    ShowErrorInfo(nRet);

    nRet  = KSJ_CaptureGetFieldOfView(m_nDeviceCurSel, &nColStart, &nRowStart, &nColSize, &nRowSize, &ColAddressMode, &RowAddressMode);

    ui->CaptureColStartSpinBox->setValue(nColStart);
    ui->CaptureRowStartSpinBox->setValue(nRowStart);
    ui->CaptureColSizeSpinBox->setValue(nColSize);
    ui->CaptureRowSizeSpinBox->setValue(nRowSize);
    ui->CaptureMultiFrameSpinBox->setValue(usMultiFrame);
}

void KSJDemoQT_Base::on_CaptureButton_clicked()
{
    if(m_pKsjpreviewwidget!=NULL)
    {
        on_PreviewButton_clicked();
    }
    m_nDeviceCurSel = 0;
    if (m_nDeviceCurSel == -1)   return;
    int   nCaptureWidth = 0, nCaptureHeight = 0, nCaptureBitCount = 8;

    int   nRet = KSJ_CaptureGetSizeEx(m_nDeviceCurSel, &nCaptureWidth, &nCaptureHeight, &nCaptureBitCount);
    ShowErrorInfo(nRet);
    unsigned char *pImageData = new unsigned char[nCaptureWidth * nCaptureHeight * (nCaptureBitCount >> 3)];
    nRet = KSJ_CaptureRgbData(m_nDeviceCurSel, pImageData);
    ShowErrorInfo(nRet);
    TCHAR   szElapseTime[32] = { '\0' };
    ui->ELAPSE_TIME_Label->setText(szElapseTime);

    if(m_pImgForMap[m_nIndexForMap]!=NULL)
    {
        delete []m_pImgForMap[m_nIndexForMap];
        m_pImgForMap[m_nIndexForMap]=NULL;
    }
    m_pImgForMap[m_nIndexForMap] = new unsigned char[nCaptureWidth * nCaptureHeight * (nCaptureBitCount >> 3)];
    ConvetData(pImageData, nCaptureWidth, nCaptureHeight, nCaptureBitCount, m_pImgForMap[m_nIndexForMap]);
    m_pWidgetForMap[m_nIndexForMap]->m_nImageH=nCaptureHeight;
    m_pWidgetForMap[m_nIndexForMap]->m_nImageW=nCaptureWidth;
    m_pWidgetForMap[m_nIndexForMap]->m_nBitCount=nCaptureBitCount;
    m_pWidgetForMap[m_nIndexForMap]->On_UpdateSignal(m_pImgForMap[m_nIndexForMap]);
    QImage::Format format;
    if (nCaptureBitCount == 24)
    {
        format = QImage::Format_RGB888;
    }
    else if(nCaptureBitCount == 32)
    {
        format = QImage::Format_RGB32;
    }else
    {

        format = QImage::Format_Indexed8;
    }

    QImage img = QImage(m_pImgForMap[m_nIndexForMap], nCaptureWidth, nCaptureHeight, format);  //封装QImage
    if (nCaptureBitCount == 8) img.setColorTable(m_vcolorTable); //设置颜色表
    QPixmap pixmap = QPixmap::fromImage(img);
    pixmap = pixmap.scaled(ui->PREVIEWWND_Widget->size());
    ui->PREVIEWWND_Widget->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(ui->PREVIEWWND_Widget->backgroundRole(), QBrush(pixmap));
    ui->PREVIEWWND_Widget->setPalette(palette);
    ui->PREVIEWWND_Widget->repaint();
    if (Qt::Checked == ui->SaveCheckBox->checkState())
    {
        KSJ_HelperSaveToBmp(pImageData, nCaptureWidth, nCaptureHeight, nCaptureBitCount, "Capture.bmp");
    }

    if(++m_nIndexForMap==NUM_IMGFORMAP)
    {
        m_nIndexForMap=0;
    }
    delete[] pImageData;
    pImageData = NULL;
}


void KSJDemoQT_Base::UpdateInterfaceFunction()
{
    ui->FunctionTableWidget;

    ui->FunctionTableWidget->setColumnCount(2);
    ui->FunctionTableWidget->setHorizontalHeaderLabels(QStringList() << "Function" << "Support");
    ui->FunctionTableWidget->verticalHeader()->setVisible(false); // 隐藏水平header

    ui->FunctionTableWidget->setColumnWidth(0, 210);
    ui->FunctionTableWidget->setColumnWidth(1, 80);
    ui->FunctionTableWidget->setRowCount(g_nFunction);
    int i = g_nFunction - 1;
    char szSupport[32] = { 0 };

    do{
        ui->FunctionTableWidget->setItem(i, 0, new QTableWidgetItem(QString(QLatin1String(g_szFunction[i]))));
        int nSupport = 0;
        KSJ_QueryFunction(m_nDeviceCurSel, (KSJ_FUNCTION)i, &nSupport);
        sprintf(szSupport, _T("%d"), nSupport);

        ui->FunctionTableWidget->setItem(i, 1, new QTableWidgetItem(QString(QLatin1String(szSupport))));
    } while (i--);
}


void KSJDemoQT_Base::UpdateInterface()
{
    if (m_nDeviceCurSel == -1)   return;

    // Initial Exposure Time
    int nMin = 0;
    int nMax = 0;
    int nCur = 0;

    KSJ_GetParamRange(m_nDeviceCurSel, KSJ_EXPOSURE, &nMin, &nMax);
    KSJ_GetParam(m_nDeviceCurSel, KSJ_EXPOSURE, &nCur);
    ui->ExposureTimeSpinBox->setRange(nMin, nMax);
    ui->ExposureTimeSpinBox->setValue(nCur);
    TCHAR   szText[64] = { '\0' };
    sprintf(szText, _T("%d-%d ms (%0.2fsec, %0.2fmin)"), nMin, nMax, (float)nMin / 1000.0f, (float)nMax / 60000.0f);
    ui->EXPOSURE_TIME_RANGE_Label->setText(szText);

    KSJ_GetParamRange(m_nDeviceCurSel, KSJ_EXPOSURE_LINES, &nMin, &nMax);
    KSJ_GetParam(m_nDeviceCurSel, KSJ_EXPOSURE_LINES, &nCur);
    ui->ExposureLinesSpinBox->setRange(nMin, nMax);
    ui->ExposureLinesSpinBox->setValue(nCur);
    sprintf(szText, _T("%d-%d Lines"), nMin, nMax);
    ui->EXPOSURE_LINES_RANGE_Label->setText(szText);

    KSJ_GetParamRange(m_nDeviceCurSel, KSJ_RED, &nMin, &nMax);
    KSJ_GetParam(m_nDeviceCurSel, KSJ_RED, &nCur);
    ui->GainSpinBox->setRange(nMin, nMax);
    ui->GainSpinBox->setValue(nCur);
    sprintf(szText, _T("%d-%d"), nMin, nMax);
    ui->GAIN_RANGE_Label->setText(szText);

    // FOV
    int nColStart;
    int nRowStart;
    int nColSize;
    int nRowSize;
    unsigned short usMultiFrame;
    KSJ_ADDRESSMODE ColAddressMode;
    KSJ_ADDRESSMODE RowAddressMode;

    KSJ_CaptureGetDefaultFieldOfView(m_nDeviceCurSel, &nColStart, &nRowStart, &nColSize, &nRowSize, &ColAddressMode, &RowAddressMode);
    usMultiFrame =1;
    // Preview, Capture they are same, so you should get one is ok!
    sprintf(szText, _T("%d-%d"), nColSize, nRowSize);
    ui->FOV_RANGE_Label->setText(szText);

    KSJ_CaptureGetDefaultFieldOfView(m_nDeviceCurSel, &nColStart, &nRowStart, &nColSize, &nRowSize, &ColAddressMode, &RowAddressMode);
    ui->CaptureColStartSpinBox->setRange(0, nColSize);
    ui->CaptureColStartSpinBox->setValue(nColStart);
    ui->CaptureRowStartSpinBox->setRange(0, nRowSize);
    ui->CaptureRowStartSpinBox->setValue(nRowStart);
    ui->CaptureColSizeSpinBox->setRange(0, nColSize);
    ui->CaptureColSizeSpinBox->setValue(nColSize);
    ui->CaptureRowSizeSpinBox->setRange(0, nRowSize);
    ui->CaptureRowSizeSpinBox->setValue(nRowSize);
    ui->CaptureMultiFrameSpinBox->setRange(1, 20000);
    ui->CaptureMultiFrameSpinBox->setValue(usMultiFrame);
}


void KSJDemoQT_Base::UpdateDeviceList()
{
    m_nDeviceNum = KSJ_DeviceGetCount();
    if (m_nDeviceNum == 0)
    {
        return;
    }

    ui->DEVICE_LIST_ComboBox->blockSignals(true);
    for (int i = 0; i < m_nDeviceNum; i++)
    {
        m_DeviceInfo[i].nIndex = i;
        KSJ_DeviceGetInformationEx(i, (unsigned short *)&(m_DeviceInfo[i].DeviceType), &(m_DeviceInfo[i].nSerials), &(m_DeviceInfo[i].wFirmwareVersion), &(m_DeviceInfo[i].wFpgaVersion));

        unsigned char btMajVersion = (m_DeviceInfo[i].wFirmwareVersion & 0xFF00) >> 8;		// 得到主版本号
        unsigned char btMinVersion = m_DeviceInfo[i].wFirmwareVersion & 0x00FF;				// 得到副版本号

        unsigned char btFpgaMajVersion = (m_DeviceInfo[i].wFpgaVersion & 0xFF00) >> 8;		// 得到主版本号
        unsigned char btFpgaMinVersion = m_DeviceInfo[i].wFpgaVersion & 0x00FF;				// 得到副版本号

        TCHAR  szMenuItem[256] = { '\0' };
        sprintf(szMenuItem, _T("Index(%d)-Type(%s)-Serials(%d)-FwVer(%d.%d)-FpgaVer(%d.%d)"), i, g_szDeviceType[m_DeviceInfo[i].DeviceType], m_DeviceInfo[i].nSerials, btMajVersion, btMinVersion, btFpgaMajVersion, btFpgaMinVersion);
        ui->DEVICE_LIST_ComboBox->addItem(szMenuItem);
    }

    ui->DEVICE_LIST_ComboBox->blockSignals(false);
    on_DEVICE_LIST_ComboBox_currentIndexChanged(0);
}

void KSJDemoQT_Base::ShowErrorInfo(int nRet)
{
    TCHAR szErrorInfo[256] = { '\0' };
    KSJ_GetErrorInfo(nRet, szErrorInfo);
    ui->ERROR_INFO_Label->setText(szErrorInfo);
}

void KSJDemoQT_Base::on_CaliCheckBox_clicked(bool bState)
{
    if(m_pKsjpreviewwidget)
    {
        m_pKsjpreviewwidget->m_pPreview_Thread->m_bCalibration=bState;
    }
}

void KSJDemoQT_Base::on_CreateMapButton_clicked()
{
    if(m_pImgForMap[0]==NULL)return;
    vector<vector<Point2f> > vecImagePts;
    vector<vector<Point3f> > vecWorldPts;

    int nWidth =m_pWidgetForMap[0]->m_nImageW;
    int nHeight= m_pWidgetForMap[0]->m_nImageH;
    int nBitCount=m_pWidgetForMap[0]->m_nBitCount;
    Size BoardSize(ui->ChessWidthSpinBox->value()-1,ui->ChessHeightSpinBox->value()-1);
    for(int i=0;i<NUM_IMGFORMAP;i++)
    {
        if(m_pImgForMap[i]==NULL)continue;
        if(m_pWidgetForMap[i]->m_nImageW!=nWidth || m_pWidgetForMap[i]->m_nImageH!=nHeight)
        {
            ui->ERROR_INFO_Label->setText("Different size");
            return;
        }
        Mat img(nHeight, nWidth, CV_8UC(nBitCount>>3), m_pImgForMap[i], nWidth*nBitCount>>3);
        KSJ_FindConners(img,BoardSize,vecImagePts,vecWorldPts);
    }
    if(vecImagePts.size()==0)
    {
        ui->ERROR_INFO_Label->setText("Can't find chess board");
        return;
    }
    Mat mapx, mapy;
    KSJ_CalRemapMat(vecImagePts, vecWorldPts, Size(nWidth,nHeight),mapx,mapy);
    KSJ_WriteMaptoFile("Map12", mapx, mapy);
}
