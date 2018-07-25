#include "bitslinger.h"

#include "proxytoolview.h"
#include "ui_proxytoolview.h"

ProxyToolView::ProxyToolView(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ProxyToolView)
{
    m_ui->setupUi(this);
dumpObjectTree();
    m_ui->hexView->setReadOnly(true);
    m_ui->textView->setReadOnly(true);

    connect(m_ui->journalView, SIGNAL(entryActivated(JournalEvent *)),
            this, SLOT(showEntry(JournalEvent*)));
}

ProxyToolView::~ProxyToolView()
{
    delete m_ui;
}

bool ProxyToolView::restoreState(const QByteArray &state)
{
    return m_ui->journalView->header()->restoreState(state);
}

QByteArray ProxyToolView::saveState()
{
    return m_ui->journalView->header()->saveState();
}

void ProxyToolView::setBitSlinger(BitSlinger *slinger)
{
    m_ui->journalView->setJournal(slinger->journal());
}

void ProxyToolView::showEntry(JournalEvent *entry)
{
    m_ui->hexView->setData(entry->content);
    m_ui->textView->setPlainText(QString(entry->content));
}
