#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>

class SettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsPage(QWidget *parent = 0);

    // Returns true if it is okay to close
    bool aboutToClose();

signals:
    void modifiedChanged(bool);

    void saved();

public slots:
    void aboutToShow();

    void setModified();
    void clearModified();

    virtual void load();
    virtual void save();
    virtual void reset();
    virtual void defaults();

private:
    bool m_modified;
};

#endif // SETTINGSPAGE_H
