#include <QDebug>

#include "proxysettingspage.h"
#include "ui_proxysettingspage.h"

ProxySettingsPage::ProxySettingsPage(QWidget *parent) :
    SettingsPage(parent),
    ui(new Ui::ProxySettingsPage)
{
    ui->setupUi(this);
    qDebug() << "ProxySettingsPage";
}

ProxySettingsPage::~ProxySettingsPage()
{
    delete ui;
}
