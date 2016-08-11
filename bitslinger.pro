#-------------------------------------------------
#
# Project created by QtCreator 2016-07-31T12:50:18
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bitslinger
TEMPLATE = app

#INCLUDEPATH += c:/OpenSSL-Win64/include
win32-msvc {
  contains(QMAKE_HOST.arch, x86):{
    INCLUDEPATH += c:/OpenSSL-Win32/include
 } else {
    INCLUDEPATH += c:/OpenSSL-Win64/include
  }
}

gcc|clang:QMAKE_CXXFLAGS += -Wall -Wextra

contains(QT_CONFIG, openssl-linked) {
    win32-msvc {
        LIBS += -leay32
    }
    else {
        LIBS += -lcrypto
    }
}


SOURCES += main.cpp\
        mainwindow.cpp \
    connection.cpp \
    journal.cpp \
    qhexedit2/chunks.cpp \
    qhexedit2/commands.cpp \
    qhexedit2/qhexedit.cpp \
    bitslinger.cpp \
    journalview.cpp \
    listener.cpp \
    listenerdialog.cpp \
    listenereditdialog.cpp \
    settingsdialog.cpp \
    settingspage.cpp \
    settings/proxysettingspage.cpp \
    utils/openssl_symbol_helper.cpp \
    utils/certificategenerator.cpp \
    utils/tcpserver.cpp \
    settings/sslcasettingspage.cpp

HEADERS  += mainwindow.h \
    connection.h \
    journal.h \
    qhexedit2/chunks.h \
    qhexedit2/commands.h \
    qhexedit2/qhexedit.h \
    bitslinger.h \
    journalview.h \
    listener.h \
    listenerdialog.h \
    listenereditdialog.h \
    settingsdialog.h \
    settingspage.h \
    settings/proxysettingspage.h \
    utils/openssl_symbol_helper.h \
    utils/certificategenerator.h \
    utils/tcpserver.h \
    settings/sslcasettingspage.h

FORMS    += mainwindow.ui \
    mockups/mainview_mockup.ui \
    listenerdialog.ui \
    listenereditdialog.ui \
    settingsdialog.ui \
    settings/proxysettingspage.ui \
    settings/sslcasettingspage.ui

DISTFILES += \
    NOTES.txt \
    state_format.txt

RESOURCES += \
    bitslinger.qrc
