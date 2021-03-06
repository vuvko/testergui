#ifndef GAMEKNIGHTS_H
#define GAMEKNIGHTS_H

#include <vector>
#include <QDebug>

#include "gamerules.h"
#include "logic.h"
#include "lerrors.h"
#include "IMMPui.h"

namespace mmp
{
namespace logic
{

class GameKnights : public GameRules
{
public:
    GameKnights();

    Position checkMove(const mmp::ui::Point &from, const mmp::ui::Point &to);

private:
    bool checkEnd();
    bool checkField();
};

} // end of logic namespace
} // end of mmp namespace

#endif // GAMEKNIGHTS_H
