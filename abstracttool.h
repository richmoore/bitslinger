#ifndef ABSTRACTTOOL_H
#define ABSTRACTTOOL_H

#include <QObject>
#include <QIcon>

class QWidget;

class BitSlinger;

class AbstractTool : public QObject
{
    Q_OBJECT
public:
    explicit AbstractTool(QObject *parent = 0);

    BitSlinger *bitSlinger() const { return m_slinger; }
    void setBitSlinder(BitSlinger *slinger);

    QString name() const { return m_name; }
    QWidget *widget() const { return m_widget; }
    QIcon icon() const { return m_icon; }

    virtual bool restoreState(const QByteArray &state);
    virtual QByteArray saveState();

signals:
    void bitSlingerChanged(BitSlinger *slinger);

public slots:

protected:
    void setName(const QString &name) { m_name = name; }
    void setWidget(QWidget *widget) { m_widget = widget; }
    void setIcon(const QIcon &icon) { m_icon = icon; }

private:
    BitSlinger *m_slinger;
    QWidget *m_widget;
    QString m_name;
    QIcon m_icon;
};

#endif // ABSTRACTTOOL_H
