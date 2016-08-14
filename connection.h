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

    void setSslMode(ListenerConfig::SslMode mode) { m_sslMode = mode; }

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

protected slots:
    void encryptClientConnection();

private:
    int findSslClientHello(const QByteArray &data);

private:
    BitSlinger *m_slinger;
    QSslSocket *m_server;
    QSslSocket *m_client;
    ListenerConfig::SslMode m_sslMode;
    int m_connectionId;
    Journal *m_journal;
};

#endif // CONNECTION_H
