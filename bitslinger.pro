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
    journal.cpp \
    qhexedit2/chunks.cpp \
    qhexedit2/commands.cpp \
    qhexedit2/qhexedit.cpp \
    bitslinger.cpp

HEADERS  += mainwindow.h \
    tcpproxy.h \
    connection.h \
    journal.h \
    qhexedit2/chunks.h \
    qhexedit2/commands.h \
    qhexedit2/qhexedit.h \
    bitslinger.h

FORMS    += mainwindow.ui \
    mockups/mainview_mockup.ui

DISTFILES += \
    NOTES.txt \
    icons/client2server.png \
    icons/client2server_connected.png \
    icons/client2server_disconnected.png \
    icons/client2server_edited.png \
    icons/server2client.png \
    icons/server2client_disconnected.png \
    icons/server2client_edited.png

RESOURCES += \
    bitslinger.qrc
