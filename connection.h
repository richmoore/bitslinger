#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>

#include "listener.h"

class QSslSocket;

class BitSlinger;
class Journal;
class HttpProxyRequestHandler;

class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(QSslSocket *sock, QObject *parent = 0);

    void setProxyType(Listener::ProxyType type) { m_proxyType = type; }
    void setSslMode(Listener::SslMode mode) { m_sslMode = mode; }

    BitSlinger *bitSlinger() const { return m_slinger; }
    void setBitSlinger(BitSlinger *slinger) { m_slinger = slinger; }

    int id() const { return m_connectionId; }
    void setId(int id) { m_connectionId = id; }
    void setJournal(Journal *journal) { m_journal = journal; }

    QSslSocket *serverSocket() const { return m_server; }
    QSslSocket *clientSocket() const { return m_client; }
signals:

public slots:
    void startHttpProxy();

    void connectToHost(const QString &server, int port);

    void connected();
    void disconnected();
    void clientDisconnected();

    void clientData();
    void serverData();

    void serverEncrypted();

protected slots:
    void proxyRequestReady();
    void encryptClientConnection();

protected:
    void transferClientData(int bytes);

private:
    int findSslClientHello(const QByteArray &data);

private:
    BitSlinger *m_slinger;
    QSslSocket *m_server;
    QSslSocket *m_client;
    Listener::ProxyType m_proxyType;
    Listener::SslMode m_sslMode;
    int m_connectionId;
    Journal *m_journal;
    HttpProxyRequestHandler *m_httpProxyHandler;
};

#endif // CONNECTION_H
