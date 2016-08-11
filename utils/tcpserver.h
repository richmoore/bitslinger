#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QtNetwork/QTcpServer>
#include <QSslSocket>

class TcpServer : public QTcpServer
{
    Q_OBJECT
    Q_DISABLE_COPY(TcpServer)

public:
    TcpServer(QObject *parent = Q_NULLPTR);
    virtual ~TcpServer();

    QSslSocket *nextPendingConnection();

protected:
    virtual void incomingConnection(qintptr socket);
};

#endif // TCPSERVER_H
