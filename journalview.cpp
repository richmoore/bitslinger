#include <QMenu>
#include <QHeaderView>


#include "journal.h"
#include "journalview.h"

JournalView::JournalView(QWidget *parent)
    : QTreeView(parent)
{
    connect(this, SIGNAL(activated(QModelIndex)), this, SLOT(rowActivated(QModelIndex)));

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showItemContextMenu(QPoint)));
    setContextMenuPolicy(Qt::CustomContextMenu);

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

void JournalView::showItemContextMenu(const QPoint &pos)
{
    QModelIndex index = indexAt(pos);

    QMenu menu(this);
    menu.addAction("Test");

    menu.exec(viewport()->mapToGlobal(pos));

    // Leave this for now - will need a QWidgetAction
//    QMenu *highlightMenu = menu.addMenu(tr("Highlight"));
//    highlightMenu->addText();
}

void JournalView::showHeaderContextMenu(const QPoint &pos)
{
    QMenu menu(this);

    for (int i=0; i < m_journal->columnCount(QModelIndex()); i++) {
        QAction *action = menu.addAction(m_journal->headerData(i, Qt::Horizontal, Journal::ContextMenuRole).toString());
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

