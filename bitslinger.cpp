#include <QHostAddress>

#include "journal.h"
#include "tcpproxy.h"

#include "bitslinger.h"

BitSlinger::BitSlinger(QObject *parent) : QObject(parent)
{
    m_journal = new Journal(this);
}

bool BitSlinger::addProxy(const QHostAddress &listenAddress, int listenPort, const QString &server, int serverPort)
{
    TcpProxyConfig conf;
    conf.listenAddress = listenAddress;
    conf.listenPort = listenPort;
    conf.targetHost = server;
    conf.targetPort = serverPort;

    TcpProxy *proxy = new TcpProxy(conf, this);
    proxy->setJournal(m_journal);
    m_proxies.append(proxy);
    return proxy->listen();
}
