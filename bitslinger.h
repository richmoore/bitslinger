#ifndef BITSLINGER_H
#define BITSLINGER_H

#include <QObject>

class QHostAddress;

class TcpProxy;
class Journal;

typedef QList<TcpProxy *> TcpProxyList;

class BitSlinger : public QObject
{
    Q_OBJECT
public:
    explicit BitSlinger(QObject *parent = 0);

    Journal *journal() const { return m_journal; }

    bool addProxy(const QHostAddress &listenAddress, int listenPort, const QString &server, int serverPort);

signals:

public slots:
private:
    TcpProxyList m_proxies;
    Journal *m_journal;
};

#endif // BITSLINGER_H
