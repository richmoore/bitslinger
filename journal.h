#ifndef JOURNAL_H
#define JOURNAL_H

#include <QObject>
#include <QList>

#include "connection.h"

struct JournalEntry
{
    int connectionId;
    Connection::EventType type;
    Connection::Direction dir;
    Connection::State newState;
    QByteArray content;
};

class Journal : public QObject
{
    Q_OBJECT
public:
    explicit Journal(QObject *parent = 0);

signals:

public slots:
    void recordEvent(int id, Connection::EventType type, Connection::Direction dir, Connection::State newState, const QByteArray &content);

public:
    QList<JournalEntry *> events;
};

#endif // JOURNAL_H
