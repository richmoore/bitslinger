#include <QSslSocket>
#include <QDebug>

#include "connection.h"
#include "journal.h"
#include "bitslinger.h"
#include "utils/tcpserver.h"

#include "listener.h"

Listener::Listener(const Listener::Config &config, QObject *parent)
    : QObject(parent),
      m_journal(0),
      m_config(config),
      m_server(0)
{
}

void Listener::setConfig(const Listener::Config &config)
{
    if (m_config == config)
        return;

    m_config = config;
    if (m_server) {
        stopListening();
        listen();
    }
}

void Listener::setBitSlinger(BitSlinger *slinger)
{
    m_slinger = slinger;
    m_journal = slinger->journal();
}

void Listener::stopListening()
{
    m_server->close();
    m_server->deleteLater();
    m_server = 0;
}

bool Listener::listen()
{
    m_server = new TcpServer(this);
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
        QSslSocket *sock = m_server->nextPendingConnection();

        Connection *con = new Connection(sock, this);
        con->setSslMode(m_config.sslMode);
        con->setProxyType(m_config.type);
        con->setBitSlinger(m_slinger);
        m_journal->addConnection(con);

        if (m_config.type == Listener::HttpProxy)
            con->startHttpProxy();
        else
            con->connectToHost(m_config.targetHost, m_config.targetPort);
    }
}
