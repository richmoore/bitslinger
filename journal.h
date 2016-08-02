#ifndef JOURNAL_H
#define JOURNAL_H

#include <QAbstractTableModel>
#include <QList>
#include <QColor>

#include "connection.h"

//### We need to be able to lookup the host etc. too
struct JournalEntry
{
    qint64 timestamp;
    int connectionId;
    Connection::EventType type;
    QByteArray content;
    QColor color;
    QString comment;
};

class Journal : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit Journal(QObject *parent = 0);

    JournalEntry *entry(const QModelIndex &index) { return events[index.row()]; }

signals:

public slots:
    //### Change this to a direct function call?
    void connectionEvent(int id, Connection::EventType type, const QByteArray &content);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
protected:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    qint64 journalStartTime;
    QList<JournalEntry *> events;
};

#endif // JOURNAL_H
