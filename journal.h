#ifndef JOURNAL_H
#define JOURNAL_H

#include <QAbstractTableModel>
#include <QList>
#include <QColor>

#include "connection.h"

struct JournalConnection
{
    QHostAddress clientAddress;
    QHostAddress listenAddress;
    int listenPort;
    QHostAddress targetAddress;
    int targetPort;
};

//### We need to be able to lookup the host etc. too
struct JournalEvent
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
    enum CustomRoles {
        ContextMenuRole = Qt::UserRole
    };

    explicit Journal(QObject *parent = 0);

    JournalEvent *entry(const QModelIndex &index) { return m_events[index.row()]; }

    void addConnection(Connection *con);
    void recordEvent(Connection *con, Connection::EventType type, const QByteArray &content);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;

signals:

public slots:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

protected:

private:
    int m_nextConnectionId;
    qint64 m_journalStartTime;
    QList<JournalConnection *> m_connections;
    QList<JournalEvent *> m_events;
};

#endif // JOURNAL_H
