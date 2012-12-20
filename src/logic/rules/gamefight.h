#ifndef GAMEFIGHT_H
#define GAMEFIGHT_H

#include "gamerules.h"
#include "logic.h"
#include "lerrors.h"
#include "IMMPui.h"

#include <map>

namespace mmp
{
namespace logic
{

class GameFight : public GameRules
{
public:
    GameFight();

    Position checkMove(const mmp::ui::Point &from, const mmp::ui::Point &to);

private:
    bool checkEnd();
    bool checkField();

    Position unknownMove(const mmp::ui::Point &from);
    mmp::ui::Point moveFrom(const mmp::ui::Point &from, int dir) const;
    Position findMove(void);
    void moveTo(const mmp::ui::Point &to);

    mmp::ui::Point oldFrom;
    Position oldPos;
};

} // end of logic namespace
} // end of mmp namespace

#endif // GAMEFIGHT_H
