#include "matchrule.h"

MatchRule::MatchRule()
    : action(ActionNone)
{

}

MatchRule::setRewriteRule(const QByteArray &search, const QByteArray &replace)
{
    assert(!search.isEmpty());

    m_enabled = true;
    m_search = search;
    m_replace = replace;
    m_action = ActionReplace;
}
