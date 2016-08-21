#include "proxytoolview.h"
#include "proxytool.h"

ProxyTool::ProxyTool(QObject *parent)
    : AbstractTool(parent)
{
    setName(tr("Proxy"));
    setIcon(QIcon(":icons/stethoscope.svg"));

    m_view = new ProxyToolView;
    setWidget(m_view);

    connect(this, SIGNAL(bitSlingerChanged(BitSlinger*)), m_view, SLOT(setBitSlinger(BitSlinger*)));
}

QByteArray ProxyTool::saveState()
{
    return m_view->saveState();
}

bool ProxyTool::restoreState(const QByteArray &state)
{
    return m_view->restoreState(state);
}
