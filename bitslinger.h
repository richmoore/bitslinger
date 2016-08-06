#ifndef BITSLINGER_H
#define BITSLINGER_H

#include <QObject>
#include <QNetworkProxy>

class QHostAddress;

class Listener;
class Journal;

typedef QList<Listener *> TcpProxyList;

class BitSlinger : public QObject
{
    Q_OBJECT
public:
    explicit BitSlinger(QObject *parent = 0);

    TcpProxyList proxies() const { return m_proxies; }
    Journal *journal() const { return m_journal; }
    void setUpstreamProxy(const QNetworkProxy &upstream);

    bool addProxy(const QHostAddress &listenAddress, int listenPort, const QString &server, int serverPort);

signals:

public slots:
private:
    TcpProxyList m_proxies;
    Journal *m_journal;
    QNetworkProxy m_upstream;
};

#endif // BITSLINGER_H
