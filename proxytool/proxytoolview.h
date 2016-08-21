#ifndef PROXYTOOLVIEW_H
#define PROXYTOOLVIEW_H

#include <QWidget>

namespace Ui {
class ProxyToolView;
}

class BitSlinger;
class JournalEvent;

class ProxyToolView : public QWidget
{
    Q_OBJECT

public:
    explicit ProxyToolView(QWidget *parent = 0);
    ~ProxyToolView();

    QByteArray saveState();
    bool restoreState(const QByteArray &state);

public slots:
    void setBitSlinger(BitSlinger *slinger);
    void showEntry(JournalEvent *entry);

private:
    Ui::ProxyToolView *m_ui;
};

#endif // PROXYTOOLVIEW_H
