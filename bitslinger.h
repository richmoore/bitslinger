#ifndef BITSLINGER_H
#define BITSLINGER_H

#include <QObject>
#include <QNetworkProxy>

#include "utils/certificategenerator.h"

#include "listener.h"

class QHostAddress;
class QIODevice;

class Journal;
class AbstractTool;

typedef QList<Listener *> ListenerList;
typedef QList<AbstractTool *> AbstractToolList;

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

    void addListener(const Listener::Config &config);
    void editListener(int index, const Listener::Config &config);
    void removeListener(int index);

    bool writeState(QIODevice *output);
    bool readState(QIODevice *input);

    const AbstractToolList tools() const { return m_tools; }

signals:

public slots:
    void loadCaConfig();
    void loadProxyConfig();

    void saveListenerConfig();
    void loadListenerConfig();

private:
    ListenerList m_listeners;
    Journal *m_journal;
    QNetworkProxy m_upstream;
    CertificateGenerator m_certGenerator;
    AbstractToolList m_tools;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(BitSlinger::StateFileFlags)

#endif // BITSLINGER_H
