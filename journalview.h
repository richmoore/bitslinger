#ifndef JOURNALVIEW_H
#define JOURNALVIEW_H

#include <QTreeView>

#include "journal.h"

class JournalView : public QTreeView
{
    Q_OBJECT
public:
    JournalView(QWidget *parent=0);

    void setJournal(Journal *journal);

signals:
    void entryActivated(JournalEntry *entry);

protected slots:
    void rowActivated(const QModelIndex &index);

private:
    Journal *m_journal;
};

#endif // JOURNALVIEW_H
