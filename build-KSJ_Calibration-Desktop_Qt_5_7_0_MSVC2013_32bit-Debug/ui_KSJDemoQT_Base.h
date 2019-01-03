/********************************************************************************
** Form generated from reading UI file 'KSJDemoQT_Base.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KSJDEMOQT_BASE_H
#define UI_KSJDEMOQT_BASE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KSJDemoQT_Base
{
public:
    QWidget *PREVIEWWND_Widget;
    QLabel *DEVICENUM_Label;
    QComboBox *DEVICE_LIST_ComboBox;
    QSpinBox *ExposureLinesSpinBox;
    QSpinBox *GainSpinBox;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QSpinBox *ExposureTimeSpinBox;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *EXPOSURE_TIME_RANGE_Label;
    QLabel *EXPOSURE_LINES_RANGE_Label;
    QLabel *GAIN_RANGE_Label;
    QLabel *EXPOSURE_TIME_REAL_Label;
    QLabel *FOV_RANGE_Label;
    QLabel *label_12;
    QGroupBox *groupBox_2;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *label_21;
    QLabel *label_22;
    QPushButton *SetCaptureFovButton;
    QSpinBox *CaptureColStartSpinBox;
    QSpinBox *CaptureColSizeSpinBox;
    QSpinBox *CaptureRowStartSpinBox;
    QSpinBox *CaptureRowSizeSpinBox;
    QSpinBox *CaptureMultiFrameSpinBox;
    QPushButton *PreviewButton;
    QPushButton *CaptureButton;
    QLabel *ERROR_INFO_Label;
    QLabel *ELAPSE_TIME_Label;
    QTableWidget *FunctionTableWidget;
    QCheckBox *SaveCheckBox;
    QPushButton *CreateMapButton;
    QCheckBox *CaliCheckBox;
    QSpinBox *ChessWidthSpinBox;
    QSpinBox *ChessHeightSpinBox;
    QLabel *ERROR_INFO_Label_2;

    void setupUi(QDialog *KSJDemoQT_Base)
    {
        if (KSJDemoQT_Base->objectName().isEmpty())
            KSJDemoQT_Base->setObjectName(QStringLiteral("KSJDemoQT_Base"));
        KSJDemoQT_Base->resize(1199, 642);
        PREVIEWWND_Widget = new QWidget(KSJDemoQT_Base);
        PREVIEWWND_Widget->setObjectName(QStringLiteral("PREVIEWWND_Widget"));
        PREVIEWWND_Widget->setGeometry(QRect(-10, 0, 501, 561));
        DEVICENUM_Label = new QLabel(KSJDemoQT_Base);
        DEVICENUM_Label->setObjectName(QStringLiteral("DEVICENUM_Label"));
        DEVICENUM_Label->setGeometry(QRect(510, 20, 71, 21));
        DEVICE_LIST_ComboBox = new QComboBox(KSJDemoQT_Base);
        DEVICE_LIST_ComboBox->setObjectName(QStringLiteral("DEVICE_LIST_ComboBox"));
        DEVICE_LIST_ComboBox->setGeometry(QRect(590, 20, 301, 22));
        ExposureLinesSpinBox = new QSpinBox(KSJDemoQT_Base);
        ExposureLinesSpinBox->setObjectName(QStringLiteral("ExposureLinesSpinBox"));
        ExposureLinesSpinBox->setGeometry(QRect(640, 100, 91, 22));
        GainSpinBox = new QSpinBox(KSJDemoQT_Base);
        GainSpinBox->setObjectName(QStringLiteral("GainSpinBox"));
        GainSpinBox->setGeometry(QRect(640, 170, 91, 22));
        label_6 = new QLabel(KSJDemoQT_Base);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(600, 170, 41, 21));
        label_7 = new QLabel(KSJDemoQT_Base);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(520, 60, 111, 21));
        label_8 = new QLabel(KSJDemoQT_Base);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(530, 100, 91, 21));
        ExposureTimeSpinBox = new QSpinBox(KSJDemoQT_Base);
        ExposureTimeSpinBox->setObjectName(QStringLiteral("ExposureTimeSpinBox"));
        ExposureTimeSpinBox->setGeometry(QRect(640, 60, 91, 22));
        label_9 = new QLabel(KSJDemoQT_Base);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(760, 60, 61, 21));
        label_10 = new QLabel(KSJDemoQT_Base);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(760, 100, 41, 21));
        label_11 = new QLabel(KSJDemoQT_Base);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(760, 170, 41, 21));
        EXPOSURE_TIME_RANGE_Label = new QLabel(KSJDemoQT_Base);
        EXPOSURE_TIME_RANGE_Label->setObjectName(QStringLiteral("EXPOSURE_TIME_RANGE_Label"));
        EXPOSURE_TIME_RANGE_Label->setGeometry(QRect(830, 60, 71, 21));
        EXPOSURE_LINES_RANGE_Label = new QLabel(KSJDemoQT_Base);
        EXPOSURE_LINES_RANGE_Label->setObjectName(QStringLiteral("EXPOSURE_LINES_RANGE_Label"));
        EXPOSURE_LINES_RANGE_Label->setGeometry(QRect(830, 100, 71, 21));
        GAIN_RANGE_Label = new QLabel(KSJDemoQT_Base);
        GAIN_RANGE_Label->setObjectName(QStringLiteral("GAIN_RANGE_Label"));
        GAIN_RANGE_Label->setGeometry(QRect(830, 170, 71, 21));
        EXPOSURE_TIME_REAL_Label = new QLabel(KSJDemoQT_Base);
        EXPOSURE_TIME_REAL_Label->setObjectName(QStringLiteral("EXPOSURE_TIME_REAL_Label"));
        EXPOSURE_TIME_REAL_Label->setGeometry(QRect(640, 140, 201, 21));
        FOV_RANGE_Label = new QLabel(KSJDemoQT_Base);
        FOV_RANGE_Label->setObjectName(QStringLiteral("FOV_RANGE_Label"));
        FOV_RANGE_Label->setGeometry(QRect(640, 200, 201, 21));
        label_12 = new QLabel(KSJDemoQT_Base);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(570, 200, 61, 21));
        groupBox_2 = new QGroupBox(KSJDemoQT_Base);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setEnabled(true);
        groupBox_2->setGeometry(QRect(500, 230, 331, 141));
        label_18 = new QLabel(groupBox_2);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(20, 20, 61, 21));
        label_19 = new QLabel(groupBox_2);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(180, 20, 61, 21));
        label_20 = new QLabel(groupBox_2);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(20, 60, 61, 21));
        label_21 = new QLabel(groupBox_2);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(180, 60, 61, 21));
        label_22 = new QLabel(groupBox_2);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(10, 100, 71, 21));
        SetCaptureFovButton = new QPushButton(groupBox_2);
        SetCaptureFovButton->setObjectName(QStringLiteral("SetCaptureFovButton"));
        SetCaptureFovButton->setGeometry(QRect(180, 100, 111, 23));
        CaptureColStartSpinBox = new QSpinBox(groupBox_2);
        CaptureColStartSpinBox->setObjectName(QStringLiteral("CaptureColStartSpinBox"));
        CaptureColStartSpinBox->setGeometry(QRect(90, 20, 71, 22));
        CaptureColSizeSpinBox = new QSpinBox(groupBox_2);
        CaptureColSizeSpinBox->setObjectName(QStringLiteral("CaptureColSizeSpinBox"));
        CaptureColSizeSpinBox->setGeometry(QRect(90, 60, 71, 22));
        CaptureRowStartSpinBox = new QSpinBox(groupBox_2);
        CaptureRowStartSpinBox->setObjectName(QStringLiteral("CaptureRowStartSpinBox"));
        CaptureRowStartSpinBox->setGeometry(QRect(240, 20, 71, 22));
        CaptureRowSizeSpinBox = new QSpinBox(groupBox_2);
        CaptureRowSizeSpinBox->setObjectName(QStringLiteral("CaptureRowSizeSpinBox"));
        CaptureRowSizeSpinBox->setGeometry(QRect(240, 60, 71, 22));
        CaptureMultiFrameSpinBox = new QSpinBox(groupBox_2);
        CaptureMultiFrameSpinBox->setObjectName(QStringLiteral("CaptureMultiFrameSpinBox"));
        CaptureMultiFrameSpinBox->setGeometry(QRect(90, 100, 71, 22));
        PreviewButton = new QPushButton(KSJDemoQT_Base);
        PreviewButton->setObjectName(QStringLiteral("PreviewButton"));
        PreviewButton->setGeometry(QRect(840, 270, 75, 23));
        CaptureButton = new QPushButton(KSJDemoQT_Base);
        CaptureButton->setObjectName(QStringLiteral("CaptureButton"));
        CaptureButton->setGeometry(QRect(840, 340, 75, 23));
        ERROR_INFO_Label = new QLabel(KSJDemoQT_Base);
        ERROR_INFO_Label->setObjectName(QStringLiteral("ERROR_INFO_Label"));
        ERROR_INFO_Label->setGeometry(QRect(510, 380, 201, 21));
        ELAPSE_TIME_Label = new QLabel(KSJDemoQT_Base);
        ELAPSE_TIME_Label->setObjectName(QStringLiteral("ELAPSE_TIME_Label"));
        ELAPSE_TIME_Label->setGeometry(QRect(740, 380, 121, 21));
        FunctionTableWidget = new QTableWidget(KSJDemoQT_Base);
        FunctionTableWidget->setObjectName(QStringLiteral("FunctionTableWidget"));
        FunctionTableWidget->setGeometry(QRect(920, 0, 311, 641));
        SaveCheckBox = new QCheckBox(KSJDemoQT_Base);
        SaveCheckBox->setObjectName(QStringLiteral("SaveCheckBox"));
        SaveCheckBox->setGeometry(QRect(840, 310, 71, 20));
        CreateMapButton = new QPushButton(KSJDemoQT_Base);
        CreateMapButton->setObjectName(QStringLiteral("CreateMapButton"));
        CreateMapButton->setGeometry(QRect(810, 610, 75, 23));
        CaliCheckBox = new QCheckBox(KSJDemoQT_Base);
        CaliCheckBox->setObjectName(QStringLiteral("CaliCheckBox"));
        CaliCheckBox->setGeometry(QRect(500, 610, 91, 21));
        ChessWidthSpinBox = new QSpinBox(KSJDemoQT_Base);
        ChessWidthSpinBox->setObjectName(QStringLiteral("ChessWidthSpinBox"));
        ChessWidthSpinBox->setGeometry(QRect(710, 610, 42, 22));
        ChessWidthSpinBox->setMinimum(4);
        ChessHeightSpinBox = new QSpinBox(KSJDemoQT_Base);
        ChessHeightSpinBox->setObjectName(QStringLiteral("ChessHeightSpinBox"));
        ChessHeightSpinBox->setGeometry(QRect(760, 610, 42, 22));
        ChessHeightSpinBox->setMinimum(4);
        ERROR_INFO_Label_2 = new QLabel(KSJDemoQT_Base);
        ERROR_INFO_Label_2->setObjectName(QStringLiteral("ERROR_INFO_Label_2"));
        ERROR_INFO_Label_2->setGeometry(QRect(640, 610, 71, 20));

        retranslateUi(KSJDemoQT_Base);

        QMetaObject::connectSlotsByName(KSJDemoQT_Base);
    } // setupUi

    void retranslateUi(QDialog *KSJDemoQT_Base)
    {
        KSJDemoQT_Base->setWindowTitle(QApplication::translate("KSJDemoQT_Base", "KSJCalibration", 0));
        DEVICENUM_Label->setText(QApplication::translate("KSJDemoQT_Base", "Device Num:", 0));
        label_6->setText(QApplication::translate("KSJDemoQT_Base", "Gain:", 0));
        label_7->setText(QApplication::translate("KSJDemoQT_Base", "Exposure Time(ms):", 0));
        label_8->setText(QApplication::translate("KSJDemoQT_Base", "Exposure Lines:", 0));
        label_9->setText(QApplication::translate("KSJDemoQT_Base", "Range(ms):", 0));
        label_10->setText(QApplication::translate("KSJDemoQT_Base", "Range:", 0));
        label_11->setText(QApplication::translate("KSJDemoQT_Base", "Range:", 0));
        EXPOSURE_TIME_RANGE_Label->setText(QApplication::translate("KSJDemoQT_Base", "Range", 0));
        EXPOSURE_LINES_RANGE_Label->setText(QApplication::translate("KSJDemoQT_Base", "Range", 0));
        GAIN_RANGE_Label->setText(QApplication::translate("KSJDemoQT_Base", "Range", 0));
        EXPOSURE_TIME_REAL_Label->setText(QApplication::translate("KSJDemoQT_Base", "REAL EXP TIME", 0));
        FOV_RANGE_Label->setText(QApplication::translate("KSJDemoQT_Base", "EXP LINES RANGE", 0));
        label_12->setText(QApplication::translate("KSJDemoQT_Base", "FOV Range:", 0));
        groupBox_2->setTitle(QApplication::translate("KSJDemoQT_Base", "Capture", 0));
        label_18->setText(QApplication::translate("KSJDemoQT_Base", "Col Start:", 0));
        label_19->setText(QApplication::translate("KSJDemoQT_Base", "Row Start:", 0));
        label_20->setText(QApplication::translate("KSJDemoQT_Base", "Col Size:", 0));
        label_21->setText(QApplication::translate("KSJDemoQT_Base", "Row Size:", 0));
        label_22->setText(QApplication::translate("KSJDemoQT_Base", "MultiFrame:", 0));
        SetCaptureFovButton->setText(QApplication::translate("KSJDemoQT_Base", "Set Capture Fov", 0));
        PreviewButton->setText(QApplication::translate("KSJDemoQT_Base", "Preview", 0));
        CaptureButton->setText(QApplication::translate("KSJDemoQT_Base", "Capture", 0));
        ERROR_INFO_Label->setText(QApplication::translate("KSJDemoQT_Base", "Error Info", 0));
        ELAPSE_TIME_Label->setText(QApplication::translate("KSJDemoQT_Base", "Elapse", 0));
        SaveCheckBox->setText(QApplication::translate("KSJDemoQT_Base", "Save", 0));
        CreateMapButton->setText(QApplication::translate("KSJDemoQT_Base", "Create Map", 0));
        CaliCheckBox->setText(QApplication::translate("KSJDemoQT_Base", "Calibration", 0));
        ERROR_INFO_Label_2->setText(QApplication::translate("KSJDemoQT_Base", "Chess Size:", 0));
    } // retranslateUi

};

namespace Ui {
    class KSJDemoQT_Base: public Ui_KSJDemoQT_Base {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KSJDEMOQT_BASE_H
