#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

#include "connection.h"
#include "journal.h"

#include "tcpproxy.h"

TcpProxy::TcpProxy(const TcpProxyConfig &config, QObject *parent)
    : QObject(parent)
{
    m_journal = 0;
    m_nextId = 0;
    m_config = config;
}

void TcpProxy::setJournal(Journal *journal)
{
    m_journal = journal;
}

bool TcpProxy::listen()
{
    m_server = new QTcpServer(this);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(handleConnection()));

    bool ok = m_server->listen(m_config.listenAddress, m_config.listenPort);
    if (!ok) {
        qDebug() << "Unable to listen" << m_server->errorString();
    }
    return ok;
}

void TcpProxy::handleConnection()
{
    while (m_server->hasPendingConnections()) {
        qDebug() << "New connection";
        QTcpSocket *sock = m_server->nextPendingConnection();

        Connection *con = new Connection(m_nextId++, sock, this);

        connect(con, SIGNAL(connectionEvent(int, Connection::EventType, const QByteArray &)),
                m_journal, SLOT(connectionEvent(int, Connection::EventType, const QByteArray &)));

        con->connectToHost(m_config.targetHost, m_config.targetPort);
    }
}
