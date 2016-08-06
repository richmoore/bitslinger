#include "bitslinger.h"
#include "listener.h"

#include "listenerdialog.h"
#include "ui_listenerdialog.h"

ListenerDialog::ListenerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListenerDialog)
{
    ui->setupUi(this);
}

ListenerDialog::~ListenerDialog()
{
    delete ui;
}

void ListenerDialog::setBitSlinger(BitSlinger *slinger)
{
    m_slinger = slinger;
    foreach(Listener *listener, slinger->proxies()) {
        ListenerConfig config = listener->config();

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, config.listenAddress.toString());
        item->setText(1, QString::number(config.listenPort));
        item->setText(2, config.targetHost);
        item->setText(3, QString::number(config.targetPort));
        ui->listenerTree->addTopLevelItem(item);
    }

}
