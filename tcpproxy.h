#ifndef TCPPROXY_H
#define TCPPROXY_H

#include <QObject>
#include <QHostAddress>
#include <QNetworkProxy>

class QTcpServer;


class Journal;

struct TcpProxyConfig
{
    QHostAddress listenAddress;
    int listenPort;
    QString targetHost;
    int targetPort;
};

class TcpProxy : public QObject
{
    Q_OBJECT
public:
    explicit TcpProxy(const TcpProxyConfig &config, QObject *parent = 0);

    void setJournal(Journal *journal);
    void setUpstreamProxy(const QNetworkProxy &upstream);

signals:
    void connectionReceived();

public slots:
    bool listen();

private slots:
    void handleConnection();

private:
    Journal *m_journal;
    TcpProxyConfig m_config;
    QTcpServer *m_server;
    QNetworkProxy m_upstream;
};

#endif // TCPPROXY_H
