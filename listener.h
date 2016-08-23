#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include <QHostAddress>

class TcpServer;

class BitSlinger;
class Journal;

class Listener : public QObject
{
    Q_OBJECT
public:
    enum ProxyType {
        TcpProxy,
        HttpProxy
    };

    enum SslMode {
        SslAutoMode,
        SslStripClientMode,
        SslStripServerMode,
        DumbMode
    };

    struct Config
    {
        ProxyType type;
        QHostAddress listenAddress;
        int listenPort;
        QString targetHost;
        int targetPort;
        SslMode sslMode;

        bool operator==(const Listener::Config &other) const {
            return type == other.type
                    && listenAddress == other.listenAddress
                    && listenPort == other.listenPort
                    && targetHost == other.targetHost
                    && targetPort == other.targetPort
                    && sslMode == other.sslMode;
        }

        bool operator!=(const Listener::Config &other) const {
           return !(*this == other);
         }
    };

    explicit Listener(const Listener::Config &config, QObject *parent = 0);

    Listener::Config config() const { return m_config; }
    void setConfig(const Listener::Config &config);

    BitSlinger *bitSlinger() const { return m_slinger; }
    void setBitSlinger(BitSlinger *slinger);

signals:
    void connectionReceived();

public slots:
    void stopListening();
    bool listen();

private slots:
    void handleConnection();

private:
    Journal *m_journal;
    Listener::Config m_config;
    TcpServer *m_server;
    BitSlinger *m_slinger;
};

#endif // LISTENER_H
