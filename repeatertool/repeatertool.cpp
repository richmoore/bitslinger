#include "repeatertoolview.h"
#include "repeatertool.h"

RepeaterTool::RepeaterTool(QObject *parent)
    : AbstractTool(parent)
{
    setName(tr("Repeater"));
    setIcon(QIcon(":icons/repost.svg"));

    m_view = new RepeaterToolView;
    setWidget(m_view);

    connect(this, SIGNAL(bitSlingerChanged(BitSlinger*)), m_view, SLOT(setBitSlinger(BitSlinger*)));
}

QByteArray RepeaterTool::saveState()
{
    return m_view->saveState();
}

bool RepeaterTool::restoreState(const QByteArray &state)
{
    return m_view->restoreState(state);
}
