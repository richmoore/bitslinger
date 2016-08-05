#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QModelIndex;

class BitSlinger;
class JournalEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setBitSlinger(BitSlinger *slinger);

public slots:
    void showEntry(JournalEvent *entry);

private:
    Ui::MainWindow *m_ui;
};

#endif // MAINWINDOW_H
