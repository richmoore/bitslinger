#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

#include "connection.h"
#include "journal.h"

#include "listener.h"

Listener::Listener(const ListenerConfig &config, QObject *parent)
    : QObject(parent),
      m_journal(0),
      m_config(config),
      m_server(0)
{
}

void Listener::setConfig(const ListenerConfig config)
{
    if (m_config == config)
        return;

    m_config = config;
    if (m_server) {
        stopListening();
        listen();
    }
}

void Listener::setJournal(Journal *journal)
{
    m_journal = journal;
}

void Listener::setUpstreamProxy(const QNetworkProxy &upstream)
{
    m_upstream = upstream;
}

void Listener::stopListening()
{
    m_server->close();
    m_server->deleteLater();
    m_server = 0;
}

bool Listener::listen()
{
    m_server = new QTcpServer(this);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(handleConnection()));

    bool ok = m_server->listen(m_config.listenAddress, m_config.listenPort);
    if (!ok) {
        qDebug() << "Unable to listen" << m_server->errorString();
    }
    return ok;
}

void Listener::handleConnection()
{
    while (m_server->hasPendingConnections()) {
        qDebug() << "New connection";
        QTcpSocket *sock = m_server->nextPendingConnection();

        Connection *con = new Connection(sock, this);
        con->setUpstreamProxy(m_upstream);
        m_journal->addConnection(con);

        con->connectToHost(m_config.targetHost, m_config.targetPort);
    }
}
