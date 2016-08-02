#include "journal.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setJournal(Journal *journal)
{
    this->journal = journal;
    ui->journalView->setModel(journal);
    connect(ui->journalView, SIGNAL(activated(QModelIndex)),
            this, SLOT(entryActivated(QModelIndex)));
}

void MainWindow::entryActivated(const QModelIndex &index)
{
    showEntry(journal->entry(index));
}

void MainWindow::showEntry(JournalEntry *entry)
{
    ui->hexEdit->setData(entry->content);
}
