#ifndef PROXYTOOL_H
#define PROXYTOOL_H

#include "abstracttool.h"

class ProxyToolView;

class ProxyTool : public AbstractTool
{
public:
    ProxyTool(QObject *parent);

    QByteArray saveState();
    bool restoreState(const QByteArray &state);

private:
    ProxyToolView *m_view;
};

#endif // PROXYTOOL_H
