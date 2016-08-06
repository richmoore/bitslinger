#include <QHostAddress>

#include "journal.h"
#include "listener.h"

#include "bitslinger.h"

BitSlinger::BitSlinger(QObject *parent) : QObject(parent)
{
    m_journal = new Journal(this);
}

void BitSlinger::setUpstreamProxy(const QNetworkProxy &upstream)
{
    m_upstream = upstream;
}

void BitSlinger::addListener(const ListenerConfig &config)
{
    Listener *proxy = new Listener(config, this);
    proxy->setUpstreamProxy(m_upstream);
    proxy->setJournal(m_journal);
    m_listeners.append(proxy);

    proxy->listen();
}

void BitSlinger::editListener(int index, const ListenerConfig &config)
{
    m_listeners[index]->setConfig(config);
}

void BitSlinger::removeListener(int index)
{
    m_listeners.removeAt(index);
}
