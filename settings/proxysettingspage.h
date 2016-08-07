#ifndef PROXYSETTINGSPAGE_H
#define PROXYSETTINGSPAGE_H

#include "settingspage.h"

namespace Ui {
class ProxySettingsPage;
}

class ProxySettingsPage : public SettingsPage
{
    Q_OBJECT

public:
    explicit ProxySettingsPage(QWidget *parent = 0);
    ~ProxySettingsPage();

private:
    Ui::ProxySettingsPage *ui;
};

#endif // PROXYSETTINGSPAGE_H
