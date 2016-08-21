#include "journalview.h"

#include "repeatergutterwidget.h"

RepeaterGutterWidget::RepeaterGutterWidget(QWidget *parent)
    : QWidget(parent),
      m_view(0)
{

}

void RepeaterGutterWidget::setTreeView(JournalView *view)
{
    m_view = view;
}
