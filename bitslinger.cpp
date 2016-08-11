#include <QHostAddress>
#include <QDataStream>

#include "journal.h"
#include "listener.h"

#include "bitslinger.h"

const quint32 STATE_FILE_MAGIC = 0xB175BABE;
const quint16 STATE_FILE_VERSION = 0x0001;

BitSlinger::BitSlinger(QObject *parent) : QObject(parent)
{
    m_journal = new Journal(this);
    m_certGenerator.load();
}

void BitSlinger::setUpstreamProxy(const QNetworkProxy &upstream)
{
    m_upstream = upstream;
}

void BitSlinger::addListener(const ListenerConfig &config)
{
    Listener *proxy = new Listener(config, this);
    proxy->setBitSlinger(this);
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

bool BitSlinger::writeState(QIODevice *output)
{
    QDataStream stream(output);
    stream << STATE_FILE_MAGIC;
    stream << STATE_FILE_VERSION;

    StateFileFlags flags = StateFileHasJournal;
    stream << quint32(flags);

    if (flags & StateFileHasJournal)
        stream << *m_journal;

    return stream.status() == QDataStream::Ok;
}

bool BitSlinger::readState(QIODevice *input)
{
    QDataStream stream(input);
    quint32 magic;
    stream >> magic;
    if (magic != STATE_FILE_MAGIC) {
        qDebug() << "Cannot load state, Bad magic" << magic;
        return false;
    }

    quint16 version;
    stream >> version;
    if (version != STATE_FILE_VERSION) {
        qDebug() << "Cannot, load state, unknown version" << version;
        return false;
    }

    quint32 flags;
    stream >> flags;
    if (flags != (StateFileHasJournal)) {
        qDebug() << "Unsupported flags";
        return false;
    }

    stream >> *m_journal;

    return stream.status() != QDataStream::Ok;
}








