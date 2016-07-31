#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>

class QTcpSocket;

class Connection : public QObject
{
    Q_OBJECT
public:
    enum EventType {
        StateChangeEvent,
        DataEvent
    };

    enum Direction {
        ClientToServerDirection,
        ServerToClientDirection,
        NoDirection
    };

    enum State {
        ConnectingState,
        ConnectedState,
        DisconnectingState,
        DisconnectedState
    };

    explicit Connection(int id, QTcpSocket *sock, QObject *parent = 0);

signals:
    void networkEvent(int id, Connection::EventType type, Connection::Direction dir, Connection::State newState, const QByteArray &content);

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
    State state;
};

#endif // CONNECTION_H
