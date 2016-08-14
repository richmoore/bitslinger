#ifndef HOSTNAMELINEEDIT_H
#define HOSTNAMELINEEDIT_H

#include <QLineEdit>

class QHostInfo;

class HostnameLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    HostnameLineEdit(QWidget *parent=nullptr);

public slots:
    void lookupHost();
    void setStatusGood(bool yes);

protected slots:
    void lookupComplete(const QHostInfo &info);

private:
    int m_lookupId;
};

#endif // HOSTNAMELINEEDIT_H
