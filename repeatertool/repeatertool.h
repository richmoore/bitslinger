#ifndef REPEATERTOOL_H
#define REPEATERTOOL_H

#include "abstracttool.h"

class RepeaterToolView;
class Journal;

class RepeaterTool : public AbstractTool
{
public:
    RepeaterTool(QObject *parent);

    Journal *journal() const { return m_journal; }

    QByteArray saveState();
    bool restoreState(const QByteArray &state);

private:
    RepeaterToolView *m_view;
    Journal *m_journal;
};

#endif // REPEATERTOOL_H
