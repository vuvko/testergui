#ifndef GAMEFIGHT_H
#define GAMEFIGHT_H

#include "gamerules.h"
#include "logic.h"
#include "../errors.h"
#include "IMMPui.h"

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
};

} // end of logic namespace
} // end of mmp namespace

#endif // GAMEFIGHT_H
