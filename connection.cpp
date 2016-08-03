#include <QTcpSocket>

#include "journal.h"

#include "connection.h"

const qint64 MAX_CHUNK_SIZE = 32*1024; // 32K

Connection::Connection(QTcpSocket *sock, QObject *parent)
    : QObject(parent),
      m_server(0),
      m_client(sock),
      m_connectionId(-1),
      m_journal(0)
{
}

void Connection::connectToHost(const QString &hostname, int port)
{
    m_journal->recordEvent(m_connectionId, ClientConnectionEvent, QByteArray());

    m_server = new QTcpSocket(this);
    m_server->connectToHost(hostname, port);

    connect(m_client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
    connect(m_server, SIGNAL(connected()), this, SLOT(connected()));
    connect(m_server, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void Connection::connected()
{
    qDebug() << "Connected to server";
    m_journal->recordEvent(m_connectionId, ServerConnectionEvent, QByteArray());

    connect(m_client, SIGNAL(readyRead()), this, SLOT(clientData()));
    connect(m_server, SIGNAL(readyRead()), this, SLOT(serverData()));

    // The client may have already sent some data at this point
    if (m_client->bytesAvailable())
        clientData();
}

void Connection::disconnected()
{
    qDebug() << "Disconnected from server";
    m_journal->recordEvent(m_connectionId, ServerDisconnectionEvent, QByteArray());
    m_client->close();
}

void Connection::clientDisconnected()
{
    qDebug() << "Client has disconnected";
    m_journal->recordEvent(m_connectionId, ClientDisconnectionEvent, QByteArray());
    m_server->close();
}

void Connection::clientData()
{
    while (m_client->bytesAvailable()) {
        QByteArray data = m_client->read(qMin(m_client->bytesAvailable(), MAX_CHUNK_SIZE));
        qDebug() << ">>>" << data.size() << "bytes";

        m_journal->recordEvent(m_connectionId, ClientDataEvent, data);
        m_server->write(data);
    }
}

void Connection::serverData()
{
    while (m_server->bytesAvailable()) {
        QByteArray data = m_server->read(qMin(m_server->bytesAvailable(), MAX_CHUNK_SIZE));
        qDebug() << "<<<" << data.size() << "bytes";

        m_journal->recordEvent(m_connectionId, ServerDataEvent, data);
        m_client->write(data);
    }
}
