#-------------------------------------------------
#
# Project created by QtCreator 2017-10-31T17:45:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtdemo_linux
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
        -Wl,--start-group  -lopencv_core -lopencv_highgui -lopencv_contrib \
        -lksjlog -lopencv_imgproc -lavformat -lavcodec -lavutil -lswresample -lusb-1.0 -lksjapi \
        -lksjbayer  -ljpeg -ldl -lpthread  -lz -llzma -Wl,--end-group -Wl,-rpath,../../KSJApi.bin/linux/x64

QMAKE_CXXFLAGS +=


QMAKE_LFLAGS +=

SOURCES += main.cpp\
        mainwindow.cpp \
    KSJDemoQT_Base.cpp \
    KSJ_GS.cpp \
    KSJPreviewWidget.cpp \
    KSJPreviewThread.cpp


HEADERS  += mainwindow.h \
    KSJDemoQT_Base.h \
    KSJ_GS.H \
    KSJPreviewWidget.h \
    KSJPreviewThread.h

FORMS    += mainwindow.ui \
    KSJDemoQT_Base.ui

RESOURCES += \
    KSJDemoQT_Base.qrc
