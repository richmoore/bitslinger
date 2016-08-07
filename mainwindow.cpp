#include <QDebug>
#include <QFileDialog>
#include <QFile>

#include "bitslinger.h"
#include "listenerdialog.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_slinger(0)
{
    m_ui->setupUi(this);
    m_ui->hexView->setReadOnly(true);
    m_ui->textView->setReadOnly(true);

    setWindowIcon(QIcon(":icons/soundwave.svg"));
    m_ui->action_Listeners->setIcon(QIcon(":icons/stethoscope.svg"));

    connect(m_ui->action_Listeners, SIGNAL(triggered()), this, SLOT(showListenerDialog()));
    connect(m_ui->action_Load_State, SIGNAL(triggered()), this, SLOT(openState()));
    connect(m_ui->action_Save_State, SIGNAL(triggered()), this, SLOT(saveState()));

}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::setBitSlinger(BitSlinger *slinger)
{
    m_slinger = slinger;
    m_ui->journalView->setJournal(slinger->journal());
    connect(m_ui->journalView, SIGNAL(entryActivated(JournalEvent *)),
            this, SLOT(showEntry(JournalEvent*)));
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

void MainWindow::openState()
{
    qDebug() << "openState" << m_slinger;
    QString filename = QFileDialog::getOpenFileName(this);
    if (filename.isEmpty())
        return;;

    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open file" << filename;
        return;
    }

    m_slinger->readState(&f);
}

void MainWindow::saveState()
{
    qDebug() << "saveState" << m_slinger;
    QString filename = QFileDialog::getSaveFileName(this);
    if (filename.isEmpty())
        return;;

    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open file" << filename;
        return;
    }

    m_slinger->writeState(&f);
}
