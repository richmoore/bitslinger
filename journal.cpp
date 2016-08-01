#include <QDateTime>

#include "journal.h"

Journal::Journal(QObject *parent) : QObject(parent)
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

    events.append(entry);
}
