#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QModelIndex;

class BitSlinger;
struct JournalEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setBitSlinger(BitSlinger *slinger);

signals:
    void fileOpened(const QString &filename);
    void fileSaved(const QString &filename);

public slots:
    void showListenerDialog();
    void showSettings();

    void openState();
    bool openState(const QString &filename);
    void saveState();
    bool saveState(const QString &filename);

protected slots:
    void showEntry(JournalEvent *entry);

private:
    Ui::MainWindow *m_ui;
    BitSlinger *m_slinger;
};

#endif // MAINWINDOW_H
