#include "IMMPui.h"

namespace mmp
{
namespace ui
{

Symbol::Symbol(const Point &p) : pos(p) {}

Symbol::Symbol(const Symbol *s) : pos(s->getPoint()) {}

Point Symbol::getPoint() const
{
    return pos;
}

Token::Token(const Point &p, int team) : pos(p), teamId(team) {}

Token::Token(const Token *t) : pos(t->getPoint()), teamId(t->getTeamId()) {}

Point Token::getPoint() const
{
    return pos;
}

int Token::getTeamId() const
{
    return teamId;
}

void IMMPui::ShowError(const std::string error)
{
    QMessageBox::critical(0, "Fatal Error", QString(error.c_str()), QMessageBox::Ok);
}

} // end of ui namespace
} // end of mmp namespace
