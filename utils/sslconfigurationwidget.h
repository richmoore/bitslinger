#ifndef SSLCONFIGURATIONWIDGET_H
#define SSLCONFIGURATIONWIDGET_H

#include <QWidget>

class QSslConfiguration;

namespace Ui {
class SslConfigurationWidget;
}

class SslConfigurationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SslConfigurationWidget(QWidget *parent = 0);
    ~SslConfigurationWidget();

    void setConfiguration(const QSslConfiguration &config);

private:
    Ui::SslConfigurationWidget *ui;
};

#endif // SSLCONFIGURATIONWIDGET_H
