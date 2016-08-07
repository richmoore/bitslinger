#include <QTreeWidgetItem>
#include <QDebug>

#include "settings/proxysettingspage.h"

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    createPages();

    ui->pageTitle->setText(pages[0]->windowTitle());
    ui->pageStack->setCurrentIndex(0);

    updateGeometry();

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
}

void SettingsDialog::addPage(SettingsPage *page)
{
    qDebug() << "Adding page" << page->windowTitle();
    pages.append(page);

    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
    item->setText(0, page->windowTitle());


    ui->pageStack->addWidget(page);
    page->show();
}

void SettingsDialog::pageChanged()
{
    int index = ui->treeWidget->currentIndex().row();
    if (index < 0)
        return;
    qDebug() << "Changing to page" << index;
    ui->pageTitle->setText(pages[index]->windowTitle());
    ui->pageStack->setCurrentIndex(index);
}
