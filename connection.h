#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>

#include "listener.h"

class QSslSocket;

class BitSlinger;
class Journal;

class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(QSslSocket *sock, QObject *parent = 0);

    void setServerSslMode(ListenerConfig::SslMode mode) { m_serverSslMode = mode; }
    void setClientSslMode(ListenerConfig::SslMode mode) { m_clientSslMode = mode; }

    BitSlinger *bitSlinger() const { return m_slinger; }
    void setBitSlinger(BitSlinger *slinger) { m_slinger = slinger; }

    int id() const { return m_connectionId; }
    void setId(int id) { m_connectionId = id; }
    void setJournal(Journal *journal) { m_journal = journal; }

    QSslSocket *serverSocket() const { return m_server; }
    QSslSocket *clientSocket() const { return m_client; }
signals:

public slots:
    void connectToHost(const QString &server, int port);

    void connected();
    void disconnected();
    void clientDisconnected();

    void clientData();
    void serverData();

    void serverEncrypted();

private:
    int findSslClientHello(const QByteArray &data);

private:
    BitSlinger *m_slinger;
    ListenerConfig::SslMode m_serverSslMode;
    QSslSocket *m_server;
    ListenerConfig::SslMode m_clientSslMode;
    QSslSocket *m_client;
    int m_connectionId;
    Journal *m_journal;
};

#endif // CONNECTION_H
