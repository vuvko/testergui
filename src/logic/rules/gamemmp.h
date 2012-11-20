#ifndef GAMEMMP_H
#define GAMEMMP_H

#include "gamerules.h"
#include "logic.h"
#include "errors.h"
#include "IMMPGui.h"

namespace mmp
{
namespace logic
{

class GameMMP : public GameRules
{
public:
    GameMMP();

    Position checkMove(const mmp::gui::Point &from, const mmp::gui::Point &to);

private:
    bool checkEnd();
    bool checkField();
};

} // end of logic namespace
} // end of mmp namespace

#endif // GAMEMMP_H
