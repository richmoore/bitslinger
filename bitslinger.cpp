#include <QHostAddress>
#include <QDataStream>
#include <QDebug>
#include <QSslCertificate>
#include <QSslKey>
#include <QSettings>

#include "journal.h"
#include "listener.h"

#include "bitslinger.h"

const quint32 STATE_FILE_MAGIC = 0xB175BABE;
const quint16 STATE_FILE_VERSION = 0x0001;

#define QLL QLatin1Literal

BitSlinger::BitSlinger(QObject *parent) : QObject(parent)
{
    m_journal = new Journal(this);

    loadCaConfig();
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
    m_listeners[index]->deleteLater();
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

void BitSlinger::loadCaConfig()
{
    QSettings settings;
    settings.beginGroup(QLL("SSL Certificate Authority"));

    QSslKey caKey;
    QSslCertificate caCert;

    if (settings.contains(QLL("CAKey")) && settings.contains(QLL("CACert"))) {
        qDebug() << "Using existing CA";

        caKey = QSslKey(settings.value("CAKey").toByteArray(), QSsl::Rsa, QSsl::Pem);
        caCert = QSslCertificate(settings.value("CACert").toByteArray(), QSsl::Pem);
    }
    else {
        qDebug() << "Making new CA";

        caKey = m_certGenerator.createKey();
        caCert = m_certGenerator.createCaCertificate(caKey);
    }

    m_certGenerator.setCaKey(caKey);
    m_certGenerator.setCaCertificate(caCert);

    // Save the generated CA
    settings.setValue(QLL("CAKey"), caKey.toPem());
    settings.setValue(QLL("CACert"), caCert.toPem());

    QSslKey leafKey = m_certGenerator.createKey();
    m_certGenerator.setLeafKey(leafKey);
}

void BitSlinger::saveListenerConfig()
{
    QSettings settings;
    settings.beginGroup(QLL("Listener Configuration"));

    settings.beginWriteArray(QLL("Listeners"));
    for(int i=0; i < m_listeners.size(); i++) {
        settings.setArrayIndex(i);

        ListenerConfig config = m_listeners[i]->config();
        settings.setValue(QLL("ListenAddress"), config.listenAddress.toString());
        settings.setValue(QLL("ListenPort"), config.listenPort);
        settings.setValue(QLL("TargetHost"), config.targetHost);
        settings.setValue(QLL("TargetPort"), config.targetPort);
    }
    settings.endArray();
}

void BitSlinger::loadListenerConfig()
{
    // Remove any existing listeners
    while(m_listeners.size())
        removeListener(0);

    QSettings settings;
    settings.beginGroup(QLL("Listener Configuration"));

    int count = settings.beginReadArray(QLL("Listeners"));
    for(int i=0; i < count; i++) {
        settings.setArrayIndex(i);
        ListenerConfig config;
        config.listenAddress = QHostAddress(settings.value(QLL("ListenAddress")).toString());
        config.listenPort = settings.value(QLL("ListenPort")).toInt();
        config.targetHost = settings.value(QLL("TargetHost")).toString();
        config.targetPort = settings.value(QLL("TargetPort")).toInt();

        addListener(config);
    }
    settings.endArray();
}








