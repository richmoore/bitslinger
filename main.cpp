#include "mainwindow.h"
#include <QApplication>

#include "tcpproxy.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    TcpProxy proxy;
    proxy.listen();

    return a.exec();
}
