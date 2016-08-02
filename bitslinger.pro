#-------------------------------------------------
#
# Project created by QtCreator 2016-07-31T12:50:18
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bitslinger
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tcpproxy.cpp \
    connection.cpp \
    journal.cpp

HEADERS  += mainwindow.h \
    tcpproxy.h \
    connection.h \
    journal.h

FORMS    += mainwindow.ui

DISTFILES += \
    NOTES.txt

RESOURCES += \
    bitslinger.qrc
