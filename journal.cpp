#include <QDateTime>
#include <QDebug>
#include <QIcon>
#include <QSslSocket>

#include "connection.h"
#include "utils/colorutils.h"

#include "journal.h"

enum Columns {
    COLUMN_ID,
    COLUMN_TIME,
    COLUMN_DIRECTION,
    COLUMN_CONNECTION,
    COLUMN_TYPE,
    COLUMN_LENGTH,
    COLUMN_DETAILS,
    COLUMN_COMMENT
};

QString headings[] = {
    "Id",
    "Time",
    "Direction",
    "Connection",
    "Type",
    "Length",
    "Details",
    "Comment"
};

const int JOURNAL_COLUMNS = 8;

Journal::Journal(QObject *parent)
    : QAbstractTableModel(parent),
      m_nextConnectionId(0),
      m_journalStartTime(QDateTime::currentMSecsSinceEpoch())
{
}

void Journal::addConnection(Connection *con)
{
    con->setJournal(this);
    con->setId(m_nextConnectionId++);

    // We can't record the server end yet as we haven't established the upstream connection
    JournalConnection *jcon = new JournalConnection;
    jcon->clientAddress = con->clientSocket()->peerAddress();
    jcon->listenAddress = con->clientSocket()->localAddress();
    jcon->listenPort = con->clientSocket()->localPort();

    m_connections.append(jcon);
}

void Journal::recordEvent(Connection *con, Journal::EventType type, const QByteArray &content, const QString &comment)
{
    if (type == ServerConnectionEvent) {
        JournalConnection *jcon = m_connections[con->id()];
        jcon->targetAddress = con->serverSocket()->peerAddress();
        jcon->targetPort = con->serverSocket()->peerPort();
    }

    JournalEvent *entry = new JournalEvent;
    entry->timestamp = QDateTime::currentMSecsSinceEpoch() - m_journalStartTime;
    entry->connectionId = con->id();
    entry->type = type;
    entry->content = content;
    entry->comment = comment; //### Comment should only be used for user data!!!

    //### Hack
    //entry->color = ColorUtils::autoBackgroundColor(0, con->id());

    beginInsertRows(QModelIndex(), m_events.size(), m_events.size());
    m_events.append(entry);
    endInsertRows();
}

int Journal::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_events.size();
}

int Journal::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return JOURNAL_COLUMNS;
}

QVariant Journal::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::DecorationRole && role != ContextMenuRole)
        return QVariant();

    if (section >= JOURNAL_COLUMNS)
        return QVariant();

    if (section == COLUMN_DIRECTION) {
        if (role == Qt::DecorationRole) {
            return QIcon(":icons/arrow_bidi.svg");
        }
        if (role == Qt::DisplayRole)
            return QVariant();
    }

    return headings[section];
}

Qt::ItemFlags Journal::flags(const QModelIndex &index) const
{
    if (index.column() == COLUMN_COMMENT)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemNeverHasChildren | Qt::ItemIsEditable;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
}

QVariant Journal::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(index.row() >= m_events.size() || index.row() < 0)
        return QVariant();

    JournalEvent *entry = m_events[index.row()];

    if (role == Qt::DisplayRole) {
        switch(index.column()) {
        case COLUMN_ID:
            return index.row();
        case COLUMN_TIME:
            return entry->timestamp;
        case COLUMN_DIRECTION:
            return QVariant();
        case COLUMN_CONNECTION:
            return entry->connectionId;
        case COLUMN_TYPE:
            switch(entry->type) {
            case ClientConnectionEvent:
                return tr("Client Connection");
            case ServerConnectionEvent:
                return tr("Server Connection");
            case ClientDataEvent:
                return tr("Client Data");
            case ServerDataEvent:
                return tr("Server Data");
            case ClientDisconnectionEvent:
                return tr("Client Disconnection");
            case ServerDisconnectionEvent:
                return tr("Server Disconnection");
            case ClientNoteEvent:
                return tr("Client Note");
            case ServerNoteEvent:
                return tr("Server Note");
            case ClientSwitchedToSslEvent:
                return tr("Client Switch to SSL");
            case ServerSwitchedToSslEvent:
                return tr("Server switched to SSL");
            default:
                qDebug() << "Unknown entry type" << entry->type;
                return tr("Unknown");
            }
        case COLUMN_LENGTH:
            return entry->content.size();
        case COLUMN_DETAILS:
            if (entry->type == ClientDataEvent || entry->type == ServerDataEvent)
                return entry->content.toHex();
            else if (entry->type == ClientConnectionEvent) {
                return tr("Connection from %1").arg(m_connections[entry->connectionId]->clientAddress.toString());
            }
            else if (entry->type == ServerConnectionEvent) {
                return tr("Connected to %1:%2")
                        .arg(m_connections[entry->connectionId]->targetAddress.toString())
                        .arg(m_connections[entry->connectionId]->targetPort);
            }
            else if (entry->type == ClientDisconnectionEvent) {
                return tr("Disconnection from %1").arg(m_connections[entry->connectionId]->clientAddress.toString());
            }
            else if (entry->type == ServerDisconnectionEvent) {
                return tr("Disconnected from %1:%2")
                        .arg(m_connections[entry->connectionId]->targetAddress.toString())
                        .arg(m_connections[entry->connectionId]->targetPort);
            }
            break;
        case COLUMN_COMMENT:
            return entry->comment;
        default:
            qDebug() << "Unknown column for display role" << index.column();
            return QVariant();
        }
    }
    else if (role == Qt::DecorationRole) {
        if (index.column() == COLUMN_DIRECTION) {
            switch (entry->type) {
            case ClientDataEvent:
            case ClientNoteEvent: // TODO: new icon
                return QIcon(":icons/client2server.svg");
            case ClientSwitchedToSslEvent:
                return QIcon(":icons/lock.svg");
            case ServerDataEvent:
            case ServerNoteEvent: // TODO: new icon
                return QIcon(":icons/server2client.svg");
            case ServerSwitchedToSslEvent:
                return QIcon(":icons/lock.svg");
            case ClientConnectionEvent:
                return QIcon(":icons/client2server_connected.svg");
            case ServerConnectionEvent:
                return QIcon(":icons/server2client_connected.svg");
            case ClientDisconnectionEvent:
                return QIcon(":icons/client2server_disconnected.svg");
            case ServerDisconnectionEvent:
                return QIcon(":icons/server2client_disconnected.svg");
            default:
                qDebug() << "Unknown direction" << entry->type;
                return tr("Unknown");
            }
        }
    }
    else if (role == Qt::EditRole) {
        if (index.column() == COLUMN_COMMENT)
            return entry->comment;
    }
    else if (role == Qt::BackgroundColorRole) {
        if (entry->color.isValid())
            return entry->color;
    }

    return QVariant();
}

bool Journal::setData(const QModelIndex &index, const QVariant &value, int role)
{
    qDebug() << "setData called" << value;
    if (!index.isValid())
        return false;

    if(index.row() >= m_events.size() || index.row() < 0)
        return false;

    JournalEvent *entry = m_events[index.row()];

    if (role != Qt::EditRole)
        return false;

    if (index.column() == COLUMN_COMMENT) {
        entry->comment = value.toString();

        QVector<int> roles;
        roles << role;
        emit dataChanged(index, index, roles);
        return true;
    }

    return false;
}


QDataStream &operator<<(QDataStream &stream, const JournalConnection *connection)
{
    qDebug() << "writing connection";
    stream << connection->clientAddress;
    stream << connection->listenAddress;
    stream << connection->listenPort;
    stream << connection->targetAddress;
    stream << connection->targetPort;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, JournalConnection *&connection)
{
    qDebug() << "reading connection";
    connection = new JournalConnection;
    stream >> connection->clientAddress;
    stream >> connection->listenAddress;
    stream >> connection->listenPort;
    stream >> connection->targetAddress;
    stream >> connection->targetPort;

    return stream;
}

QDataStream &operator<<(QDataStream &stream, const JournalEvent *event)
{
    qDebug() << "writing event";
    stream << event->timestamp;
    stream << event->connectionId;
    stream << quint16(event->type);
    stream << event->flags;
    stream << event->content;
    stream << event->editedContent;
    stream << event->extraData;
    stream << event->color;
    stream << event->comment;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, JournalEvent *&event)
{
    qDebug() << "reading event";
    event = new JournalEvent;
    stream >> event->timestamp;
    stream >> event->connectionId;
    quint16 eventType;
    stream >> eventType;
    event->type = Journal::EventType(eventType);
    quint32 flags;
    stream >> flags;
    event->flags = JournalEvent::JournalEventFlags(flags);
    stream >> event->content;
    stream >> event->editedContent;
    stream >> event->extraData;
    stream >> event->color;
    stream >> event->comment;

    return stream;
}

QDataStream &operator<<(QDataStream &stream, const Journal &journal)
{
    qDebug() << "writing journal";
    stream << journal.m_nextConnectionId;
    stream << journal.m_journalStartTime;
    stream << journal.m_connections;
    stream << journal.m_events;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, Journal &journal)
{
    journal.beginResetModel();
    qDebug() << "reading journal";
    stream >> journal.m_nextConnectionId;
    stream >> journal.m_journalStartTime;
    qDebug() << "reading connections";
    stream >> journal.m_connections;
    qDebug() << "reading events";
    stream >> journal.m_events;
    journal.endResetModel();

    return stream;
}
