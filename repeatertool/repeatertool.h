#ifndef REPEATERTOOL_H
#define REPEATERTOOL_H

#include "abstracttool.h"

class RepeaterToolView;

class RepeaterTool : public AbstractTool
{
public:
    RepeaterTool(QObject *parent);

    QByteArray saveState();
    bool restoreState(const QByteArray &state);

private:
    RepeaterToolView *m_view;
};

#endif // REPEATERTOOL_H
