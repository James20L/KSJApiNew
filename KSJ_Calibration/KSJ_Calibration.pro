#-------------------------------------------------
#
# Project created by QtCreator 2018-03-20T15:06:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KSJ_Calibration
TEMPLATE = app

INCLUDEPATH +=../../../github.KSJApiNew/KSJApi.inc
INCLUDEPATH +=../../../git.KSJWinInLinux

LIBS += -L../../../git.opencv310-in-vs/opencv.inc
LIBS += -L../../../github.KSJApiNew/KSJApi.bin/linux/x64 \
       -Wl,--start-group  \
       -lopencv_core -lopencv_flann -lopencv_calib3d -lopencv_imgproc \
        -lksjlog -lavformat -lavcodec -lavutil -lswresample -lusb-1.0 -lksjapi \
       -lksjbayer  -ljpeg -ldl -lpthread  -lz -llzma -Wl,--end-group -Wl,-rpath,../../KSJApi.bin/linux/x64


SOURCES += main.cpp \
    KSJ_GS.cpp \
    KSJDemoQT_Base.cpp \
    KSJPreviewThread.cpp \
    KSJPreviewWidget.cpp \
    ksj_calibration_link.cpp \
    KSJCalibration.cpp \
    KSJRemap.cpp

HEADERS  += \
    KSJ_GS.H \
    KSJDemoQT_Base.h \
    KSJPreviewThread.h \
    KSJPreviewWidget.h \
    KSJCalibrartion.h \
    KSJRemap.h \
    version.h

FORMS    += \
    KSJDemoQT_Base.ui
