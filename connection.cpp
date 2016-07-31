#include <QTcpSocket>

#include "connection.h"

const qint64 MAX_CHUNK_SIZE = 32*1024; // 32K

Connection::Connection(int id, QTcpSocket *sock, QObject *parent) : QObject(parent)
{
    connectionId = id;
    client = sock;
}

void Connection::connectToHost(const QString &hostname, int port)
{
    state = ConnectingState;
    emit networkEvent(connectionId, StateChangeEvent, ClientToServerDirection, state, QByteArray());

    server = new QTcpSocket(this);
    server->connectToHost(hostname, port);

    connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
    connect(server, SIGNAL(connected()), this, SLOT(connected()));
    connect(server, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void Connection::connected()
{
    qDebug() << "Connected to server";
    state = ConnectedState;
    emit networkEvent(connectionId, StateChangeEvent, ClientToServerDirection, state, QByteArray());

    connect(client, SIGNAL(readyRead()), this, SLOT(clientData()));
    connect(server, SIGNAL(readyRead()), this, SLOT(serverData()));

    // The client may have already sent some data at this point
    if (client->bytesAvailable())
        clientData();
}

void Connection::disconnected()
{
    qDebug() << "Disconnected from server";
    state = DisconnectedState;
    emit networkEvent(connectionId, StateChangeEvent, ServerToClientDirection, state, QByteArray());
}

void Connection::clientDisconnected()
{
    qDebug() << "Client has disconnected";
    state = DisconnectingState;
    emit networkEvent(connectionId, StateChangeEvent, ClientToServerDirection, state, QByteArray());

    server->close();
}

void Connection::clientData()
{
    while (client->bytesAvailable()) {
        QByteArray data = client->read(qMin(client->bytesAvailable(), MAX_CHUNK_SIZE));
        qDebug() << ">>>" << data.size() << "bytes";

        emit networkEvent(connectionId, DataEvent, ClientToServerDirection, state, data);
        server->write(data);
    }
}

void Connection::serverData()
{
    while (server->bytesAvailable()) {
        QByteArray data = server->read(qMin(server->bytesAvailable(), MAX_CHUNK_SIZE));
        qDebug() << "<<<" << data.size() << "bytes";

        emit networkEvent(connectionId, DataEvent, ServerToClientDirection, state, data);
        client->write(data);
    }
}
