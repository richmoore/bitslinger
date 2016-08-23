#include <QHostAddress>

#include "bitslinger.h"
#include "listener.h"

#include "listenereditdialog.h"
#include "ui_listenereditdialog.h"

#include "listenerdialog.h"
#include "ui_listenerdialog.h"

ListenerDialog::ListenerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListenerDialog)
{
    ui->setupUi(this);

    connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(addListener()));
    connect(ui->editButton, SIGNAL(clicked(bool)), this, SLOT(editListener()));
    connect(ui->removeButton, SIGNAL(clicked(bool)), this, SLOT(removeListener()));
}

ListenerDialog::~ListenerDialog()
{
    delete ui;
}

void ListenerDialog::setBitSlinger(BitSlinger *slinger)
{
    m_slinger = slinger;
    refreshList();
}

void ListenerDialog::refreshList()
{
    ui->listenerTree->clear();

    foreach(Listener *listener, m_slinger->listeners()) {
        Listener::Config config = listener->config();

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, config.listenAddress.toString());
        item->setText(1, QString::number(config.listenPort));
        item->setText(2, config.targetHost);
        item->setText(3, QString::number(config.targetPort));

        if (config.type == Listener::TcpProxy)
            item->setText(4, tr("TCP Proxy"));
        else if (config.type == Listener::HttpProxy)
            item->setText(4, tr("HTTP Proxy"));

        ui->listenerTree->addTopLevelItem(item);
    }
}

void ListenerDialog::addListener()
{
    ListenerEditDialog dlg(this);

    int result = dlg.exec();
    if (result == QDialog::Rejected)
        return;

    Listener::Config config;
    config.listenAddress = dlg.listenAddress();
    config.listenPort = dlg.listenPort();
    config.targetHost = dlg.targetHost();
    config.targetPort = dlg.targetPort();
    config.sslMode = dlg.sslMode();
    config.type = dlg.proxyType();

    m_slinger->addListener(config);
    m_slinger->saveListenerConfig();
    refreshList();
}

void ListenerDialog::editListener()
{
    ListenerEditDialog dlg(this);

    int index = ui->listenerTree->currentIndex().row();
    Listener::Config config = m_slinger->listenerAt(index)->config();
    dlg.setListenAddress(config.listenAddress);
    dlg.setListenPort(config.listenPort);
    dlg.setTargetHost(config.targetHost);
    dlg.setTargetPort(config.targetPort);
    dlg.setSslMode(config.sslMode);
    dlg.setProxyType(config.type);

    int result = dlg.exec();
    if (result == QDialog::Rejected)
        return;

    config.listenAddress = dlg.listenAddress();
    config.listenPort = dlg.listenPort();
    config.targetHost = dlg.targetHost();
    config.targetPort = dlg.targetPort();
    config.sslMode = dlg.sslMode();
    config.type = dlg.proxyType();

    m_slinger->editListener(index, config);
    m_slinger->saveListenerConfig();
    refreshList();
}

void ListenerDialog::removeListener()
{
    int index = ui->listenerTree->currentIndex().row();
    m_slinger->removeListener(index);
    m_slinger->saveListenerConfig();
    refreshList();
}
