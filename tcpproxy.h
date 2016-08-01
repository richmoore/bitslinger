#ifndef TCPPROXY_H
#define TCPPROXY_H

#include <QObject>

class Journal;

class TcpProxy : public QObject
{
    Q_OBJECT
public:
    explicit TcpProxy(QObject *parent = 0);

    Journal *journal();

signals:
    void connectionReceived();

public slots:
    void listen();

private slots:
    void handleConnection();

private:
    struct TcpProxyPrivate *d;
};

#endif // TCPPROXY_H
