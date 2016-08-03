#include <QApplication>
#include <QHostAddress>

#include "bitslinger.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    BitSlinger slinger;
    w.setJournal(slinger.journal());
    slinger.addProxy(QHostAddress::LocalHost, 4433, QString("xmelegance.org"), 443);

    return a.exec();
}
