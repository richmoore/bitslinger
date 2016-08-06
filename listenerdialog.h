#ifndef LISTENERDIALOG_H
#define LISTENERDIALOG_H

#include <QDialog>

namespace Ui {
class ListenerDialog;
}

class BitSlinger;

class ListenerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ListenerDialog(QWidget *parent = 0);
    ~ListenerDialog();

    void setBitSlinger(BitSlinger *slinger);

private:
    Ui::ListenerDialog *ui;
    BitSlinger *m_slinger;
};

#endif // LISTENERDIALOG_H
