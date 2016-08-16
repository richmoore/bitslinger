#ifndef SSLCONFIGURATIONWIDGET_H
#define SSLCONFIGURATIONWIDGET_H

#include <QWidget>
#include <QSslConfiguration>

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

public slots:
    void setReadOnly(bool readonly);
    void setConfiguration(const QSslConfiguration &config);

protected:
    void reloadConfiguration();

private:
    Ui::SslConfigurationWidget *ui;
    QSslConfiguration m_config;
    bool m_readonly;
};

#endif // SSLCONFIGURATIONWIDGET_H
