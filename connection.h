#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QNetworkProxy>

class QSslSocket;

class Journal;

class Connection : public QObject
{
    Q_OBJECT
public:
    enum EventType {
        ClientConnectionEvent,
        ServerConnectionEvent,
        ClientDataEvent,
        ServerDataEvent,
        ClientDisconnectionEvent,
        ServerDisconnectionEvent
    };

    explicit Connection(QSslSocket *sock, QObject *parent = 0);

    int id() const { return m_connectionId; }
    void setId(int id) { m_connectionId = id; }
    void setJournal(Journal *journal) { m_journal = journal; }

    void setUpstreamProxy(const QNetworkProxy &upstream);
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
private:
    QSslSocket *m_server;
    QSslSocket *m_client;
    int m_connectionId;
    Journal *m_journal;
    QNetworkProxy m_upstream;
};

#endif // CONNECTION_H
