#include "journal.h"

#include "repeatertoolview.h"
#include "repeatertool.h"

RepeaterTool::RepeaterTool(QObject *parent)
    : AbstractTool(parent)
{
    setName(tr("Repeater"));
    setIcon(QIcon(":icons/repost.svg"));

    m_view = new RepeaterToolView;
    setWidget(m_view);

    m_journal = new Journal(this);
    m_view->setJournal(m_journal);
}

QByteArray RepeaterTool::saveState()
{
    return m_view->saveState();
}

bool RepeaterTool::restoreState(const QByteArray &state)
{
    return m_view->restoreState(state);
}
