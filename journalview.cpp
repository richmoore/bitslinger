#include "journalview.h"

JournalView::JournalView(QWidget *parent)
    : QTreeView(parent)
{
    connect(this, SIGNAL(activated(QModelIndex)), this, SLOT(rowActivated(QModelIndex)));
}

void JournalView::setJournal(Journal *journal)
{
    m_journal = journal;
    setModel(journal);
}

void JournalView::rowActivated(const QModelIndex &index)
{
    emit entryActivated(m_journal->entry(index));
}
