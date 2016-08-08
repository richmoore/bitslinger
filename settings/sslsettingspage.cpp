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
}

SslSettingsPage::~SslSettingsPage()
{
    delete ui;
}
