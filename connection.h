#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>

class QTcpSocket;

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

    explicit Connection(int id, QTcpSocket *sock, QObject *parent = 0);

signals:
    void connectionEvent(int id, Connection::EventType type, const QByteArray &content);

public slots:
    void connectToHost(const QString &server, int port);

    void connected();
    void disconnected();
    void clientDisconnected();

    void clientData();
    void serverData();
private:
    QTcpSocket *server;
    QTcpSocket *client;
    int connectionId;
};

#endif // CONNECTION_H
