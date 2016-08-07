#ifndef SSLSETTINGSPAGE_H
#define SSLSETTINGSPAGE_H

#include "settingspage.h"

namespace Ui {
class SslSettingsPage;
}

class SslSettingsPage : public SettingsPage
{
    Q_OBJECT

public:
    explicit SslSettingsPage(QWidget *parent = 0);
    ~SslSettingsPage();

private:
    Ui::SslSettingsPage *ui;
};

#endif // SSLSETTINGSPAGE_H
