#include "mainwindow.h"
#include <QApplication>

#include "tcpproxy.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    TcpProxy proxy;

    w.setJournal(proxy.journal());

    proxy.listen();

    return a.exec();
}
