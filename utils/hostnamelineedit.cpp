#include <QHostInfo>
#include <QDebug>

#include "hostnamelineedit.h"

HostnameLineEdit::HostnameLineEdit(QWidget *parent)
    : QLineEdit(parent),
      m_lookupId(-1)
{
    connect(this, SIGNAL(textChanged(QString)), SLOT(lookupHost()));
}

void HostnameLineEdit::lookupHost()
{
    if (m_lookupId >= 0)
        QHostInfo::abortHostLookup(m_lookupId);

    QHostInfo::lookupHost(text(), this, SLOT(lookupComplete(QHostInfo)));
}

void HostnameLineEdit::setStatusGood(bool yes)
{
    QColor c;
    if (yes)
        c = QColor(0xcc, 0xee, 0xcc);
    else
        c = QColor(0xee, 0xcc, 0xcc);

    QPalette pal(palette());
    pal.setColor(QPalette::Base, c);
    setPalette(pal);
}

void HostnameLineEdit::lookupComplete(const QHostInfo &info)
{
    if (info.error() != QHostInfo::NoError)
        setStatusGood(false);
    else
        setStatusGood(true);
}
