#include <QTreeWidgetItem>
#include <QDebug>

#include "settings/proxysettingspage.h"
#include "settings/sslsettingspage.h"

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    createPages();

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 6);

    ui->pageTitle->setText(pages[0]->windowTitle());
    ui->pageStack->setCurrentIndex(0);

    pages[0]->show();

    connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
            this, SLOT(pageChanged()));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::createPages()
{
    ProxySettingsPage *proxySettings = new ProxySettingsPage();
    addPage(proxySettings);

    SslSettingsPage *sslSettings = new SslSettingsPage();
    addPage(sslSettings);
}

void SettingsDialog::addPage(SettingsPage *page)
{
    qDebug() << "Adding page" << page->windowTitle();
    pages.append(page);

    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
    item->setText(0, page->windowTitle());

    ui->pageStack->addWidget(page);
}

void SettingsDialog::pageChanged()
{
    int index = ui->treeWidget->currentIndex().row();
    if (index < 0)
        return;

    int prev = ui->pageStack->currentIndex();
    if (!pages[prev]->aboutToClose()) {
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(prev);
        item->setSelected(true);
        qDebug() << "attempting to select" << item->text(0);
        return;
    }

    qDebug() << "Changing to page" << index;
    ui->pageTitle->setText(pages[index]->windowTitle());
    ui->pageStack->setCurrentIndex(index);
}
