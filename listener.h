#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include <QHostAddress>
#include <QNetworkProxy>

class QTcpServer;


class Journal;

struct ListenerConfig
{
    QHostAddress listenAddress;
    int listenPort;
    QString targetHost;
    int targetPort;

    bool operator==(const ListenerConfig &other) const {
        return listenAddress == other.listenAddress
                && listenPort == other.listenPort
                && targetHost == other.targetHost
                && targetPort == other.targetPort;
    }

    bool operator!=(const ListenerConfig &other) const {
       return !(*this == other);
     }
};

class Listener : public QObject
{
    Q_OBJECT
public:
    explicit Listener(const ListenerConfig &config, QObject *parent = 0);

    ListenerConfig config() const { return m_config; }
    void setConfig(const ListenerConfig config);

    void setJournal(Journal *journal);
    void setUpstreamProxy(const QNetworkProxy &upstream);

signals:
    void connectionReceived();

public slots:
    void stopListening();
    bool listen();

private slots:
    void handleConnection();

private:
    Journal *m_journal;
    ListenerConfig m_config;
    QTcpServer *m_server;
    QNetworkProxy m_upstream;
};

#endif // LISTENER_H
