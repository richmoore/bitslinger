#include "journal.h"

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

void MainWindow::setJournal(Journal *journal)
{
    m_journal = journal;
    m_ui->journalView->setModel(journal);
    connect(m_ui->journalView, SIGNAL(activated(QModelIndex)),
            this, SLOT(entryActivated(QModelIndex)));
}

void MainWindow::entryActivated(const QModelIndex &index)
{
    showEntry(m_journal->entry(index));
}

void MainWindow::showEntry(JournalEntry *entry)
{
    m_ui->hexView->setData(entry->content);
    m_ui->textView->setPlainText(QString(entry->content));
}
