#ifndef SSLSETTINGSPAGE_H
#define SSLSETTINGSPAGE_H

#include "settingspage.h"

namespace Ui {
class SslCaSettingsPage;
}

class SslCaSettingsPage : public SettingsPage
{
    Q_OBJECT

public:
    explicit SslCaSettingsPage(QWidget *parent = 0);
    ~SslCaSettingsPage();

public slots:
    void load();
    void save();
    void defaults();

    void regenerateCa();

private:
    Ui::SslCaSettingsPage *ui;
};

#endif // SSLSETTINGSPAGE_H
