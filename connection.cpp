#include <QSslSocket>
#include <QSslKey>
#include <QSslCertificate>

#include "bitslinger.h"
#include "journal.h"
#include "utils/certificategenerator.h"

#include "connection.h"

const qint64 MAX_CHUNK_SIZE = 32*1024; // 32K

Connection::Connection(QSslSocket *sock, QObject *parent)
    : QObject(parent),
      m_slinger(0),
      m_serverSslMode(ListenerConfig::AutoSslMode),
      m_server(0),
      m_clientSslMode(ListenerConfig::AutoSslMode),
      m_client(sock),
      m_connectionId(-1),
      m_journal(0)
{
}

void Connection::connectToHost(const QString &hostname, int port)
{
    qDebug() << Q_FUNC_INFO;

    m_journal->recordEvent(this, Journal::ClientConnectionEvent, QByteArray());

    m_server = new QSslSocket(this);
    qDebug() << "Connecting via" << m_slinger->upstreamProxy();
    m_server->setProxy(m_slinger->upstreamProxy());
    m_server->connectToHost(hostname, port);

    connect(m_client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
    connect(m_server, SIGNAL(connected()), this, SLOT(connected()));
    connect(m_server, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void Connection::connected()
{
    qDebug() << "Connected to server";
    m_journal->recordEvent(this, Journal::ServerConnectionEvent, QByteArray());

    connect(m_client, SIGNAL(readyRead()), this, SLOT(clientData()));
    connect(m_server, SIGNAL(readyRead()), this, SLOT(serverData()));

    // The client may have already sent some data at this point
    if (m_client->bytesAvailable())
        clientData();
}

void Connection::disconnected()
{
    qDebug() << "Disconnected from server";
    m_journal->recordEvent(this, Journal::ServerDisconnectionEvent, QByteArray());
    m_client->close();
}

void Connection::clientDisconnected()
{
    qDebug() << "Client has disconnected";
    m_journal->recordEvent(this, Journal::ClientDisconnectionEvent, QByteArray());
    m_server->close();
}

int Connection::findSslClientHello(const QByteArray &data)
{
    //### This should really search the whole message for one
    if (data[0] == 0x16 && data[1] == 0x03
        && data[5] == 0x01 && data[9] == 0x03) {
        qDebug() << QString("Client hello, record version 3.%1, handshake version 3.%2").arg(int(data[2])).arg(int(data[10]));
        return 0;
    }
    return -1;
}

void Connection::clientData()
{
    while (m_client->bytesAvailable()) {
        // Detect SSL client hello
        QByteArray peeked = m_client->peek(qMin(m_client->bytesAvailable(), MAX_CHUNK_SIZE));

        int index = findSslClientHello(peeked);
        // If we found one
        if (index >= 0) {
            qDebug() << "SSL client hello detected";

            m_journal->recordEvent(this, Journal::ClientSwitchedToSslEvent, QByteArray());
            if (m_clientSslMode == ListenerConfig::AutoSslMode) {
                m_server->flush();

                // TODO: we should log these event though we're going to ignore them
                connect(m_server, SIGNAL(sslErrors(QList<QSslError>)), m_server, SLOT(ignoreSslErrors()));
                connect(m_server, SIGNAL(encrypted()), this, SLOT(serverEncrypted()));
                m_server->startClientEncryption();
                qDebug() << "Swtiching to ssl for server connection";

                return; // Force exit without processing the data
            }
        }

        QByteArray data = m_client->read(qMin(m_client->bytesAvailable(), MAX_CHUNK_SIZE));
        qDebug() << ">>>" << data.size() << "bytes";


        m_journal->recordEvent(this, Journal::ClientDataEvent, data);
        m_server->write(data);
    }
}

void Connection::serverData()
{
    while (m_server->bytesAvailable()) {
        QByteArray data = m_server->read(qMin(m_server->bytesAvailable(), MAX_CHUNK_SIZE));
        qDebug() << "<<<" << data.size() << "bytes";

        m_journal->recordEvent(this, Journal::ServerDataEvent, data);
        m_client->write(data);
    }
}

void Connection::serverEncrypted()
{
    m_journal->recordEvent(this, Journal::ServerSwitchedToSslEvent, QByteArray());

    qDebug() << "Server connection is now encrypted, responding to client";
    CertificateGenerator *gen = m_slinger->certificateGenerator();
    QSslCertificate leaf = gen->createClone(m_server->peerCertificate());
    qDebug() << leaf.toText();

    m_client->setPrivateKey(gen->leafKey());
    m_client->setLocalCertificate(leaf);
    m_client->startServerEncryption();
}
