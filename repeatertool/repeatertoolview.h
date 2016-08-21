#ifndef REPEATERTOOLVIEW_H
#define REPEATERTOOLVIEW_H

#include <QWidget>

namespace Ui {
class RepeaterToolView;
}

class Journal;

class RepeaterToolView : public QWidget
{
    Q_OBJECT

public:
    explicit RepeaterToolView(QWidget *parent = 0);
    ~RepeaterToolView();

    QByteArray saveState();
    bool restoreState(const QByteArray &state);

public slots:
    void setJournal(Journal *journal);

private:
    Ui::RepeaterToolView *ui;
};

#endif // REPEATERTOOLVIEW_H
