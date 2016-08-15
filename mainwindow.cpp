#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QSettings>

#include "bitslinger.h"
#include "listenerdialog.h"
#include "settingsdialog.h"
#include "utils/recentfilesmenu.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#define QLL QLatin1Literal

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_slinger(0)
{
    m_ui->setupUi(this);
    m_ui->hexView->setReadOnly(true);
    m_ui->textView->setReadOnly(true);

    setWindowIcon(QIcon(":icons/soundwave.svg"));

    m_recent = new RecentFilesMenu(this);
    connect(this, SIGNAL(fileOpened(QString)), m_recent, SLOT(addRecentFile(QString)));
    connect(this, SIGNAL(fileSaved(QString)), m_recent, SLOT(addRecentFile(QString)));
    connect(m_recent, SIGNAL(recentFileTriggered(QString)), this, SLOT(openState(QString)));

    m_recent->setTitle(tr("Open Recent"));
    m_ui->menuBitslinger->insertMenu(m_ui->action_Save_State, m_recent);

    connect(m_ui->action_Listeners, SIGNAL(triggered()), this, SLOT(showListenerDialog()));
    connect(m_ui->action_Options, SIGNAL(triggered()), this, SLOT(showSettings()));
    connect(m_ui->action_Load_State, SIGNAL(triggered()), this, SLOT(openState()));
    connect(m_ui->action_Save_State, SIGNAL(triggered()), this, SLOT(saveState()));

    loadGuiSettings();
}

MainWindow::~MainWindow()
{
    saveGuiSettings();
    delete m_ui;
}

void MainWindow::setBitSlinger(BitSlinger *slinger)
{
    m_slinger = slinger;
    m_ui->journalView->setJournal(slinger->journal());
    connect(m_ui->journalView, SIGNAL(entryActivated(JournalEvent *)),
            this, SLOT(showEntry(JournalEvent*)));
}

void MainWindow::loadGuiSettings()
{
    QSettings settings;
    settings.beginGroup(QLL("GUI"));

    m_recent->restoreState(settings.value(QLL("RecentFiles")).toByteArray());
    m_ui->journalView->header()->restoreState(settings.value(QLL("JournalView")).toByteArray());
}

void MainWindow::saveGuiSettings()
{
    QSettings settings;
    settings.beginGroup(QLL("GUI"));

    settings.setValue(QLL("RecentFiles"), m_recent->saveState());
    settings.setValue(QLL("JournalView"), m_ui->journalView->header()->saveState());
}

void MainWindow::showEntry(JournalEvent *entry)
{
    m_ui->hexView->setData(entry->content);
    m_ui->textView->setPlainText(QString(entry->content));
}

void MainWindow::showListenerDialog()
{
    qDebug() << "showListenerDialog" << m_slinger;
    ListenerDialog dlg(this);
    dlg.setWindowIcon(QIcon(":icons/stethoscope.svg"));
    dlg.setBitSlinger(m_slinger);
    dlg.exec();
}

void MainWindow::showSettings()
{
    SettingsDialog dlg(m_slinger ,this);
    dlg.exec();
}

void MainWindow::openState()
{
    qDebug() << "openState" << m_slinger;
    QString filename = QFileDialog::getOpenFileName(this);
    if (filename.isEmpty())
        return;;

    openState(filename);
}

bool MainWindow::openState(const QString &filename)
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open file" << filename;
        return false;
    }

    emit fileOpened(filename);
    return m_slinger->readState(&f);
}

void MainWindow::saveState()
{
    qDebug() << "saveState" << m_slinger;
    QString filename = QFileDialog::getSaveFileName(this);
    if (filename.isEmpty())
        return;;

    saveState(filename);
}

bool MainWindow::saveState(const QString &filename)
{
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open file" << filename;
        return false;
    }

    emit fileSaved(filename);
    return m_slinger->writeState(&f);
}
