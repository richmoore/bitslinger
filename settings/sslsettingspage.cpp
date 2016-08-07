#include "sslsettingspage.h"
#include "ui_sslsettingspage.h"

SslSettingsPage::SslSettingsPage(QWidget *parent) :
    SettingsPage(parent),
    ui(new Ui::SslSettingsPage)
{
    ui->setupUi(this);
}

SslSettingsPage::~SslSettingsPage()
{
    delete ui;
}
