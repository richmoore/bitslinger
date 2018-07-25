#ifndef REWRITERULE_H
#define REWRITERULE_H

#include <QByteArray>

//### TODO: Make implicitly shared?

class MatchRule
{
public:
    enum Action {
        ActionNone,
        ActionReplace
    };

    MatchRule();

    setEnabled(bool enable);
    isEnabled() const { return m_enabled; }

    Action action() const { return m_action; }

    QByteArray search() const { return m_action; }
    QByteArray replace() const { return m_replace; }

    setRewriteRule(const QByteArray &search, const QByteArray &replace);

    bool m_enabled;
    QByteArray m_search;
    Action m_action;
    QByteArray m_replace;
};

#endif // REWRITERULE_H
