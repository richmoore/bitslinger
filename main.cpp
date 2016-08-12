#include <QApplication>
#include <QHostAddress>

#include "bitslinger.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setOrganizationName(QStringLiteral("Richard J. Moore"));
    app.setOrganizationDomain("sectools.co.uk");
    app.setApplicationName(QStringLiteral("BitSlinger"));

    MainWindow w;
    w.show();

    BitSlinger slinger;
    w.setBitSlinger(&slinger);
    slinger.loadListenerConfig();

    return app.exec();
}
