#include <QDateTime>
#include <QDebug>

#include "journal.h"

const int JOURNAL_COLUMNS = 6;

Journal::Journal(QObject *parent) : QAbstractTableModel(parent)
{

}

void Journal::recordEvent(int id, Connection::EventType type, Connection::Direction dir, Connection::State newState, const QByteArray &content)
{
    JournalEntry *entry = new JournalEntry;
    entry->timestamp = QDateTime::currentMSecsSinceEpoch();
    entry->connectionId = id;
    entry->type = type;
    entry->dir = dir;
    entry->newState = newState;
    entry->content = content;

    qDebug() << "X";

    events.append(entry);
    beginInsertRows(QModelIndex(), events.size(), events.size());
    endInsertRows();
}

int Journal::rowCount(const QModelIndex &parent) const
{
    qDebug() << "rowCount" << events.size();
    return events.size();
}

int Journal::columnCount(const QModelIndex &parent) const
{
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
        return QString("Length");
    case 4:
        return QString("Details");
    case 5:
        return QString("Comment");
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

QVariant Journal::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        return QString("test");
    }

    return QVariant();
}
