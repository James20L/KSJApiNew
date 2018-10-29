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

QT_BEGIN_NAMESPACE

class Ui_KSJSCZDemoMainWindow
{
public:
    QPushButton *CapturePushButton;
    QComboBox *DevicesComboBox;
    QPushButton *RefreshPushButton;
    QComboBox *TrigetComboBox;
    QCheckBox *MirrorCheckBox;
    QCheckBox *FlipCheckBox;
    QPushButton *SaveImagePushButton;
    QLabel *MessageInfoLabel;
    QPushButton *ExitPushButton;
    QGroupBox *groupBox;
    QSpinBox *ColStartSpinBox;
    QSpinBox *RowStartSpinBox;
    QSpinBox *ColSizeSpinBox;
    QSpinBox *RowSizeSpinBox;
    QPushButton *SetFovPushButton;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *MultiFrameNumberSpinBox;
    QDoubleSpinBox *FixFrameRateDoubleSpinBox;

    void setupUi(QDialog *KSJSCZDemoMainWindow)
    {
        if (KSJSCZDemoMainWindow->objectName().isEmpty())
            KSJSCZDemoMainWindow->setObjectName(QStringLiteral("KSJSCZDemoMainWindow"));
        KSJSCZDemoMainWindow->resize(1266, 795);
        CapturePushButton = new QPushButton(KSJSCZDemoMainWindow);
        CapturePushButton->setObjectName(QStringLiteral("CapturePushButton"));
        CapturePushButton->setGeometry(QRect(10, 310, 120, 36));
        DevicesComboBox = new QComboBox(KSJSCZDemoMainWindow);
        DevicesComboBox->setObjectName(QStringLiteral("DevicesComboBox"));
        DevicesComboBox->setGeometry(QRect(10, 10, 248, 28));
        RefreshPushButton = new QPushButton(KSJSCZDemoMainWindow);
        RefreshPushButton->setObjectName(QStringLiteral("RefreshPushButton"));
        RefreshPushButton->setGeometry(QRect(10, 40, 120, 36));
        TrigetComboBox = new QComboBox(KSJSCZDemoMainWindow);
        TrigetComboBox->setObjectName(QStringLiteral("TrigetComboBox"));
        TrigetComboBox->setGeometry(QRect(10, 84, 248, 28));
        MirrorCheckBox = new QCheckBox(KSJSCZDemoMainWindow);
        MirrorCheckBox->setObjectName(QStringLiteral("MirrorCheckBox"));
        MirrorCheckBox->setGeometry(QRect(20, 156, 85, 20));
        FlipCheckBox = new QCheckBox(KSJSCZDemoMainWindow);
        FlipCheckBox->setObjectName(QStringLiteral("FlipCheckBox"));
        FlipCheckBox->setGeometry(QRect(162, 156, 85, 20));
        SaveImagePushButton = new QPushButton(KSJSCZDemoMainWindow);
        SaveImagePushButton->setObjectName(QStringLiteral("SaveImagePushButton"));
        SaveImagePushButton->setGeometry(QRect(10, 360, 120, 36));
        MessageInfoLabel = new QLabel(KSJSCZDemoMainWindow);
        MessageInfoLabel->setObjectName(QStringLiteral("MessageInfoLabel"));
        MessageInfoLabel->setGeometry(QRect(10, 402, 248, 85));
        ExitPushButton = new QPushButton(KSJSCZDemoMainWindow);
        ExitPushButton->setObjectName(QStringLiteral("ExitPushButton"));
        ExitPushButton->setGeometry(QRect(10, 570, 120, 36));
        groupBox = new QGroupBox(KSJSCZDemoMainWindow);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 186, 248, 117));
        ColStartSpinBox = new QSpinBox(groupBox);
        ColStartSpinBox->setObjectName(QStringLiteral("ColStartSpinBox"));
        ColStartSpinBox->setGeometry(QRect(24, 20, 59, 22));
        ColStartSpinBox->setMaximum(8192);
        RowStartSpinBox = new QSpinBox(groupBox);
        RowStartSpinBox->setObjectName(QStringLiteral("RowStartSpinBox"));
        RowStartSpinBox->setGeometry(QRect(24, 52, 59, 22));
        RowStartSpinBox->setMaximum(8192);
        ColSizeSpinBox = new QSpinBox(groupBox);
        ColSizeSpinBox->setObjectName(QStringLiteral("ColSizeSpinBox"));
        ColSizeSpinBox->setGeometry(QRect(106, 20, 59, 22));
        ColSizeSpinBox->setMaximum(8192);
        RowSizeSpinBox = new QSpinBox(groupBox);
        RowSizeSpinBox->setObjectName(QStringLiteral("RowSizeSpinBox"));
        RowSizeSpinBox->setGeometry(QRect(106, 52, 59, 22));
        RowSizeSpinBox->setMaximum(8192);
        SetFovPushButton = new QPushButton(groupBox);
        SetFovPushButton->setObjectName(QStringLiteral("SetFovPushButton"));
        SetFovPushButton->setGeometry(QRect(180, 28, 55, 36));
        label = new QLabel(KSJSCZDemoMainWindow);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(16, 126, 120, 16));
        label_2 = new QLabel(KSJSCZDemoMainWindow);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(16, 272, 120, 16));
        MultiFrameNumberSpinBox = new QSpinBox(KSJSCZDemoMainWindow);
        MultiFrameNumberSpinBox->setObjectName(QStringLiteral("MultiFrameNumberSpinBox"));
        MultiFrameNumberSpinBox->setGeometry(QRect(140, 270, 59, 22));
        MultiFrameNumberSpinBox->setMaximum(8192);
        FixFrameRateDoubleSpinBox = new QDoubleSpinBox(KSJSCZDemoMainWindow);
        FixFrameRateDoubleSpinBox->setObjectName(QStringLiteral("FixFrameRateDoubleSpinBox"));
        FixFrameRateDoubleSpinBox->setGeometry(QRect(140, 124, 62, 22));

        retranslateUi(KSJSCZDemoMainWindow);

        QMetaObject::connectSlotsByName(KSJSCZDemoMainWindow);
    } // setupUi

    void retranslateUi(QDialog *KSJSCZDemoMainWindow)
    {
        KSJSCZDemoMainWindow->setWindowTitle(QApplication::translate("KSJSCZDemoMainWindow", "Dialog", 0));
        CapturePushButton->setText(QApplication::translate("KSJSCZDemoMainWindow", "Start", 0));
        RefreshPushButton->setText(QApplication::translate("KSJSCZDemoMainWindow", "Refresh", 0));
        MirrorCheckBox->setText(QApplication::translate("KSJSCZDemoMainWindow", "Mirror", 0));
        FlipCheckBox->setText(QApplication::translate("KSJSCZDemoMainWindow", "Flip", 0));
        SaveImagePushButton->setText(QApplication::translate("KSJSCZDemoMainWindow", "Save", 0));
        MessageInfoLabel->setText(QString());
        ExitPushButton->setText(QApplication::translate("KSJSCZDemoMainWindow", "Exit", 0));
        groupBox->setTitle(QApplication::translate("KSJSCZDemoMainWindow", "FOV", 0));
        SetFovPushButton->setText(QApplication::translate("KSJSCZDemoMainWindow", "Set", 0));
        label->setText(QApplication::translate("KSJSCZDemoMainWindow", "Fixed frame rate:", 0));
        label_2->setText(QApplication::translate("KSJSCZDemoMainWindow", "Multiframe number:", 0));
    } // retranslateUi

};

namespace Ui {
    class KSJSCZDemoMainWindow: public Ui_KSJSCZDemoMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KSJSCZDEMOMAINWINDOW_H
