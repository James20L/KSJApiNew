/********************************************************************************
** Form generated from reading UI file 'KSJSCZDemoMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KSJSCZDEMOMAINWINDOW_H
#define UI_KSJSCZDEMOMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KSJSCZDemoMainWindow
{
public:
    QGroupBox *SettingGroupBox;
    QComboBox *DevicesComboBox;
    QPushButton *RefreshPushButton;
    QLabel *label_3;
    QPushButton *PreViewPushButton;
    QPushButton *SnapImagePushButton;
    QPushButton *ExitAppPushButton;
    QPushButton *PragramSettingsPushButton;
    QPushButton *ClearSettingsPushButton;
    QLabel *SnapCountLabel;
    QTabWidget *SettingTabWidget;
    QWidget *NormalTab;
    QPushButton *ProgramLutPushButton;
    QGroupBox *groupBox_2;
    QSpinBox *GainGreenSpinBox;
    QSpinBox *GainRedSpinBox;
    QSpinBox *GainBlueSpinBox;
    QLabel *label_13;
    QLabel *label_16;
    QLabel *label_19;
    QDoubleSpinBox *FixFrameRateDoubleSpinBox;
    QComboBox *SensitivityComboBox;
    QSpinBox *CaptureTimeoutSpinBox;
    QComboBox *TrigetModeComboBox;
    QLabel *label_17;
    QLabel *label;
    QLabel *label_7;
    QLabel *label_4;
    QCheckBox *FlipCheckBox;
    QGroupBox *groupBox;
    QSpinBox *ColStartSpinBox;
    QSpinBox *RowStartSpinBox;
    QSpinBox *ColSizeSpinBox;
    QSpinBox *RowSizeSpinBox;
    QPushButton *ApplyFovPushButton;
    QSpinBox *MultiFrameNumberSpinBox;
    QLabel *label_2;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_11;
    QLabel *label_10;
    QComboBox *TrigetMethodComboBox;
    QLabel *label_18;
    QGroupBox *groupBox_4;
    QSpinBox *ExpoureLineSpinBox;
    QLabel *label_14;
    QLabel *label_15;
    QDoubleSpinBox *ExposureTimeSpinBox;
    QComboBox *SkipComboBox;
    QLabel *label_6;
    QCheckBox *MirrorCheckBox;
    QWidget *ColorMatrixTab;
    QGroupBox *groupBox_3;
    QPushButton *ProgramWBMatrixPushButton;
    QLabel *label_12;
    QComboBox *WBModeComboBox;
    QLabel *label_20;
    QComboBox *WBPresettingComboBox;
    QLabel *label_25;
    QLabel *label_23;
    QLabel *label_24;
    QDoubleSpinBox *WBRedSpinBox;
    QSpinBox *WBPHiSpinBox;
    QLabel *label_21;
    QDoubleSpinBox *WBGreenSpinBox;
    QDoubleSpinBox *WBBlueSpinBox;
    QGroupBox *groupBox_5;
    QPushButton *ProgramCCMMatrixPushButton;
    QLabel *label_22;
    QComboBox *CCMModeComboBox;
    QLabel *label_26;
    QComboBox *CCMPresettingComboBox;
    QDoubleSpinBox *CCM00SpinBox;
    QDoubleSpinBox *CCM01SpinBox;
    QDoubleSpinBox *CCM02SpinBox;
    QDoubleSpinBox *CCM11SpinBox;
    QDoubleSpinBox *CCM10SpinBox;
    QDoubleSpinBox *CCM12SpinBox;
    QDoubleSpinBox *CCM21SpinBox;
    QDoubleSpinBox *CCM22SpinBox;
    QDoubleSpinBox *CCM20SpinBox;

    void setupUi(QDialog *KSJSCZDemoMainWindow)
    {
        if (KSJSCZDemoMainWindow->objectName().isEmpty())
            KSJSCZDemoMainWindow->setObjectName(QStringLiteral("KSJSCZDemoMainWindow"));
        KSJSCZDemoMainWindow->resize(1266, 768);
        SettingGroupBox = new QGroupBox(KSJSCZDemoMainWindow);
        SettingGroupBox->setObjectName(QStringLiteral("SettingGroupBox"));
        SettingGroupBox->setGeometry(QRect(4, 2, 312, 757));
        DevicesComboBox = new QComboBox(SettingGroupBox);
        DevicesComboBox->setObjectName(QStringLiteral("DevicesComboBox"));
        DevicesComboBox->setGeometry(QRect(10, 38, 221, 28));
        RefreshPushButton = new QPushButton(SettingGroupBox);
        RefreshPushButton->setObjectName(QStringLiteral("RefreshPushButton"));
        RefreshPushButton->setGeometry(QRect(233, 36, 69, 32));
        label_3 = new QLabel(SettingGroupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(12, 18, 120, 16));
        PreViewPushButton = new QPushButton(SettingGroupBox);
        PreViewPushButton->setObjectName(QStringLiteral("PreViewPushButton"));
        PreViewPushButton->setGeometry(QRect(10, 76, 69, 32));
        SnapImagePushButton = new QPushButton(SettingGroupBox);
        SnapImagePushButton->setObjectName(QStringLiteral("SnapImagePushButton"));
        SnapImagePushButton->setGeometry(QRect(94, 76, 69, 32));
        ExitAppPushButton = new QPushButton(SettingGroupBox);
        ExitAppPushButton->setObjectName(QStringLiteral("ExitAppPushButton"));
        ExitAppPushButton->setGeometry(QRect(233, 76, 69, 32));
        PragramSettingsPushButton = new QPushButton(SettingGroupBox);
        PragramSettingsPushButton->setObjectName(QStringLiteral("PragramSettingsPushButton"));
        PragramSettingsPushButton->setGeometry(QRect(10, 722, 132, 32));
        ClearSettingsPushButton = new QPushButton(SettingGroupBox);
        ClearSettingsPushButton->setObjectName(QStringLiteral("ClearSettingsPushButton"));
        ClearSettingsPushButton->setGeometry(QRect(170, 722, 132, 32));
        SnapCountLabel = new QLabel(SettingGroupBox);
        SnapCountLabel->setObjectName(QStringLiteral("SnapCountLabel"));
        SnapCountLabel->setGeometry(QRect(168, 84, 29, 16));
        SettingTabWidget = new QTabWidget(SettingGroupBox);
        SettingTabWidget->setObjectName(QStringLiteral("SettingTabWidget"));
        SettingTabWidget->setGeometry(QRect(4, 116, 304, 581));
        NormalTab = new QWidget();
        NormalTab->setObjectName(QStringLiteral("NormalTab"));
        ProgramLutPushButton = new QPushButton(NormalTab);
        ProgramLutPushButton->setObjectName(QStringLiteral("ProgramLutPushButton"));
        ProgramLutPushButton->setGeometry(QRect(8, 522, 120, 32));
        groupBox_2 = new QGroupBox(NormalTab);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(6, 456, 288, 53));
        GainGreenSpinBox = new QSpinBox(groupBox_2);
        GainGreenSpinBox->setObjectName(QStringLiteral("GainGreenSpinBox"));
        GainGreenSpinBox->setGeometry(QRect(140, 21, 48, 22));
        GainGreenSpinBox->setMaximum(8192);
        GainRedSpinBox = new QSpinBox(groupBox_2);
        GainRedSpinBox->setObjectName(QStringLiteral("GainRedSpinBox"));
        GainRedSpinBox->setGeometry(QRect(42, 21, 48, 22));
        GainRedSpinBox->setMaximum(8192);
        GainBlueSpinBox = new QSpinBox(groupBox_2);
        GainBlueSpinBox->setObjectName(QStringLiteral("GainBlueSpinBox"));
        GainBlueSpinBox->setGeometry(QRect(230, 21, 48, 22));
        GainBlueSpinBox->setMaximum(8192);
        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(96, 24, 40, 16));
        label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_16 = new QLabel(groupBox_2);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(192, 24, 35, 16));
        label_16->setLayoutDirection(Qt::LeftToRight);
        label_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_19 = new QLabel(groupBox_2);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(7, 24, 31, 16));
        label_19->setLayoutDirection(Qt::LeftToRight);
        label_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        FixFrameRateDoubleSpinBox = new QDoubleSpinBox(NormalTab);
        FixFrameRateDoubleSpinBox->setObjectName(QStringLiteral("FixFrameRateDoubleSpinBox"));
        FixFrameRateDoubleSpinBox->setGeometry(QRect(136, 114, 62, 22));
        SensitivityComboBox = new QComboBox(NormalTab);
        SensitivityComboBox->setObjectName(QStringLiteral("SensitivityComboBox"));
        SensitivityComboBox->setGeometry(QRect(90, 422, 137, 28));
        CaptureTimeoutSpinBox = new QSpinBox(NormalTab);
        CaptureTimeoutSpinBox->setObjectName(QStringLiteral("CaptureTimeoutSpinBox"));
        CaptureTimeoutSpinBox->setGeometry(QRect(136, 142, 79, 22));
        CaptureTimeoutSpinBox->setMaximum(999999999);
        TrigetModeComboBox = new QComboBox(NormalTab);
        TrigetModeComboBox->setObjectName(QStringLiteral("TrigetModeComboBox"));
        TrigetModeComboBox->setGeometry(QRect(26, 28, 264, 28));
        label_17 = new QLabel(NormalTab);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(6, 180, 43, 16));
        label = new QLabel(NormalTab);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(6, 118, 120, 16));
        label_7 = new QLabel(NormalTab);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(6, 62, 120, 16));
        label_4 = new QLabel(NormalTab);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(6, 146, 120, 16));
        FlipCheckBox = new QCheckBox(NormalTab);
        FlipCheckBox->setObjectName(QStringLiteral("FlipCheckBox"));
        FlipCheckBox->setGeometry(QRect(148, 392, 85, 20));
        groupBox = new QGroupBox(NormalTab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(4, 204, 290, 117));
        ColStartSpinBox = new QSpinBox(groupBox);
        ColStartSpinBox->setObjectName(QStringLiteral("ColStartSpinBox"));
        ColStartSpinBox->setGeometry(QRect(96, 18, 59, 22));
        ColStartSpinBox->setMaximum(8192);
        RowStartSpinBox = new QSpinBox(groupBox);
        RowStartSpinBox->setObjectName(QStringLiteral("RowStartSpinBox"));
        RowStartSpinBox->setGeometry(QRect(96, 50, 59, 22));
        RowStartSpinBox->setMaximum(8192);
        ColSizeSpinBox = new QSpinBox(groupBox);
        ColSizeSpinBox->setObjectName(QStringLiteral("ColSizeSpinBox"));
        ColSizeSpinBox->setGeometry(QRect(204, 18, 59, 22));
        ColSizeSpinBox->setMaximum(8192);
        RowSizeSpinBox = new QSpinBox(groupBox);
        RowSizeSpinBox->setObjectName(QStringLiteral("RowSizeSpinBox"));
        RowSizeSpinBox->setGeometry(QRect(204, 50, 59, 22));
        RowSizeSpinBox->setMaximum(8192);
        ApplyFovPushButton = new QPushButton(groupBox);
        ApplyFovPushButton->setObjectName(QStringLiteral("ApplyFovPushButton"));
        ApplyFovPushButton->setGeometry(QRect(208, 78, 69, 32));
        MultiFrameNumberSpinBox = new QSpinBox(groupBox);
        MultiFrameNumberSpinBox->setObjectName(QStringLiteral("MultiFrameNumberSpinBox"));
        MultiFrameNumberSpinBox->setGeometry(QRect(130, 84, 59, 22));
        MultiFrameNumberSpinBox->setMaximum(8192);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(8, 86, 120, 16));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 20, 80, 16));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(10, 52, 80, 16));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(160, 52, 36, 16));
        label_11->setLayoutDirection(Qt::LeftToRight);
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(160, 20, 36, 16));
        label_10->setLayoutDirection(Qt::LeftToRight);
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        TrigetMethodComboBox = new QComboBox(NormalTab);
        TrigetMethodComboBox->setObjectName(QStringLiteral("TrigetMethodComboBox"));
        TrigetMethodComboBox->setGeometry(QRect(26, 80, 264, 28));
        label_18 = new QLabel(NormalTab);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(6, 430, 79, 16));
        groupBox_4 = new QGroupBox(NormalTab);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(4, 330, 290, 55));
        ExpoureLineSpinBox = new QSpinBox(groupBox_4);
        ExpoureLineSpinBox->setObjectName(QStringLiteral("ExpoureLineSpinBox"));
        ExpoureLineSpinBox->setGeometry(QRect(72, 24, 69, 22));
        ExpoureLineSpinBox->setMaximum(8192);
        label_14 = new QLabel(groupBox_4);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(10, 26, 40, 16));
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_15 = new QLabel(groupBox_4);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(152, 26, 59, 16));
        label_15->setLayoutDirection(Qt::LeftToRight);
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        ExposureTimeSpinBox = new QDoubleSpinBox(groupBox_4);
        ExposureTimeSpinBox->setObjectName(QStringLiteral("ExposureTimeSpinBox"));
        ExposureTimeSpinBox->setEnabled(false);
        ExposureTimeSpinBox->setGeometry(QRect(216, 24, 62, 22));
        SkipComboBox = new QComboBox(NormalTab);
        SkipComboBox->setObjectName(QStringLiteral("SkipComboBox"));
        SkipComboBox->setGeometry(QRect(58, 172, 137, 28));
        label_6 = new QLabel(NormalTab);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(6, 8, 120, 16));
        MirrorCheckBox = new QCheckBox(NormalTab);
        MirrorCheckBox->setObjectName(QStringLiteral("MirrorCheckBox"));
        MirrorCheckBox->setGeometry(QRect(6, 392, 85, 20));
        SettingTabWidget->addTab(NormalTab, QString());
        ColorMatrixTab = new QWidget();
        ColorMatrixTab->setObjectName(QStringLiteral("ColorMatrixTab"));
        groupBox_3 = new QGroupBox(ColorMatrixTab);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(4, 12, 290, 191));
        ProgramWBMatrixPushButton = new QPushButton(groupBox_3);
        ProgramWBMatrixPushButton->setObjectName(QStringLiteral("ProgramWBMatrixPushButton"));
        ProgramWBMatrixPushButton->setGeometry(QRect(142, 154, 141, 32));
        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(8, 26, 80, 16));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        WBModeComboBox = new QComboBox(groupBox_3);
        WBModeComboBox->setObjectName(QStringLiteral("WBModeComboBox"));
        WBModeComboBox->setGeometry(QRect(102, 22, 168, 28));
        label_20 = new QLabel(groupBox_3);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(8, 60, 80, 16));
        label_20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        WBPresettingComboBox = new QComboBox(groupBox_3);
        WBPresettingComboBox->setObjectName(QStringLiteral("WBPresettingComboBox"));
        WBPresettingComboBox->setGeometry(QRect(102, 56, 168, 28));
        label_25 = new QLabel(groupBox_3);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setGeometry(QRect(8, 128, 29, 16));
        label_25->setLayoutDirection(Qt::LeftToRight);
        label_25->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_23 = new QLabel(groupBox_3);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setGeometry(QRect(196, 128, 36, 16));
        label_23->setLayoutDirection(Qt::LeftToRight);
        label_23->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_24 = new QLabel(groupBox_3);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setGeometry(QRect(100, 128, 36, 16));
        label_24->setLayoutDirection(Qt::LeftToRight);
        label_24->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        WBRedSpinBox = new QDoubleSpinBox(groupBox_3);
        WBRedSpinBox->setObjectName(QStringLiteral("WBRedSpinBox"));
        WBRedSpinBox->setGeometry(QRect(42, 126, 52, 22));
        WBRedSpinBox->setDecimals(3);
        WBRedSpinBox->setMaximum(5);
        WBPHiSpinBox = new QSpinBox(groupBox_3);
        WBPHiSpinBox->setObjectName(QStringLiteral("WBPHiSpinBox"));
        WBPHiSpinBox->setGeometry(QRect(102, 94, 59, 22));
        WBPHiSpinBox->setMaximum(8192);
        label_21 = new QLabel(groupBox_3);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(8, 96, 80, 16));
        label_21->setLayoutDirection(Qt::LeftToRight);
        label_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        WBGreenSpinBox = new QDoubleSpinBox(groupBox_3);
        WBGreenSpinBox->setObjectName(QStringLiteral("WBGreenSpinBox"));
        WBGreenSpinBox->setGeometry(QRect(142, 126, 52, 22));
        WBGreenSpinBox->setDecimals(3);
        WBGreenSpinBox->setMaximum(5);
        WBBlueSpinBox = new QDoubleSpinBox(groupBox_3);
        WBBlueSpinBox->setObjectName(QStringLiteral("WBBlueSpinBox"));
        WBBlueSpinBox->setGeometry(QRect(232, 126, 52, 22));
        WBBlueSpinBox->setDecimals(3);
        WBBlueSpinBox->setMaximum(5);
        groupBox_5 = new QGroupBox(ColorMatrixTab);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(4, 214, 290, 233));
        ProgramCCMMatrixPushButton = new QPushButton(groupBox_5);
        ProgramCCMMatrixPushButton->setObjectName(QStringLiteral("ProgramCCMMatrixPushButton"));
        ProgramCCMMatrixPushButton->setGeometry(QRect(142, 196, 141, 32));
        label_22 = new QLabel(groupBox_5);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(8, 26, 80, 16));
        label_22->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        CCMModeComboBox = new QComboBox(groupBox_5);
        CCMModeComboBox->setObjectName(QStringLiteral("CCMModeComboBox"));
        CCMModeComboBox->setGeometry(QRect(102, 22, 168, 28));
        label_26 = new QLabel(groupBox_5);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setGeometry(QRect(8, 60, 80, 16));
        label_26->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        CCMPresettingComboBox = new QComboBox(groupBox_5);
        CCMPresettingComboBox->setObjectName(QStringLiteral("CCMPresettingComboBox"));
        CCMPresettingComboBox->setGeometry(QRect(102, 56, 168, 28));
        CCM00SpinBox = new QDoubleSpinBox(groupBox_5);
        CCM00SpinBox->setObjectName(QStringLiteral("CCM00SpinBox"));
        CCM00SpinBox->setGeometry(QRect(54, 96, 52, 22));
        CCM00SpinBox->setDecimals(3);
        CCM00SpinBox->setMaximum(1e+12);
        CCM00SpinBox->setSingleStep(0.001);
        CCM01SpinBox = new QDoubleSpinBox(groupBox_5);
        CCM01SpinBox->setObjectName(QStringLiteral("CCM01SpinBox"));
        CCM01SpinBox->setGeometry(QRect(134, 96, 52, 22));
        CCM01SpinBox->setDecimals(3);
        CCM01SpinBox->setMaximum(1e+12);
        CCM01SpinBox->setSingleStep(0.001);
        CCM02SpinBox = new QDoubleSpinBox(groupBox_5);
        CCM02SpinBox->setObjectName(QStringLiteral("CCM02SpinBox"));
        CCM02SpinBox->setGeometry(QRect(214, 96, 52, 22));
        CCM02SpinBox->setDecimals(3);
        CCM02SpinBox->setMaximum(1e+12);
        CCM02SpinBox->setSingleStep(0.001);
        CCM11SpinBox = new QDoubleSpinBox(groupBox_5);
        CCM11SpinBox->setObjectName(QStringLiteral("CCM11SpinBox"));
        CCM11SpinBox->setGeometry(QRect(134, 128, 52, 22));
        CCM11SpinBox->setDecimals(3);
        CCM11SpinBox->setMaximum(1e+12);
        CCM11SpinBox->setSingleStep(0.001);
        CCM10SpinBox = new QDoubleSpinBox(groupBox_5);
        CCM10SpinBox->setObjectName(QStringLiteral("CCM10SpinBox"));
        CCM10SpinBox->setGeometry(QRect(54, 128, 52, 22));
        CCM10SpinBox->setDecimals(3);
        CCM10SpinBox->setMaximum(1e+12);
        CCM10SpinBox->setSingleStep(0.001);
        CCM12SpinBox = new QDoubleSpinBox(groupBox_5);
        CCM12SpinBox->setObjectName(QStringLiteral("CCM12SpinBox"));
        CCM12SpinBox->setGeometry(QRect(214, 128, 52, 22));
        CCM12SpinBox->setDecimals(3);
        CCM12SpinBox->setMaximum(1e+12);
        CCM12SpinBox->setSingleStep(0.001);
        CCM21SpinBox = new QDoubleSpinBox(groupBox_5);
        CCM21SpinBox->setObjectName(QStringLiteral("CCM21SpinBox"));
        CCM21SpinBox->setGeometry(QRect(134, 160, 52, 22));
        CCM21SpinBox->setDecimals(3);
        CCM21SpinBox->setMaximum(1e+12);
        CCM21SpinBox->setSingleStep(0.001);
        CCM22SpinBox = new QDoubleSpinBox(groupBox_5);
        CCM22SpinBox->setObjectName(QStringLiteral("CCM22SpinBox"));
        CCM22SpinBox->setGeometry(QRect(214, 160, 52, 22));
        CCM22SpinBox->setDecimals(3);
        CCM22SpinBox->setMaximum(1e+12);
        CCM22SpinBox->setSingleStep(0.001);
        CCM20SpinBox = new QDoubleSpinBox(groupBox_5);
        CCM20SpinBox->setObjectName(QStringLiteral("CCM20SpinBox"));
        CCM20SpinBox->setGeometry(QRect(54, 160, 52, 22));
        CCM20SpinBox->setDecimals(3);
        CCM20SpinBox->setMaximum(1e+12);
        CCM20SpinBox->setSingleStep(0.001);
        SettingTabWidget->addTab(ColorMatrixTab, QString());
        SettingGroupBox->raise();
        label_17->raise();

        retranslateUi(KSJSCZDemoMainWindow);

        SettingTabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(KSJSCZDemoMainWindow);
    } // setupUi

    void retranslateUi(QDialog *KSJSCZDemoMainWindow)
    {
        KSJSCZDemoMainWindow->setWindowTitle(QApplication::translate("KSJSCZDemoMainWindow", "Dialog", 0));
        SettingGroupBox->setTitle(QApplication::translate("KSJSCZDemoMainWindow", "Setting", 0));
        RefreshPushButton->setText(QApplication::translate("KSJSCZDemoMainWindow", "Refresh", 0));
        label_3->setText(QApplication::translate("KSJSCZDemoMainWindow", "Device:", 0));
        PreViewPushButton->setText(QApplication::translate("KSJSCZDemoMainWindow", "Start", 0));
        SnapImagePushButton->setText(QApplication::translate("KSJSCZDemoMainWindow", "Snap", 0));
        ExitAppPushButton->setText(QApplication::translate("KSJSCZDemoMainWindow", "Exit", 0));
        PragramSettingsPushButton->setText(QApplication::translate("KSJSCZDemoMainWindow", "Pragram to camera", 0));
        ClearSettingsPushButton->setText(QApplication::translate("KSJSCZDemoMainWindow", "Clear camera setting", 0));
        SnapCountLabel->setText(QApplication::translate("KSJSCZDemoMainWindow", "0", 0));
        ProgramLutPushButton->setText(QApplication::translate("KSJSCZDemoMainWindow", "Program Lut", 0));
        groupBox_2->setTitle(QApplication::translate("KSJSCZDemoMainWindow", "CMOS Gain", 0));
        label_13->setText(QApplication::translate("KSJSCZDemoMainWindow", "Green:", 0));
        label_16->setText(QApplication::translate("KSJSCZDemoMainWindow", "Blue:", 0));
        label_19->setText(QApplication::translate("KSJSCZDemoMainWindow", "Red:", 0));
        label_17->setText(QApplication::translate("KSJSCZDemoMainWindow", "Skip:", 0));
        label->setText(QApplication::translate("KSJSCZDemoMainWindow", "Fixed frame rate:", 0));
        label_7->setText(QApplication::translate("KSJSCZDemoMainWindow", "Trigger Method:", 0));
        label_4->setText(QApplication::translate("KSJSCZDemoMainWindow", "Capture timeout:", 0));
        FlipCheckBox->setText(QApplication::translate("KSJSCZDemoMainWindow", "Flip", 0));
        groupBox->setTitle(QApplication::translate("KSJSCZDemoMainWindow", "FOV", 0));
        ApplyFovPushButton->setText(QApplication::translate("KSJSCZDemoMainWindow", "Apply", 0));
        label_2->setText(QApplication::translate("KSJSCZDemoMainWindow", "Multiframe number:", 0));
        label_8->setText(QApplication::translate("KSJSCZDemoMainWindow", "Col Start:", 0));
        label_9->setText(QApplication::translate("KSJSCZDemoMainWindow", "Row Start:", 0));
        label_11->setText(QApplication::translate("KSJSCZDemoMainWindow", "Size:", 0));
        label_10->setText(QApplication::translate("KSJSCZDemoMainWindow", "Size:", 0));
        label_18->setText(QApplication::translate("KSJSCZDemoMainWindow", "Sensitivity:", 0));
        groupBox_4->setTitle(QApplication::translate("KSJSCZDemoMainWindow", "Exposure", 0));
        label_14->setText(QApplication::translate("KSJSCZDemoMainWindow", "Line:", 0));
        label_15->setText(QApplication::translate("KSJSCZDemoMainWindow", "Time(MS):", 0));
        label_6->setText(QApplication::translate("KSJSCZDemoMainWindow", "Trigger Mode:", 0));
        MirrorCheckBox->setText(QApplication::translate("KSJSCZDemoMainWindow", "Mirror", 0));
        SettingTabWidget->setTabText(SettingTabWidget->indexOf(NormalTab), QApplication::translate("KSJSCZDemoMainWindow", "Normal", 0));
        groupBox_3->setTitle(QApplication::translate("KSJSCZDemoMainWindow", "White Balance Matrix", 0));
        ProgramWBMatrixPushButton->setText(QApplication::translate("KSJSCZDemoMainWindow", "Programm WB Matrix", 0));
        label_12->setText(QApplication::translate("KSJSCZDemoMainWindow", "Mode:", 0));
        label_20->setText(QApplication::translate("KSJSCZDemoMainWindow", "Presettings:", 0));
        label_25->setText(QApplication::translate("KSJSCZDemoMainWindow", "Red:", 0));
        label_23->setText(QApplication::translate("KSJSCZDemoMainWindow", "Blue:", 0));
        label_24->setText(QApplication::translate("KSJSCZDemoMainWindow", "Green:", 0));
        label_21->setText(QApplication::translate("KSJSCZDemoMainWindow", "PHi:", 0));
        groupBox_5->setTitle(QApplication::translate("KSJSCZDemoMainWindow", "Color Correction Matrix", 0));
        ProgramCCMMatrixPushButton->setText(QApplication::translate("KSJSCZDemoMainWindow", "Programm CCM Matrix", 0));
        label_22->setText(QApplication::translate("KSJSCZDemoMainWindow", "Mode:", 0));
        label_26->setText(QApplication::translate("KSJSCZDemoMainWindow", "Presettings:", 0));
        SettingTabWidget->setTabText(SettingTabWidget->indexOf(ColorMatrixTab), QApplication::translate("KSJSCZDemoMainWindow", "Color Matrix", 0));
    } // retranslateUi

};

namespace Ui {
    class KSJSCZDemoMainWindow: public Ui_KSJSCZDemoMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KSJSCZDEMOMAINWINDOW_H
