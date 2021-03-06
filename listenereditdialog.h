#ifndef LISTENEREDITDIALOG_H
#define LISTENEREDITDIALOG_H

#include <QDialog>
#include <QHostAddress>

#include "listener.h"

namespace Ui {
class ListenerEditDialog;
}

class ListenerEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ListenerEditDialog(QWidget *parent = 0);
    ~ListenerEditDialog();

    Listener::ProxyType proxyType() const;
    void setProxyType(Listener::ProxyType type);

    Listener::SslMode sslMode() const;
    void setSslMode(Listener::SslMode mode);

    QHostAddress listenAddress() const;
    void setListenAddress(const QHostAddress &address);

    int listenPort() const;
    void setListenPort(int port);

    QString targetHost() const;
    void setTargetHost(const QString &host);

    int targetPort() const;
    void setTargetPort(int port);

protected slots:
    void proxyTypeChanged(int index);

private:
    Ui::ListenerEditDialog *ui;
    QList<QHostAddress> addresses;
};

#endif // LISTENEREDITDIALOG_H
