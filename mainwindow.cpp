#include "bitslinger.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    m_ui->hexView->setReadOnly(true);
    m_ui->textView->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::setBitSlinger(BitSlinger *slinger)
{
    m_ui->journalView->setJournal(slinger->journal());
    connect(m_ui->journalView, SIGNAL(entryActivated(JournalEntry *)),
            this, SLOT(showEntry(JournalEntry*)));
}

void MainWindow::showEntry(JournalEntry *entry)
{
    m_ui->hexView->setData(entry->content);
    m_ui->textView->setPlainText(QString(entry->content));
}
