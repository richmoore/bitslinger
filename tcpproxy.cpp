#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

#include "connection.h"
#include "journal.h"

#include "tcpproxy.h"

struct TcpProxyPrivate
{
    Journal *journal;
    QString targetHost;
    int targetPort;
    int listenPort;
    QTcpServer *server;
    int nextId;
};

TcpProxy::TcpProxy(QObject *parent) : QObject(parent)
{
        d = new TcpProxyPrivate;
        d->journal = new Journal(this);
        d->nextId = 0;
        d->server = 0;
        d->listenPort = 4433;
        d->targetHost = QString("xmelegance.org");
        d->targetPort = 4433;
}

Journal *TcpProxy::journal()
{
    return d->journal;
}

void TcpProxy::listen()
{
    d->server = new QTcpServer(this);
    connect(d->server, SIGNAL(newConnection()), this, SLOT(handleConnection()));

    bool ok = d->server->listen(QHostAddress::Any, d->listenPort);
    if (!ok) {
        qDebug() << "Unable to listen";
    }
}

void TcpProxy::handleConnection()
{
    while (d->server->hasPendingConnections()) {
        qDebug() << "New connection";
        QTcpSocket *sock = d->server->nextPendingConnection();

        Connection *con = new Connection(d->nextId++, sock, this);

        connect(con, SIGNAL(networkEvent(int, Connection::EventType, Connection::Direction, Connection::State, const QByteArray &)),
                d->journal, SLOT(recordEvent(int, Connection::EventType, Connection::Direction, Connection::State, const QByteArray &)));

        con->connectToHost(d->targetHost, d->targetPort);
    }
}
