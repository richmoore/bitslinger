#include <QDebug>

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
