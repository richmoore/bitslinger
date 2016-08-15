#include <QPalette>
#include <QWidget>
#include <QDebug>

#include "colorutils.h"


QColor ColorUtils::autoBackgroundColor(QWidget *container, int index)
{
//    QPalette pal = container->palette();
//    QColor base = pal.color(QPalette::Base);
    QColor base(0xaa, 0xee, 0xcc);


    double value = (base.red() + base.green() + base.blue()) / (3.0*256);
    qDebug() << "value" << value;
//    double newValue = value + 2*(double(qrand())/RAND_MAX)*index-index;
    double newValue = value + 2*(double(index))/50.0 - (double(index))/50.0;
    qDebug() << "new value" << newValue;
    double valueRatio = newValue/value;
    qDebug() << "ratio" << valueRatio;

    return QColor(int((double(base.red())*valueRatio)) % 256,
                  (int(double(base.green())*valueRatio)) % 256,
                  (int(double(base.blue())*valueRatio)) % 256);
}
