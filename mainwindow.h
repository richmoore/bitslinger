#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QModelIndex;

class BitSlinger;
class RecentFilesMenu;
class AbstractTool;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addTool(AbstractTool *tool);

    void setBitSlinger(BitSlinger *slinger);

    void loadGuiSettings();
    void saveGuiSettings();

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

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *m_ui;
    BitSlinger *m_slinger;
    RecentFilesMenu *m_recent;
};

#endif // MAINWINDOW_H
