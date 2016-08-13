#include <QTreeWidgetItem>
#include <QDebug>
#include <QPushButton>

#include "bitslinger.h"

#include "settings/proxysettingspage.h"
#include "settings/sslcasettingspage.h"

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(BitSlinger *slinger, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    m_slinger(slinger)
{
    ui->setupUi(this);
    createPages();

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 6);

    ui->pageTitle->setText(pages[0]->windowTitle());
    ui->pageStack->setCurrentIndex(0);

    pages[0]->aboutToShow();
    ui->pageStack->setCurrentIndex(0);

    connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
            this, SLOT(pageChanged()));

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
            this, SLOT(apply()));
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()),
            this, SLOT(apply()));
    connect(ui->buttonBox->button(QDialogButtonBox::Reset), SIGNAL(clicked()),
            this, SLOT(reset()));
    connect(ui->buttonBox->button(QDialogButtonBox::RestoreDefaults), SIGNAL(clicked()),
            this, SLOT(defaults()));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::createPages()
{
    ProxySettingsPage *proxySettings = new ProxySettingsPage();
    connect(proxySettings, SIGNAL(saved()), m_slinger, SLOT(loadProxyConfig()));
    addPage(proxySettings);

    SslCaSettingsPage *sslCaSettings = new SslCaSettingsPage();
    connect(sslCaSettings, SIGNAL(saved()), m_slinger, SLOT(loadCaConfig()));
    addPage(sslCaSettings);
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
    pages[index]->aboutToShow();
    ui->pageTitle->setText(pages[index]->windowTitle());
    ui->pageStack->setCurrentIndex(index);
}

void SettingsDialog::apply()
{
    static_cast<SettingsPage *>(ui->pageStack->currentWidget())->save();
}

void SettingsDialog::defaults()
{
    static_cast<SettingsPage *>(ui->pageStack->currentWidget())->defaults();
}

void SettingsDialog::reset()
{
    static_cast<SettingsPage *>(ui->pageStack->currentWidget())->reset();
}
