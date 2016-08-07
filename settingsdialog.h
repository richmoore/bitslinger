#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class SettingsPage;

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    void addPage(SettingsPage *page);

public slots:
    void pageChanged();

protected:
    void createPages();
private:
    Ui::SettingsDialog *ui;
    QList<SettingsPage *> pages;
};

#endif // SETTINGSDIALOG_H
