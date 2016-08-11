#include <QSslSocket>

#include "journal.h"

#include "connection.h"

const qint64 MAX_CHUNK_SIZE = 32*1024; // 32K

Connection::Connection(QSslSocket *sock, QObject *parent)
    : QObject(parent),
      m_server(0),
      m_client(sock),
      m_connectionId(-1),
      m_journal(0)
{
}

void Connection::setUpstreamProxy(const QNetworkProxy &upstream)
{
    m_upstream = upstream;
}

void Connection::connectToHost(const QString &hostname, int port)
{
    m_journal->recordEvent(this, ClientConnectionEvent, QByteArray());

    m_server = new QSslSocket(this);
    m_server->setProxy(m_upstream);
    m_server->connectToHost(hostname, port);

    connect(m_client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
    connect(m_server, SIGNAL(connected()), this, SLOT(connected()));
    connect(m_server, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void Connection::connected()
{
    qDebug() << "Connected to server";
    m_journal->recordEvent(this, ServerConnectionEvent, QByteArray());

    connect(m_client, SIGNAL(readyRead()), this, SLOT(clientData()));
    connect(m_server, SIGNAL(readyRead()), this, SLOT(serverData()));

    // The client may have already sent some data at this point
    if (m_client->bytesAvailable())
        clientData();
}

void Connection::disconnected()
{
    qDebug() << "Disconnected from server";
    m_journal->recordEvent(this, ServerDisconnectionEvent, QByteArray());
    m_client->close();
}

void Connection::clientDisconnected()
{
    qDebug() << "Client has disconnected";
    m_journal->recordEvent(this, ClientDisconnectionEvent, QByteArray());
    m_server->close();
}

QString matchClientHello(const QByteArray &data)
{
    if (data[0] == 0x16 && data[1] == 0x03
        && data[5] == 0x01 && data[9] == 0x03) {
        QString comment("Client hello, record version 3.%1, handshake version 3.%2");
        comment = comment.arg(int(data[2])).arg(int(data[10]));
        return comment;
    }
    return QString();
}

void Connection::clientData()
{
    while (m_client->bytesAvailable()) {
        QByteArray peeked = m_client->peek(qMin(m_client->bytesAvailable(), MAX_CHUNK_SIZE));
        QString comment = matchClientHello(peeked);

        QByteArray data = m_client->read(qMin(m_client->bytesAvailable(), MAX_CHUNK_SIZE));
        qDebug() << ">>>" << data.size() << "bytes";


        m_journal->recordEvent(this, ClientDataEvent, data, comment);
        m_server->write(data);
    }
}

void Connection::serverData()
{
    while (m_server->bytesAvailable()) {
        QByteArray data = m_server->read(qMin(m_server->bytesAvailable(), MAX_CHUNK_SIZE));
        qDebug() << "<<<" << data.size() << "bytes";

        m_journal->recordEvent(this, ServerDataEvent, data);
        m_client->write(data);
    }
}
