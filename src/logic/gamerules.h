#ifndef GAMERULES_H
#define GAMERULES_H

#include "logic.h"

namespace mmp
{
namespace logic
{

class GameRules
{
public:
    GameRules();
    virtual ~GameRules() {}

    void setPosition(const Position &pos_);

    virtual Position checkMove(
            const mmp::gui::Point &from,
            const mmp::gui::Point &to) = 0;
    virtual bool checkField() = 0;

protected:
    Position pos;
    int player;
    enum
    {
        FIRST_PLAYER = 1,
        SECOND_PLAYER = 0,
        A = 1,
        B = 0,
        FIRST_LETTER = 'A',
        SECOND_LETTER = 'B',
        SPACE = '-',
        MARK1 = '1',
        MARK2 = '2',
        ASTERISK = '*'
    };

    virtual bool checkEnd() = 0;
};

} // end of logic namepspace
} // end of mmp namespace

#endif // GAMERULES_H
