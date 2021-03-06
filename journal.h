#ifndef JOURNAL_H
#define JOURNAL_H

#include <QAbstractTableModel>
#include <QList>
#include <QColor>
#include <QDataStream>
#include <QHostAddress>

#include "connection.h"

struct JournalConnection;
struct JournalEvent;

class Journal : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum EventType {
        ClientConnectionEvent,
        ServerConnectionEvent,
        ClientDataEvent,
        ServerDataEvent,
        ClientDisconnectionEvent,
        ServerDisconnectionEvent,
        ClientNoteEvent,
        ServerNoteEvent,
        ClientSwitchedToSslEvent,
        ServerSwitchedToSslEvent
    };

    enum CustomRoles {
        ContextMenuRole = Qt::UserRole
    };

    explicit Journal(QObject *parent = 0);

    JournalEvent *entry(const QModelIndex &index) { return m_events[index.row()]; }

    void addConnection(Connection *con);
    void recordEvent(Connection *con, Journal::EventType type, const QByteArray &content, const QString &comment=QString());

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

signals:

public slots:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

protected:

private:
    int m_nextConnectionId;
    qint64 m_journalStartTime;
    QList<JournalConnection *> m_connections;
    QList<JournalEvent *> m_events;

public:
    friend QDataStream &operator<<(QDataStream &stream, const Journal &journal);
    friend QDataStream &operator>>(QDataStream &stream, Journal &journal);

};

struct JournalConnection
{
    QHostAddress clientAddress;
    QHostAddress listenAddress;
    int listenPort;
    QHostAddress targetAddress;
    int targetPort;
};

struct JournalEvent
{
    enum JournalEventFlag {
        JournalEventWasEdited = 0x1
    };
    Q_DECLARE_FLAGS(JournalEventFlags, JournalEventFlag)

    qint64 timestamp;
    int connectionId;
    Journal::EventType type;
    JournalEventFlags flags;
    QByteArray content;
    QByteArray editedContent;
    QByteArray extraData;
    QColor color;
    QString comment;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(JournalEvent::JournalEventFlags)

QDataStream &operator<<(QDataStream &stream, const JournalConnection *connection);
QDataStream &operator>>(QDataStream &stream, JournalConnection *&connection);

QDataStream &operator<<(QDataStream &stream, const JournalEvent *event);
QDataStream &operator>>(QDataStream &stream, JournalEvent *&event);

#endif // JOURNAL_H
