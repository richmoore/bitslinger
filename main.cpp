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
    w.setBitSlinger(&slinger);

    return a.exec();
}
