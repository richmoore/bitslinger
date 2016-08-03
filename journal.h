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

    JournalEntry *entry(const QModelIndex &index) { return m_events[index.row()]; }

    int addConnection(Connection *con);
    void recordEvent(int connectionId, Connection::EventType type, const QByteArray &content);

signals:

public slots:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
protected:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    int m_nextConnectionId;
    qint64 m_journalStartTime;
    QList<JournalEntry *> m_events;
};

#endif // JOURNAL_H
