#ifndef REPEATERGUTTERWIDGET_H
#define REPEATERGUTTERWIDGET_H

#include <QWidget>

class JournalView;

class RepeaterGutterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RepeaterGutterWidget(QWidget *parent = 0);

    void setTreeView(JournalView *view);
signals:

public slots:

private:
    JournalView *m_view;
};

#endif // REPEATERGUTTERWIDGET_H
