#include <QDebug>
#include <QSettings>

#include "sslcasettingspage.h"
#include "ui_sslcasettingspage.h"

#include "utils/certificategenerator.h"

#define QLL QLatin1Literal

SslCaSettingsPage::SslCaSettingsPage(QWidget *parent) :
    SettingsPage(parent),
    ui(new Ui::SslCaSettingsPage)
{
    ui->setupUi(this);

}

SslCaSettingsPage::~SslCaSettingsPage()
{
    delete ui;
}

void SslCaSettingsPage::regenerateCa()
{
    CertificateGenerator gen;

    QSslKey key = gen.createKey();
    ui->cakeyEdit->setText(key.toPem());

    QSslCertificate ca = gen.createCaCertificate(key);
    ui->cacertEdit->setText(ca.toPem());

    setModified();
}

void SslCaSettingsPage::load()
{
    qDebug() << "Loading ssl";

    QSettings settings;
    settings.beginGroup(QLL("SSL Certificate Authority"));

    QSslKey key;
    QSslCertificate cert;

    if (settings.contains(QLL("CAKey")) && settings.contains(QLL("CACert"))) {
        qDebug() << "Using existing CA";

        key = QSslKey(settings.value("CAKey").toByteArray(), QSsl::Rsa, QSsl::Pem);
        cert = QSslCertificate(settings.value("CACert").toByteArray(), QSsl::Pem);

        clearModified();
    }
    else {
        qDebug() << "Making new CA";

        CertificateGenerator gen;
        key = gen.createKey();
        cert = gen.createCaCertificate(key);

        setModified();
    }

    ui->cakeyEdit->setText(key.toPem());
    ui->cacertEdit->setText(cert.toPem());
}

void SslCaSettingsPage::save()
{
    QSettings settings;
    settings.beginGroup(QLL("SSL Certificate Authority"));

    QByteArray keyPem = ui->cakeyEdit->toPlainText().toLatin1();
    settings.setValue(QLL("CAKey"), keyPem);

    QByteArray certPem = ui->cacertEdit->toPlainText().toLatin1();
    settings.setValue(QLL("CACert"), certPem);

    clearModified();
    emit saved();
}

void SslCaSettingsPage::defaults()
{
    QSettings settings;
    settings.beginGroup(QLL("SSL Certificate Authority"));
    settings.remove(QLL(""));

    load();
}
