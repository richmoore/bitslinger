#include <QNetworkInterface>

#include "listenereditdialog.h"
#include "ui_listenereditdialog.h"

ListenerEditDialog::ListenerEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListenerEditDialog)
{
    ui->setupUi(this);

    ui->listenAddressCombo->addItem(tr("Local Host (IPv4)"));
    ui->listenAddressCombo->addItem(tr("Local Host (IPv6)"));
    ui->listenAddressCombo->addItem(tr("Any (IPv4)"));
    ui->listenAddressCombo->addItem(tr("Any (IPv6)"));
    ui->listenAddressCombo->addItem(tr("Any (IPv4 and IPv6)"));

    addresses << QHostAddress::LocalHost
              << QHostAddress::LocalHostIPv6
              << QHostAddress::AnyIPv4
              << QHostAddress::AnyIPv6
              << QHostAddress::Any;

    foreach(const QHostAddress &addr, QNetworkInterface::allAddresses()) {
        if (addr.isLoopback())
            continue;

        ui->listenAddressCombo->addItem(addr.toString());
        addresses << addr;
    }
}

ListenerEditDialog::~ListenerEditDialog()
{
    delete ui;
}

void ListenerEditDialog::setProxyType(Listener::ProxyType type)
{
    ui->proxyTypeCombo->setCurrentIndex(int(type));
}

Listener::ProxyType ListenerEditDialog::proxyType() const
{
    int index = ui->proxyTypeCombo->currentIndex();
    return Listener::ProxyType(index);
}

void ListenerEditDialog::setSslMode(Listener::SslMode mode)
{
    ui->sslModeCombo->setCurrentIndex(int(mode));
}

Listener::SslMode ListenerEditDialog::sslMode() const
{
    int index = ui->sslModeCombo->currentIndex();
    return Listener::SslMode(index);
}

QHostAddress ListenerEditDialog::listenAddress() const
{
    return addresses[ui->listenAddressCombo->currentIndex()];
}

void ListenerEditDialog::setListenAddress(const QHostAddress &address)
{
    int index = addresses.indexOf(address);
    ui->listenAddressCombo->setCurrentIndex(index);
}

int ListenerEditDialog::listenPort() const
{
    return ui->listenPortSpin->value();
}

void ListenerEditDialog::setListenPort(int port)
{
    ui->listenPortSpin->setValue(port);
}

QString ListenerEditDialog::targetHost() const
{
    return ui->targetHostEdit->text();
}

void ListenerEditDialog::setTargetHost(const QString &host)
{
    ui->targetHostEdit->setText(host);
}

int ListenerEditDialog::targetPort() const
{
    return ui->targetPortSpin->value();
}

void ListenerEditDialog::setTargetPort(int port)
{
    ui->targetPortSpin->setValue(port);
}
