#include <QDebug>
#include <QSslConfiguration>
#include <QSslSocket>
#include <QSslCipher>

#include "sslconfigurationwidget.h"
#include "ui_sslconfigurationwidget.h"

SslConfigurationWidget::SslConfigurationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SslConfigurationWidget)
{
    ui->setupUi(this);
}

SslConfigurationWidget::~SslConfigurationWidget()
{
    delete ui;
}

void SslConfigurationWidget::setConfiguration(const QSslConfiguration &config)
{
    switch(config.protocol()) {
    case QSsl::SecureProtocols:
        ui->protocolCombo->setCurrentIndex(0);
        break;
    case QSsl::TlsV1_0OrLater:
        ui->protocolCombo->setCurrentIndex(1);
        break;
    case QSsl::TlsV1_1OrLater:
        ui->protocolCombo->setCurrentIndex(2);
        break;
    case QSsl::TlsV1_2OrLater:
        ui->protocolCombo->setCurrentIndex(3);
        break;
    case QSsl::TlsV1_2:
        ui->protocolCombo->setCurrentIndex(4);
        break;
    case QSsl::TlsV1_1:
        ui->protocolCombo->setCurrentIndex(5);
        break;
    case QSsl::TlsV1_0:
        ui->protocolCombo->setCurrentIndex(6);
        break;
    case QSsl::SslV3:
        ui->protocolCombo->setCurrentIndex(7);
        break;
    case QSsl::UnknownProtocol:
    case QSsl::SslV2:
    case QSsl::TlsV1SslV3:
    case QSsl::AnyProtocol:
        qWarning() << "Bad protocol" << config.protocol();
        break;
    }

    ui->cipherTree->setDragEnabled(true);
    ui->cipherTree->setDragDropMode(QAbstractItemView::InternalMove);
    ui->cipherTree->viewport()->setAcceptDrops(true);
    ui->cipherTree->setDropIndicatorShown(true);

    QList<QSslCipher> enabledCiphers = config.ciphers();
    foreach(const QSslCipher &cipher, enabledCiphers) {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->cipherTree);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
        item->setCheckState(0, Qt::Checked);
        item->setText(0, cipher.name());
        item->setText(1, QString::number(cipher.usedBits()));
        item->setText(2, cipher.encryptionMethod());
        item->setText(3, cipher.keyExchangeMethod());
        item->setText(4, cipher.authenticationMethod());
    }

    QList<QSslCipher> supportedCiphers = QSslSocket::supportedCiphers();
    foreach(const QSslCipher &cipher, supportedCiphers) {
        if (enabledCiphers.contains(cipher))
            continue;

        QTreeWidgetItem *item = new QTreeWidgetItem(ui->cipherTree);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
        item->setCheckState(0, Qt::Unchecked);
        item->setText(0, cipher.name());
        item->setText(1, QString::number(cipher.usedBits()));
        item->setText(2, cipher.encryptionMethod());
        item->setText(3, cipher.keyExchangeMethod());
        item->setText(4, cipher.authenticationMethod());
    }
}
