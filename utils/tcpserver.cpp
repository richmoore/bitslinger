#include <QtNetwork/QSslSocket>

#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) :
    QTcpServer(parent)
{
}

TcpServer::~TcpServer()
{
}

QSslSocket *TcpServer::nextPendingConnection()
{
    return static_cast<QSslSocket *>(QTcpServer::nextPendingConnection());
}

void TcpServer::incomingConnection(qintptr socket)
{
    QSslSocket *sock = new QSslSocket();
    sock->setSocketDescriptor(socket);
    addPendingConnection(sock);
}
