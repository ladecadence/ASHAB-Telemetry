#-------------------------------------------------
#
# Project created by QtCreator 2016-06-18T21:25:12
#
#-------------------------------------------------

QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ASHABTelemetry
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    about.cpp \
    config.cpp \
    telemetry.cpp \
    logdialog.cpp \
    maxmindialog.cpp \
    rs8.c \
    ssdvdialog.cpp \
    ssdvpicturedialog.cpp \
    consoledialog.cpp \
    ssdv.c

HEADERS  += mainwindow.h \
    about.h \
    config.h \
    telemetry.h \
    logdialog.h \
    maxmindialog.h \
    lorassdv.h \
    rs8.h \
    ssdv.h \
    ssdvdialog.h \
    ssdvpicturedialog.h \
    consoledialog.h

FORMS    += mainwindow.ui \
    about.ui \
    config.ui \
    logdialog.ui \
    maxmindialog.ui \
    ssdvdialog.ui \
    ssdvpicturedialog.ui \
    dialogconsole.ui \
    consoledialog.ui

RESOURCES += \
    images.qrc
