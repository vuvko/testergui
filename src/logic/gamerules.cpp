#include "gamerules.h"

namespace mmp
{
namespace logic
{

GameRules::GameRules() : pos(), player() {}

void GameRules::setPosition(const Position &pos_)
{
    pos = pos_;
}

} // end of logic namespace
} // end of mmp namespace
