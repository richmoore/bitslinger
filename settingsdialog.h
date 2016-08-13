#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class SettingsPage;
class BitSlinger;

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(BitSlinger *slinger, QWidget *parent = 0);
    ~SettingsDialog();

    void setBitSlinger(BitSlinger *slinger) { m_slinger = slinger; }
    BitSlinger *bitSlinger() const { return m_slinger; }

    void addPage(SettingsPage *page);

public slots:
    void pageChanged();

    void apply();
    void defaults();
    void reset();

protected:
    void createPages();
private:
    Ui::SettingsDialog *ui;
    QList<SettingsPage *> pages;
    BitSlinger *m_slinger;
};

#endif // SETTINGSDIALOG_H
