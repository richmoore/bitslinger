#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>

class QTcpSocket;

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

    explicit Connection(QTcpSocket *sock, QObject *parent = 0);

    void setId(int id) { m_connectionId = id; }
    void setJournal(Journal *journal) { m_journal = journal; }

signals:

public slots:
    void connectToHost(const QString &server, int port);

    void connected();
    void disconnected();
    void clientDisconnected();

    void clientData();
    void serverData();
private:
    QTcpSocket *m_server;
    QTcpSocket *m_client;
    int m_connectionId;
    Journal *m_journal;
};

#endif // CONNECTION_H
