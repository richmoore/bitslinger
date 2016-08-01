#include <QDateTime>
#include <QDebug>

#include "connection.h"

#include "journal.h"

const int COLUMN_TIME = 0;
const int COLUMN_DIRECTION = 1;
const int COLUMN_CONNECTION = 2;
const int COLUMN_TYPE = 3;
const int COLUMN_LENGTH = 4;
const int COLUMN_DETAILS = 5;
const int COLUMN_COMMENT = 6;

const int JOURNAL_COLUMNS = 7;

QString headings[] = {
    "Time",
    "<>",
    "Connection",
    "Type",
    "Length",
    "Details",
    "Comment"
};

Journal::Journal(QObject *parent) : QAbstractTableModel(parent)
{
    journalStartTime = QDateTime::currentMSecsSinceEpoch();
}

void Journal::recordEvent(int id, Connection::EventType type, Connection::Direction dir, Connection::State newState, const QByteArray &content)
{
    JournalEntry *entry = new JournalEntry;
    entry->timestamp = QDateTime::currentMSecsSinceEpoch() - journalStartTime;
    entry->connectionId = id;
    entry->type = type;
    entry->dir = dir;
    entry->newState = newState;
    entry->content = content;

    events.append(entry);
    beginInsertRows(QModelIndex(), events.size(), events.size());
    endInsertRows();
}

int Journal::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return events.size();
}

int Journal::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return JOURNAL_COLUMNS;
}

QVariant Journal::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation != Qt::Horizontal)
        return QVariant();

    if (section >= JOURNAL_COLUMNS)
        return QVariant();

    return headings[section];
}

QVariant Journal::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(index.row() >= events.size() || index.row() < 0)
        return QVariant();

    JournalEntry *entry = events[index.row()];

    if (role == Qt::DisplayRole) {
        switch(index.column()) {
        case COLUMN_TIME:
            return entry->timestamp;
        case COLUMN_DIRECTION:
            return QString("<>");
        case COLUMN_CONNECTION:
            return entry->connectionId;
        case COLUMN_TYPE:
            if (entry->type == Connection::DataEvent)
                return QString("Data");
            else
                return QString("State");
        case COLUMN_LENGTH:
            return entry->content.size();
        case COLUMN_DETAILS:
            if (entry->type == Connection::DataEvent)
                return entry->content.toHex();
            if (entry->type == Connection::StateChangeEvent) {
                switch (entry->newState) {
                case Connection::ConnectingState:
                    return QString("Connecting");
                case Connection::ConnectedState:
                    return QString("Connected");
                case Connection::DisconnectingState:
                    return QString("Disconnecting");
                case Connection::DisconnectedState:
                    return QString("Disconnected");
                }
            }
        case COLUMN_COMMENT:
            return entry->comment;
        }
    }

    if (role == Qt::BackgroundColorRole) {
        if (entry->color.isValid())
            return entry->color;
    }

    return QVariant();
}
