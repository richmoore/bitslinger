#ifndef BITSLINGER_H
#define BITSLINGER_H

#include <QObject>
#include <QNetworkProxy>

#include "listener.h"

class QHostAddress;
class QIODevice;

class Listener;
class Journal;

typedef QList<Listener *> ListenerList;

class BitSlinger : public QObject
{
    Q_OBJECT
public:
    explicit BitSlinger(QObject *parent = 0);

    Listener *listenerAt(int index) const { return m_listeners[index]; }
    ListenerList listeners() const { return m_listeners; }
    Journal *journal() const { return m_journal; }

    QNetworkProxy upstreamProxy() const { return m_upstream; }
    void setUpstreamProxy(const QNetworkProxy &upstream);

    void addListener(const ListenerConfig &config);
    void editListener(int index, const ListenerConfig &config);
    void removeListener(int index);

    bool writeState(QIODevice *output);
    bool readState(QIODevice *input);

signals:

public slots:
private:
    ListenerList m_listeners;
    Journal *m_journal;
    QNetworkProxy m_upstream;
};

#endif // BITSLINGER_H
