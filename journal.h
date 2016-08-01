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
    Connection::Direction dir;
    Connection::State newState;
    QByteArray content;
    QColor color;
    QString comment;
};

class Journal : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit Journal(QObject *parent = 0);

signals:

public slots:
    //### Change this to a direct function call?
    void recordEvent(int id, Connection::EventType type, Connection::Direction dir, Connection::State newState, const QByteArray &content);

protected:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    QList<JournalEntry *> events;
};

#endif // JOURNAL_H
