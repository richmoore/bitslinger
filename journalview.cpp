#include <QMenu>
#include <QHeaderView>


#include "journal.h"
#include "journalview.h"

JournalView::JournalView(QWidget *parent)
    : QTreeView(parent)
{
    connect(this, SIGNAL(activated(QModelIndex)), this, SLOT(rowActivated(QModelIndex)));
    connect(header(), SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showHeaderContextMenu(QPoint)));
    header()->setContextMenuPolicy(Qt::CustomContextMenu);
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

void JournalView::showHeaderContextMenu(const QPoint &pos)
{
    QMenu menu(this);

    for (int i; i < m_journal->columnCount(QModelIndex()); i++) {
        QAction *action = menu.addAction(m_journal->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString());
        action->setData(i);
        action->setCheckable(true);
        action->setChecked(!isColumnHidden(i));
    }

    QAction *action = menu.exec(header()->mapToGlobal(pos));
    if (action) {
        int idx = action->data().toInt();
        setColumnHidden(idx, !isColumnHidden(idx));
    }
}

