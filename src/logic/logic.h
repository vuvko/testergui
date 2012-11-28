#ifndef LOGIC_H
#define LOGIC_H

#include <list>

#include "IMMPui.h"

namespace mmp
{
namespace logic
{

static const int field_width = 8;
static const int field_height = 8;
static const double eps = 0.001;

bool isToken(const char c);

typedef mmp::ui::Symbol Star;
typedef mmp::ui::Symbol Block;
typedef mmp::ui::Symbol Empty;

typedef mmp::ui::Token Checker;
typedef mmp::ui::Token Number;

struct Field
{
    std::list<Star> stars;
    std::list<Block> blocks;
    std::list<Empty> emptys;
    std::list<Number> numbers;
    std::list<Checker> checkers;
};

class CharField
{
    char field[field_height][field_width];
public:
    char at(int x, int y) const;
    char at(const mmp::ui::Point &p) const;
    int width() const;
    int height() const;

    void set(int x, int y, char c);
    void set(const mmp::ui::Point &p, char c);
};

enum GameState
{
    A_GOES,
    B_GOES,
    DRAW_GAME,
    A_HAS_WON,
    B_HAS_WON
};

struct Position
{
    int gameId;

    GameState state;
    int step;

    int scoreA, scoreB;
    double leftA, leftB;

    CharField field;
};

} // end of logic namespace
} // end of mmp namespace

#endif // LOGIC_H
