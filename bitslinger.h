#ifndef BITSLINGER_H
#define BITSLINGER_H

#include <QObject>
#include <QNetworkProxy>

#include "listener.h"

class QHostAddress;

class Listener;
class Journal;

typedef QList<Listener *> ListenerList;

class BitSlinger : public QObject
{
    Q_OBJECT
public:
    explicit BitSlinger(QObject *parent = 0);

    ListenerList listeners() const { return m_listeners; }
    Journal *journal() const { return m_journal; }
    void setUpstreamProxy(const QNetworkProxy &upstream);

    void addListener(const ListenerConfig &config);
    void editListener(int index, const ListenerConfig &config);
    void removeListener(int index);

signals:

public slots:
private:
    ListenerList m_listeners;
    Journal *m_journal;
    QNetworkProxy m_upstream;
};

#endif // BITSLINGER_H
