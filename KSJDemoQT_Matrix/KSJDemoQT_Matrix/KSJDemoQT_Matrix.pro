#-------------------------------------------------
#
# Project created by QtCreator 2018-02-26T17:28:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KSJDemoQT_Matrix
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH +=../../KSJApi.inc

INCLUDEPATH +=../../../git.KSJLog/KSJLog.Inc/


LIBS += -L../../KSJApi.bin/linux/x64 \
        -Wl,--start-group  -lopencv_core -lopencv_highgui \
        -lksjlog -lopencv_imgproc -lavformat -lavcodec -lavutil -lswresample -lusb-1.0 -lksjapi \
        -lksjbayer  -ljpeg -ldl -lpthread  -lz -llzma -Wl,--end-group -Wl,-rpath,../../KSJApi.bin/linux/x64




SOURCES += main.cpp \
    KSJDemoQT_Matrix.cpp \
    KSJ_GS.cpp \
    KSJPreviewThread.cpp \
    KSJPreviewWidget.cpp

HEADERS  += \
    KSJDemoQT_Matrix.h \
    KSJPreviewWidget.h \
    KSJPreviewThread.h

FORMS    += mainwindow.ui \
    KSJDemoQT_Matrix.ui
