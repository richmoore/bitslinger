#include "abstracttool.h"

AbstractTool::AbstractTool(QObject *parent)
    : QObject(parent),
      m_slinger(0)
{

}

void AbstractTool::setBitSlinder(BitSlinger *slinger)
{
    if (m_slinger != slinger) {
         m_slinger = slinger;
         emit bitSlingerChanged(slinger);
    }
}

bool AbstractTool::restoreState(const QByteArray &state)
{
    Q_UNUSED(state);
    return true;
}

QByteArray AbstractTool::saveState()
{
    return QByteArray();
}
