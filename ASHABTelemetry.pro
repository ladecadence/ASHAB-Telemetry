#-------------------------------------------------
#
# Project created by QtCreator 2016-06-18T21:25:12
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ASHABTelemetry
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    about.cpp \
    config.cpp \
    telemetry.cpp

HEADERS  += mainwindow.h \
    about.h \
    config.h \
    telemetry.h

FORMS    += mainwindow.ui \
    about.ui \
    config.ui

RESOURCES += \
    images.qrc
