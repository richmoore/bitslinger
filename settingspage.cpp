#include <QMessageBox>

#include "settingspage.h"

SettingsPage::SettingsPage(QWidget *parent)
    : QWidget(parent),
      m_modified(false)
{

}

void SettingsPage::aboutToShow()
{
    load();
}

bool SettingsPage::aboutToClose()
{
    if (m_modified) {
        QMessageBox::StandardButton result = \
                QMessageBox::warning(this,
                                     tr("Unsaved Changes"),
                                     tr("You have unsaved configuration changes that will be lost, do you want to discard them?"),
                                     QMessageBox::Cancel|QMessageBox::Discard|QMessageBox::Save, QMessageBox::Cancel);

        switch(result) {
        case QMessageBox::Cancel:
            return false;
        case QMessageBox::Discard:
            return true;
        case QMessageBox::Save:
            save();
            return true;
        default:
            return false;
        }
    }

    return true;
}

void SettingsPage::setModified()
{
    if (!m_modified)
        emit modifiedChanged(true);
    m_modified = true;
}

void SettingsPage::clearModified()
{
    if (m_modified)
        emit modifiedChanged(false);
    m_modified = false;
}

void SettingsPage::load()
{

}

void SettingsPage::save()
{

}

void SettingsPage::reset()
{
    load();
}

void SettingsPage::defaults()
{

}
