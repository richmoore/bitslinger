#include <QDebug>
#include <QSettings>

#include "proxysettingspage.h"
#include "ui_proxysettingspage.h"

#define QLL QLatin1Literal

ProxySettingsPage::ProxySettingsPage(QWidget *parent) :
    SettingsPage(parent),
    ui(new Ui::ProxySettingsPage)
{
    ui->setupUi(this);
    qDebug() << "ProxySettingsPage";

    connect(ui->proxyCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(setModified()));
    connect(ui->portSpin, SIGNAL(valueChanged(int)), this, SLOT(setModified()));
    connect(ui->hostEdit, SIGNAL(textChanged(QString)), this, SLOT(setModified()));
    connect(ui->userEdit, SIGNAL(textChanged(QString)), this, SLOT(setModified()));
    connect(ui->passwordEdit, SIGNAL(textChanged(QString)), this, SLOT(setModified()));
}

ProxySettingsPage::~ProxySettingsPage()
{
    delete ui;
}

void ProxySettingsPage::load()
{
    QSettings settings;
    settings.beginGroup(QLL("Proxy"));

    ui->proxyCombo->setCurrentIndex(settings.value(QLL("ProxyType"), 0).toInt());
    ui->hostEdit->setText(settings.value(QLL("ProxyHost")).toString());
    ui->portSpin->setValue(settings.value(QLL("ProxyPort"), 8080).toInt());
    ui->userEdit->setText(settings.value(QLL("ProxyUser")).toString());
    ui->passwordEdit->setText(settings.value(QLL("ProxyPassword")).toString());

    clearModified();
}

void ProxySettingsPage::save()
{
    qDebug() << "Saving proxy settings";

    QSettings settings;
    settings.beginGroup(QLL("Proxy"));

    settings.setValue(QLL("ProxyType"), ui->proxyCombo->currentIndex());
    settings.setValue(QLL("ProxyHost"), ui->hostEdit->text());
    settings.setValue(QLL("ProxyPort"), ui->portSpin->value());
    settings.setValue(QLL("ProxyUser"), ui->userEdit->text());
    settings.setValue(QLL("ProxyPassword"), ui->passwordEdit->text());

    clearModified();
}

void ProxySettingsPage::defaults()
{
    QSettings settings;
    settings.beginGroup(QLL("Proxy"));
    settings.remove(QLL(""));

    load();
}
