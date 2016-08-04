#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QModelIndex;

class BitSlinger;
class JournalEntry;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setBitSlinger(BitSlinger *slinger);

public slots:
    void showEntry(JournalEntry *entry);

private:
    Ui::MainWindow *m_ui;
};

#endif // MAINWINDOW_H
