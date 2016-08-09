#include "sslsettingspage.h"
#include "ui_sslsettingspage.h"

#include "utils/certificategenerator.h"

SslSettingsPage::SslSettingsPage(QWidget *parent) :
    SettingsPage(parent),
    ui(new Ui::SslSettingsPage)
{
    ui->setupUi(this);

    CertificateGenerator gen;
    QSslKey key = gen.createKey();
    ui->cakeyEdit->setText(key.toPem());
    QSslCertificate ca = gen.createCaCertificate(key);
    ui->cacertEdit->setText(ca.toPem());
}

SslSettingsPage::~SslSettingsPage()
{
    delete ui;
}
