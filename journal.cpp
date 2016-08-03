#include <QDateTime>
#include <QDebug>
#include <QIcon>

#include "connection.h"

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
    "<>",
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

int Journal::addConnection(Connection *con)
{
    return m_nextConnectionId++;
}

void Journal::recordEvent(int id, Connection::EventType type, const QByteArray &content)
{
    JournalEntry *entry = new JournalEntry;
    entry->timestamp = QDateTime::currentMSecsSinceEpoch() - m_journalStartTime;
    entry->connectionId = id;
    entry->type = type;
    entry->content = content;

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

    if (role != Qt::DisplayRole && role != Qt::DecorationRole)
        return QVariant();

    if (section >= JOURNAL_COLUMNS)
        return QVariant();

    if (section == COLUMN_DIRECTION) {
        if (role == Qt::DecorationRole)
            return QIcon(":icons/arrow_bidi.svg");
        else
            return QVariant();
    }

    return headings[section];
}

QVariant Journal::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(index.row() >= m_events.size() || index.row() < 0)
        return QVariant();

    JournalEntry *entry = m_events[index.row()];

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
            case Connection::ClientConnectionEvent:
                return QString("Client Connection");
            case Connection::ServerConnectionEvent:
                return QString("Server Connection");
            case Connection::ClientDataEvent:
                return QString("Client Data");
            case Connection::ServerDataEvent:
                return QString("Server Data");
            case Connection::ClientDisconnectionEvent:
                return QString("Client Disconnection");
            case Connection::ServerDisconnectionEvent:
                return QString("Server Disconnection");
            default:
                qDebug() << "Unknown entry type" << entry->type;
                return QString("Unknown");
            }
        case COLUMN_LENGTH:
            return entry->content.size();
        case COLUMN_DETAILS:
            if (entry->type == Connection::ClientDataEvent || entry->type == Connection::ServerDataEvent)
                return entry->content.toHex();
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
            case Connection::ClientDataEvent:
                return QIcon(":icons/client2server.svg");
            case Connection::ServerDataEvent:
                return QIcon(":icons/server2client.svg");
            case Connection::ClientConnectionEvent:
                return QIcon(":icons/client2server_connected.svg");
            case Connection::ServerConnectionEvent:
                return QIcon(":icons/server2client_connected.svg");
            case Connection::ClientDisconnectionEvent:
                return QIcon(":icons/client2server_disconnected.svg");
            case Connection::ServerDisconnectionEvent:
                return QIcon(":icons/server2client_disconnected.svg");
            default:
                qDebug() << "Unknown direction" << entry->type;
                return QString("Unknown");
            }
        }
    }
    else if (role == Qt::BackgroundColorRole) {
        if (entry->color.isValid())
            return entry->color;
    }

    return QVariant();
}
