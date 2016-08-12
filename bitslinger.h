#ifndef BITSLINGER_H
#define BITSLINGER_H

#include <QObject>
#include <QNetworkProxy>

#include "listener.h"
#include "utils/certificategenerator.h"

class QHostAddress;
class QIODevice;

class Listener;
class Journal;

typedef QList<Listener *> ListenerList;

class BitSlinger : public QObject
{
    Q_OBJECT
public:
    enum StateFileFlag {
        StateFileHasJournal = 0x1,
        StateFileHasListeners = 0x2,
        StateFileHasUi = 0x4
    };
    Q_DECLARE_FLAGS(StateFileFlags, StateFileFlag)

    explicit BitSlinger(QObject *parent = 0);

    CertificateGenerator *certificateGenerator() { return &m_certGenerator; }

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

    void loadCaConfig();

signals:

public slots:
private:
    ListenerList m_listeners;
    Journal *m_journal;
    QNetworkProxy m_upstream;
    CertificateGenerator m_certGenerator;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(BitSlinger::StateFileFlags)

#endif // BITSLINGER_H
