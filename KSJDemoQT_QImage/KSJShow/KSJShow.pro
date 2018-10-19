#-------------------------------------------------
#
# Project created by QtCreator 2018-06-19T10:45:16
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


INCPATH   += ../../KSJApi.inc

LIBS     = -L../../KSJApi.bin/linux/x64 -lksjapi -lksjlog -lksjbayer -lpthread -lusb-1.0 -ludev -ljpeg  -ldl

TARGET   = KSJShow
TEMPLATE = app
DEFINES -= _UNICODE


SOURCES += main.cpp \
    KSJSCZDemoMainWindow.cpp

HEADERS  += \
    KSJSCZDemoMainWindow.h

FORMS    += \
    KSJSCZDemoMainWindow.ui



