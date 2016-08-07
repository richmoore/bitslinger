#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>

class SettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsPage(QWidget *parent = 0);

signals:

public slots:
    void load();
    void save();
    void reset();
    void defaults();

};

#endif // SETTINGSPAGE_H
