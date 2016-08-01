#include <QDateTime>
#include <QDebug>

#include "connection.h"

#include "journal.h"

const int JOURNAL_COLUMNS = 7;

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

    switch(section) {
    case 0:
        return QString("Time");
    case 1:
        return QString("<>");
    case 2:
        return QString("Connection");
    case 3:
        return QString("Type");
    case 4:
        return QString("Length");
    case 5:
        return QString("Details");
    case 6:
        return QString("Comment");
    }

    return QAbstractTableModel::headerData(section, orientation, role);
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
        case 0:
            return entry->timestamp;
        case 1:
            return QString("<>");
        case 2:
            return entry->connectionId;
        case 3:
            if (entry->type == Connection::DataEvent)
                return QString("Data");
            else
                return QString("State");
        case 4:
            return entry->content.size();
        case 5:
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
        case 6:
            return entry->comment;
        }
    }

    if (role == Qt::BackgroundColorRole) {
        if (entry->color.isValid())
            return entry->color;
    }

    return QVariant();
}
