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
win32: {
  contains(QMAKE_HOST.arch, x86_64):{
    INCLUDEPATH += c:/OpenSSL-Win64/include
 } else {
    INCLUDEPATH += c:/OpenSSL-Win32/include
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
    settings/sslcasettingspage.cpp \
    utils/hostnamelineedit.cpp \
    utils/colorutils.cpp \
    utils/recentfilesmenu.cpp \
    utils/sslconfigurationwidget.cpp \
    abstracttool.cpp \
    proxytool/proxytool.cpp \
    proxytool/proxytoolview.cpp \
    repeatertool/repeatertool.cpp \
    repeatertool/repeatertoolview.cpp \
    repeatertool/repeatergutterwidget.cpp

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
    settings/sslcasettingspage.h \
    utils/hostnamelineedit.h \
    utils/colorutils.h \
    utils/recentfilesmenu.h \
    utils/sslconfigurationwidget.h \
    abstracttool.h \
    proxytool/proxytool.h \
    proxytool/proxytoolview.h \
    repeatertool/repeatertool.h \
    repeatertool/repeatertoolview.h \
    repeatertool/repeatergutterwidget.h

FORMS    += mainwindow.ui \
    mockups/mainview_mockup.ui \
    listenerdialog.ui \
    listenereditdialog.ui \
    settingsdialog.ui \
    settings/proxysettingspage.ui \
    settings/sslcasettingspage.ui \
    utils/sslconfigurationwidget.ui \
    proxytool/proxytoolview.ui \
    repeatertool/repeatertoolview.ui

DISTFILES += \
    NOTES.txt \
    state_format.txt

RESOURCES += \
    bitslinger.qrc
