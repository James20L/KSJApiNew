/********************************************************************************
** Form generated from reading UI file 'KSJDemoQT_Matrix.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KSJDEMOQT_MATRIX_H
#define UI_KSJDEMOQT_MATRIX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KSJDemoQT_Matrix
{
public:
    QWidget *widget;
    QFormLayout *formLayout;
    QWidget *PREVIEWWND_Widget;
    QVBoxLayout *verticalLayout_6;
    QGridLayout *gridLayout_3;
    QLabel *DEVICENUM_Label;
    QComboBox *DEVICE_LIST_ComboBox;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_5;
    QGridLayout *gridLayout_4;
    QLabel *label_8;
    QLabel *EXPOSURE_TIME_RANGE_Label;
    QLabel *label_9;
    QSpinBox *ExposureTimeSpinBox;
    QSpinBox *ExposureLinesSpinBox;
    QLabel *label_10;
    QLabel *label_7;
    QLabel *EXPOSURE_LINES_RANGE_Label;
    QLabel *EXPOSURE_TIME_REAL_Label;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_6;
    QSpinBox *GainSpinBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_11;
    QLabel *GAIN_RANGE_Label;
    QGridLayout *gridLayout_6;
    QLabel *label_12;
    QLabel *FOV_RANGE_Label;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *SetPreviewFovButton;
    QSpinBox *PreviewColStartSpinBox;
    QSpinBox *PreviewColSizeSpinBox;
    QSpinBox *PreviewRowStartSpinBox;
    QSpinBox *PreviewRowSizeSpinBox;
    QSpinBox *PreviewMultiFrameSpinBox;
    QPushButton *PreviewButton;
    QVBoxLayout *verticalLayout_2;
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
    QCheckBox *SaveCheckBox;
    QPushButton *CaptureButton;
    QHBoxLayout *horizontalLayout;
    QLabel *ERROR_INFO_Label;
    QLabel *ELAPSE_TIME_Label;
    QVBoxLayout *verticalLayout_4;
    QGridLayout *gridLayout;
    QSpinBox *PhiSpinBox;
    QComboBox *WBComboBox;
    QComboBox *PresettingComboBox;
    QLabel *label_13;
    QLabel *DEVICENUM_Label_5;
    QSlider *GSlider;
    QTableWidget *FunctionTableWidget;
    QLabel *DEVICENUM_Label_3;
    QLabel *label_14;
    QSlider *BSlider;
    QLabel *label_15;
    QLabel *DEVICENUM_Label_4;
    QComboBox *CCMComboBox;
    QLabel *label_16;
    QSlider *RSlider;
    QComboBox *CCMPresettingComboBox;
    QLabel *DEVICENUM_Label_2;
    QLineEdit *REdit;
    QLineEdit *GEdit;
    QLineEdit *BEdit;
    QLabel *label_17;
    QGridLayout *gridLayout_2;
    QLineEdit *GEdit02;
    QSlider *GSlider12;
    QSlider *GSlider20;
    QSlider *GSlider01;
    QSlider *GSlider02;
    QSlider *GSlider21;
    QLineEdit *GEdit20;
    QLineEdit *GEdit22;
    QSlider *GSlider11;
    QLineEdit *GEdit12;
    QLineEdit *GEdit00;
    QLineEdit *GEdit10;
    QLineEdit *GEdit21;
    QLineEdit *GEdit11;
    QSlider *GSlider10;
    QSlider *GSlider22;
    QLineEdit *GEdit01;
    QSlider *GSlider00;

    void setupUi(QDialog *KSJDemoQT_Matrix)
    {
        if (KSJDemoQT_Matrix->objectName().isEmpty())
            KSJDemoQT_Matrix->setObjectName(QStringLiteral("KSJDemoQT_Matrix"));
        KSJDemoQT_Matrix->resize(1411, 908);
        KSJDemoQT_Matrix->setLayoutDirection(Qt::LeftToRight);
        widget = new QWidget(KSJDemoQT_Matrix);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(0, 0, 1811, 623));
        formLayout = new QFormLayout(widget);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        PREVIEWWND_Widget = new QWidget(widget);
        PREVIEWWND_Widget->setObjectName(QStringLiteral("PREVIEWWND_Widget"));
        PREVIEWWND_Widget->setMinimumSize(QSize(800, 600));

        formLayout->setWidget(0, QFormLayout::LabelRole, PREVIEWWND_Widget);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        DEVICENUM_Label = new QLabel(widget);
        DEVICENUM_Label->setObjectName(QStringLiteral("DEVICENUM_Label"));
        DEVICENUM_Label->setMaximumSize(QSize(80, 16777215));

        gridLayout_3->addWidget(DEVICENUM_Label, 0, 0, 1, 1);

        DEVICE_LIST_ComboBox = new QComboBox(widget);
        DEVICE_LIST_ComboBox->setObjectName(QStringLiteral("DEVICE_LIST_ComboBox"));

        gridLayout_3->addWidget(DEVICE_LIST_ComboBox, 0, 1, 1, 1);


        verticalLayout_6->addLayout(gridLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        label_8 = new QLabel(widget);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_4->addWidget(label_8, 1, 0, 1, 1);

        EXPOSURE_TIME_RANGE_Label = new QLabel(widget);
        EXPOSURE_TIME_RANGE_Label->setObjectName(QStringLiteral("EXPOSURE_TIME_RANGE_Label"));

        gridLayout_4->addWidget(EXPOSURE_TIME_RANGE_Label, 0, 3, 1, 1);

        label_9 = new QLabel(widget);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_4->addWidget(label_9, 0, 2, 1, 1);

        ExposureTimeSpinBox = new QSpinBox(widget);
        ExposureTimeSpinBox->setObjectName(QStringLiteral("ExposureTimeSpinBox"));

        gridLayout_4->addWidget(ExposureTimeSpinBox, 0, 1, 1, 1);

        ExposureLinesSpinBox = new QSpinBox(widget);
        ExposureLinesSpinBox->setObjectName(QStringLiteral("ExposureLinesSpinBox"));

        gridLayout_4->addWidget(ExposureLinesSpinBox, 1, 1, 1, 1);

        label_10 = new QLabel(widget);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_4->addWidget(label_10, 1, 2, 1, 1);

        label_7 = new QLabel(widget);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_4->addWidget(label_7, 0, 0, 1, 1);

        EXPOSURE_LINES_RANGE_Label = new QLabel(widget);
        EXPOSURE_LINES_RANGE_Label->setObjectName(QStringLiteral("EXPOSURE_LINES_RANGE_Label"));

        gridLayout_4->addWidget(EXPOSURE_LINES_RANGE_Label, 1, 3, 1, 1);

        EXPOSURE_TIME_REAL_Label = new QLabel(widget);
        EXPOSURE_TIME_REAL_Label->setObjectName(QStringLiteral("EXPOSURE_TIME_REAL_Label"));
        EXPOSURE_TIME_REAL_Label->setMinimumSize(QSize(80, 20));
        EXPOSURE_TIME_REAL_Label->setMaximumSize(QSize(80, 20));

        gridLayout_4->addWidget(EXPOSURE_TIME_REAL_Label, 2, 0, 1, 1);


        verticalLayout_5->addLayout(gridLayout_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_6 = new QLabel(widget);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_3->addWidget(label_6);

        GainSpinBox = new QSpinBox(widget);
        GainSpinBox->setObjectName(QStringLiteral("GainSpinBox"));

        horizontalLayout_3->addWidget(GainSpinBox);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_11 = new QLabel(widget);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout_2->addWidget(label_11);

        GAIN_RANGE_Label = new QLabel(widget);
        GAIN_RANGE_Label->setObjectName(QStringLiteral("GAIN_RANGE_Label"));

        horizontalLayout_2->addWidget(GAIN_RANGE_Label);


        verticalLayout_3->addLayout(horizontalLayout_2);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setSizeConstraint(QLayout::SetFixedSize);
        label_12 = new QLabel(widget);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setMinimumSize(QSize(20, 20));
        label_12->setMaximumSize(QSize(80, 20));

        gridLayout_6->addWidget(label_12, 0, 0, 1, 1);

        FOV_RANGE_Label = new QLabel(widget);
        FOV_RANGE_Label->setObjectName(QStringLiteral("FOV_RANGE_Label"));
        FOV_RANGE_Label->setMaximumSize(QSize(80, 20));

        gridLayout_6->addWidget(FOV_RANGE_Label, 0, 1, 1, 1);


        verticalLayout_3->addLayout(gridLayout_6);


        verticalLayout_5->addLayout(verticalLayout_3);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMinimumSize(QSize(320, 140));
        groupBox->setMaximumSize(QSize(320, 140));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 20, 61, 21));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(180, 20, 61, 21));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 60, 61, 21));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(180, 60, 61, 21));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 100, 71, 21));
        SetPreviewFovButton = new QPushButton(groupBox);
        SetPreviewFovButton->setObjectName(QStringLiteral("SetPreviewFovButton"));
        SetPreviewFovButton->setGeometry(QRect(180, 100, 111, 23));
        PreviewColStartSpinBox = new QSpinBox(groupBox);
        PreviewColStartSpinBox->setObjectName(QStringLiteral("PreviewColStartSpinBox"));
        PreviewColStartSpinBox->setGeometry(QRect(90, 20, 71, 22));
        PreviewColSizeSpinBox = new QSpinBox(groupBox);
        PreviewColSizeSpinBox->setObjectName(QStringLiteral("PreviewColSizeSpinBox"));
        PreviewColSizeSpinBox->setGeometry(QRect(90, 60, 71, 22));
        PreviewRowStartSpinBox = new QSpinBox(groupBox);
        PreviewRowStartSpinBox->setObjectName(QStringLiteral("PreviewRowStartSpinBox"));
        PreviewRowStartSpinBox->setGeometry(QRect(240, 20, 71, 22));
        PreviewRowSizeSpinBox = new QSpinBox(groupBox);
        PreviewRowSizeSpinBox->setObjectName(QStringLiteral("PreviewRowSizeSpinBox"));
        PreviewRowSizeSpinBox->setGeometry(QRect(240, 60, 71, 22));
        PreviewMultiFrameSpinBox = new QSpinBox(groupBox);
        PreviewMultiFrameSpinBox->setObjectName(QStringLiteral("PreviewMultiFrameSpinBox"));
        PreviewMultiFrameSpinBox->setGeometry(QRect(90, 100, 71, 22));

        verticalLayout->addWidget(groupBox);

        PreviewButton = new QPushButton(widget);
        PreviewButton->setObjectName(QStringLiteral("PreviewButton"));
        PreviewButton->setMaximumSize(QSize(80, 20));

        verticalLayout->addWidget(PreviewButton);


        verticalLayout_5->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        groupBox_2 = new QGroupBox(widget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(320, 140));
        groupBox_2->setMaximumSize(QSize(320, 140));
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

        verticalLayout_2->addWidget(groupBox_2);

        SaveCheckBox = new QCheckBox(widget);
        SaveCheckBox->setObjectName(QStringLiteral("SaveCheckBox"));

        verticalLayout_2->addWidget(SaveCheckBox);

        CaptureButton = new QPushButton(widget);
        CaptureButton->setObjectName(QStringLiteral("CaptureButton"));
        CaptureButton->setMaximumSize(QSize(80, 20));

        verticalLayout_2->addWidget(CaptureButton);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        ERROR_INFO_Label = new QLabel(widget);
        ERROR_INFO_Label->setObjectName(QStringLiteral("ERROR_INFO_Label"));
        ERROR_INFO_Label->setMinimumSize(QSize(160, 40));
        ERROR_INFO_Label->setMaximumSize(QSize(160, 40));

        horizontalLayout->addWidget(ERROR_INFO_Label);

        ELAPSE_TIME_Label = new QLabel(widget);
        ELAPSE_TIME_Label->setObjectName(QStringLiteral("ELAPSE_TIME_Label"));
        ELAPSE_TIME_Label->setMinimumSize(QSize(160, 40));
        ELAPSE_TIME_Label->setMaximumSize(QSize(160, 40));

        horizontalLayout->addWidget(ELAPSE_TIME_Label);


        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout_5->addLayout(verticalLayout_2);


        horizontalLayout_4->addLayout(verticalLayout_5);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        PhiSpinBox = new QSpinBox(widget);
        PhiSpinBox->setObjectName(QStringLiteral("PhiSpinBox"));
        PhiSpinBox->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(PhiSpinBox, 4, 1, 1, 1);

        WBComboBox = new QComboBox(widget);
        WBComboBox->setObjectName(QStringLiteral("WBComboBox"));
        WBComboBox->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(WBComboBox, 2, 1, 1, 1);

        PresettingComboBox = new QComboBox(widget);
        PresettingComboBox->setObjectName(QStringLiteral("PresettingComboBox"));
        PresettingComboBox->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(PresettingComboBox, 3, 1, 1, 1);

        label_13 = new QLabel(widget);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(label_13, 4, 0, 1, 1);

        DEVICENUM_Label_5 = new QLabel(widget);
        DEVICENUM_Label_5->setObjectName(QStringLiteral("DEVICENUM_Label_5"));
        DEVICENUM_Label_5->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(DEVICENUM_Label_5, 12, 0, 1, 1);

        GSlider = new QSlider(widget);
        GSlider->setObjectName(QStringLiteral("GSlider"));
        GSlider->setMaximumSize(QSize(200, 16777215));
        GSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(GSlider, 7, 1, 1, 1);

        FunctionTableWidget = new QTableWidget(widget);
        FunctionTableWidget->setObjectName(QStringLiteral("FunctionTableWidget"));
        FunctionTableWidget->setMinimumSize(QSize(200, 200));
        FunctionTableWidget->setMaximumSize(QSize(200, 200));

        gridLayout->addWidget(FunctionTableWidget, 1, 1, 1, 1);

        DEVICENUM_Label_3 = new QLabel(widget);
        DEVICENUM_Label_3->setObjectName(QStringLiteral("DEVICENUM_Label_3"));
        DEVICENUM_Label_3->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(DEVICENUM_Label_3, 3, 0, 1, 1);

        label_14 = new QLabel(widget);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(label_14, 5, 0, 1, 1);

        BSlider = new QSlider(widget);
        BSlider->setObjectName(QStringLiteral("BSlider"));
        BSlider->setMaximumSize(QSize(200, 16777215));
        BSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(BSlider, 9, 1, 1, 1);

        label_15 = new QLabel(widget);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(label_15, 7, 0, 1, 1);

        DEVICENUM_Label_4 = new QLabel(widget);
        DEVICENUM_Label_4->setObjectName(QStringLiteral("DEVICENUM_Label_4"));
        DEVICENUM_Label_4->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(DEVICENUM_Label_4, 11, 0, 1, 1);

        CCMComboBox = new QComboBox(widget);
        CCMComboBox->setObjectName(QStringLiteral("CCMComboBox"));
        CCMComboBox->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(CCMComboBox, 11, 1, 1, 1);

        label_16 = new QLabel(widget);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(label_16, 9, 0, 1, 1);

        RSlider = new QSlider(widget);
        RSlider->setObjectName(QStringLiteral("RSlider"));
        RSlider->setMaximumSize(QSize(200, 16777215));
        RSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(RSlider, 5, 1, 1, 1);

        CCMPresettingComboBox = new QComboBox(widget);
        CCMPresettingComboBox->setObjectName(QStringLiteral("CCMPresettingComboBox"));
        CCMPresettingComboBox->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(CCMPresettingComboBox, 12, 1, 1, 1);

        DEVICENUM_Label_2 = new QLabel(widget);
        DEVICENUM_Label_2->setObjectName(QStringLiteral("DEVICENUM_Label_2"));
        DEVICENUM_Label_2->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(DEVICENUM_Label_2, 2, 0, 1, 1);

        REdit = new QLineEdit(widget);
        REdit->setObjectName(QStringLiteral("REdit"));
        REdit->setMaximumSize(QSize(200, 16777215));
        REdit->setReadOnly(true);

        gridLayout->addWidget(REdit, 6, 1, 1, 1);

        GEdit = new QLineEdit(widget);
        GEdit->setObjectName(QStringLiteral("GEdit"));
        GEdit->setMaximumSize(QSize(200, 16777215));
        GEdit->setReadOnly(true);

        gridLayout->addWidget(GEdit, 8, 1, 1, 1);

        BEdit = new QLineEdit(widget);
        BEdit->setObjectName(QStringLiteral("BEdit"));
        BEdit->setMaximumSize(QSize(200, 16777215));
        BEdit->setReadOnly(true);

        gridLayout->addWidget(BEdit, 10, 1, 1, 1);

        label_17 = new QLabel(widget);
        label_17->setObjectName(QStringLiteral("label_17"));

        gridLayout->addWidget(label_17, 1, 0, 1, 1);


        verticalLayout_4->addLayout(gridLayout);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        GEdit02 = new QLineEdit(widget);
        GEdit02->setObjectName(QStringLiteral("GEdit02"));
        GEdit02->setReadOnly(true);

        gridLayout_2->addWidget(GEdit02, 0, 5, 1, 1);

        GSlider12 = new QSlider(widget);
        GSlider12->setObjectName(QStringLiteral("GSlider12"));
        GSlider12->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(GSlider12, 1, 4, 1, 1);

        GSlider20 = new QSlider(widget);
        GSlider20->setObjectName(QStringLiteral("GSlider20"));
        GSlider20->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(GSlider20, 2, 0, 1, 1);

        GSlider01 = new QSlider(widget);
        GSlider01->setObjectName(QStringLiteral("GSlider01"));
        GSlider01->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(GSlider01, 0, 2, 1, 1);

        GSlider02 = new QSlider(widget);
        GSlider02->setObjectName(QStringLiteral("GSlider02"));
        GSlider02->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(GSlider02, 0, 4, 1, 1);

        GSlider21 = new QSlider(widget);
        GSlider21->setObjectName(QStringLiteral("GSlider21"));
        GSlider21->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(GSlider21, 2, 2, 1, 1);

        GEdit20 = new QLineEdit(widget);
        GEdit20->setObjectName(QStringLiteral("GEdit20"));
        GEdit20->setReadOnly(true);

        gridLayout_2->addWidget(GEdit20, 2, 1, 1, 1);

        GEdit22 = new QLineEdit(widget);
        GEdit22->setObjectName(QStringLiteral("GEdit22"));
        GEdit22->setReadOnly(true);

        gridLayout_2->addWidget(GEdit22, 2, 5, 1, 1);

        GSlider11 = new QSlider(widget);
        GSlider11->setObjectName(QStringLiteral("GSlider11"));
        GSlider11->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(GSlider11, 1, 2, 1, 1);

        GEdit12 = new QLineEdit(widget);
        GEdit12->setObjectName(QStringLiteral("GEdit12"));
        GEdit12->setReadOnly(true);

        gridLayout_2->addWidget(GEdit12, 1, 5, 1, 1);

        GEdit00 = new QLineEdit(widget);
        GEdit00->setObjectName(QStringLiteral("GEdit00"));
        GEdit00->setReadOnly(true);

        gridLayout_2->addWidget(GEdit00, 0, 1, 1, 1);

        GEdit10 = new QLineEdit(widget);
        GEdit10->setObjectName(QStringLiteral("GEdit10"));
        GEdit10->setReadOnly(true);

        gridLayout_2->addWidget(GEdit10, 1, 1, 1, 1);

        GEdit21 = new QLineEdit(widget);
        GEdit21->setObjectName(QStringLiteral("GEdit21"));
        GEdit21->setReadOnly(true);

        gridLayout_2->addWidget(GEdit21, 2, 3, 1, 1);

        GEdit11 = new QLineEdit(widget);
        GEdit11->setObjectName(QStringLiteral("GEdit11"));
        GEdit11->setReadOnly(true);

        gridLayout_2->addWidget(GEdit11, 1, 3, 1, 1);

        GSlider10 = new QSlider(widget);
        GSlider10->setObjectName(QStringLiteral("GSlider10"));
        GSlider10->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(GSlider10, 1, 0, 1, 1);

        GSlider22 = new QSlider(widget);
        GSlider22->setObjectName(QStringLiteral("GSlider22"));
        GSlider22->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(GSlider22, 2, 4, 1, 1);

        GEdit01 = new QLineEdit(widget);
        GEdit01->setObjectName(QStringLiteral("GEdit01"));
        GEdit01->setReadOnly(true);

        gridLayout_2->addWidget(GEdit01, 0, 3, 1, 1);

        GSlider00 = new QSlider(widget);
        GSlider00->setObjectName(QStringLiteral("GSlider00"));
        GSlider00->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(GSlider00, 0, 0, 1, 1);


        verticalLayout_4->addLayout(gridLayout_2);


        horizontalLayout_4->addLayout(verticalLayout_4);


        verticalLayout_6->addLayout(horizontalLayout_4);


        formLayout->setLayout(0, QFormLayout::FieldRole, verticalLayout_6);

        PREVIEWWND_Widget->raise();
        groupBox->raise();

        retranslateUi(KSJDemoQT_Matrix);

        QMetaObject::connectSlotsByName(KSJDemoQT_Matrix);
    } // setupUi

    void retranslateUi(QDialog *KSJDemoQT_Matrix)
    {
        KSJDemoQT_Matrix->setWindowTitle(QApplication::translate("KSJDemoQT_Matrix", "KSJDemoQT", 0));
        DEVICENUM_Label->setText(QApplication::translate("KSJDemoQT_Matrix", "Device Num:", 0));
        label_8->setText(QApplication::translate("KSJDemoQT_Matrix", "Exposure Lines:", 0));
        EXPOSURE_TIME_RANGE_Label->setText(QApplication::translate("KSJDemoQT_Matrix", "Range", 0));
        label_9->setText(QApplication::translate("KSJDemoQT_Matrix", "Range(ms):", 0));
        label_10->setText(QApplication::translate("KSJDemoQT_Matrix", "Range:", 0));
        label_7->setText(QApplication::translate("KSJDemoQT_Matrix", "Exposure Time(ms):", 0));
        EXPOSURE_LINES_RANGE_Label->setText(QApplication::translate("KSJDemoQT_Matrix", "Range", 0));
        EXPOSURE_TIME_REAL_Label->setText(QApplication::translate("KSJDemoQT_Matrix", "REAL EXP TIME", 0));
        label_6->setText(QApplication::translate("KSJDemoQT_Matrix", "Gain:", 0));
        label_11->setText(QApplication::translate("KSJDemoQT_Matrix", "Range:", 0));
        GAIN_RANGE_Label->setText(QApplication::translate("KSJDemoQT_Matrix", "Range", 0));
        label_12->setText(QApplication::translate("KSJDemoQT_Matrix", "FOV Range:", 0));
        FOV_RANGE_Label->setText(QApplication::translate("KSJDemoQT_Matrix", "EXP LINES RANGE", 0));
        groupBox->setTitle(QApplication::translate("KSJDemoQT_Matrix", "Preview", 0));
        label->setText(QApplication::translate("KSJDemoQT_Matrix", "Col Start:", 0));
        label_2->setText(QApplication::translate("KSJDemoQT_Matrix", "Row Start:", 0));
        label_3->setText(QApplication::translate("KSJDemoQT_Matrix", "Col Size:", 0));
        label_4->setText(QApplication::translate("KSJDemoQT_Matrix", "Row Size:", 0));
        label_5->setText(QApplication::translate("KSJDemoQT_Matrix", "MultiFrame:", 0));
        SetPreviewFovButton->setText(QApplication::translate("KSJDemoQT_Matrix", "Set Preview Fov", 0));
        PreviewButton->setText(QApplication::translate("KSJDemoQT_Matrix", "Preview", 0));
        groupBox_2->setTitle(QApplication::translate("KSJDemoQT_Matrix", "Capture", 0));
        label_18->setText(QApplication::translate("KSJDemoQT_Matrix", "Col Start:", 0));
        label_19->setText(QApplication::translate("KSJDemoQT_Matrix", "Row Start:", 0));
        label_20->setText(QApplication::translate("KSJDemoQT_Matrix", "Col Size:", 0));
        label_21->setText(QApplication::translate("KSJDemoQT_Matrix", "Row Size:", 0));
        label_22->setText(QApplication::translate("KSJDemoQT_Matrix", "MultiFrame:", 0));
        SetCaptureFovButton->setText(QApplication::translate("KSJDemoQT_Matrix", "Set Capture Fov", 0));
        SaveCheckBox->setText(QApplication::translate("KSJDemoQT_Matrix", "Save", 0));
        CaptureButton->setText(QApplication::translate("KSJDemoQT_Matrix", "Capture", 0));
        ERROR_INFO_Label->setText(QApplication::translate("KSJDemoQT_Matrix", "Error Info", 0));
        ELAPSE_TIME_Label->setText(QApplication::translate("KSJDemoQT_Matrix", "Elapse", 0));
        label_13->setText(QApplication::translate("KSJDemoQT_Matrix", "Phi:", 0));
        DEVICENUM_Label_5->setText(QApplication::translate("KSJDemoQT_Matrix", "Presetting:", 0));
        DEVICENUM_Label_3->setText(QApplication::translate("KSJDemoQT_Matrix", "Presetting:", 0));
        label_14->setText(QApplication::translate("KSJDemoQT_Matrix", "R:", 0));
        label_15->setText(QApplication::translate("KSJDemoQT_Matrix", "G:", 0));
        DEVICENUM_Label_4->setText(QApplication::translate("KSJDemoQT_Matrix", "CCM Mode:", 0));
        label_16->setText(QApplication::translate("KSJDemoQT_Matrix", "B:", 0));
        DEVICENUM_Label_2->setText(QApplication::translate("KSJDemoQT_Matrix", "WB Mode:", 0));
        label_17->setText(QApplication::translate("KSJDemoQT_Matrix", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class KSJDemoQT_Matrix: public Ui_KSJDemoQT_Matrix {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KSJDEMOQT_MATRIX_H
