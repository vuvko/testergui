#ifndef LOGIC_H
#define LOGIC_H

#include <list>

#include "ui/IMMPui.h"

namespace mmp
{
namespace logic
{

static const int field_width = 8;
static const int field_height = 8;
static const double eps = 0.001;

bool isToken(const char c);

class Star : public mmp::ui::Star
{
    mmp::ui::Point pos;

public:
    Star(const mmp::ui::Point &p) : pos(p) {}
    Star(const mmp::ui::Star *s)
        : pos(s->getPoint()) {}

    mmp::ui::Point getPoint() const { return pos; }
};

class Block : public mmp::ui::Block
{
    mmp::ui::Point pos;

public:
    Block(const mmp::ui::Point &p) : pos(p) {}
    Block(const mmp::ui::Block *b)
        : pos(b->getPoint()) {}

    mmp::ui::Point getPoint() const { return pos; }
};

class Empty : public mmp::ui::Empty
{
    mmp::ui::Point pos;

public:
    Empty(const mmp::ui::Point &p) : pos(p) {}
    Empty(const mmp::ui::Empty *e)
        : pos(e->getPoint()) {}

    mmp::ui::Point getPoint() const { return pos; }
};

class Number : public mmp::ui::Number
{
    mmp::ui::Point pos;
    int teamId;

public:
    Number(const mmp::ui::Point &p, int teamId)
        : pos(p), teamId(teamId) {}
    Number(const mmp::ui::Number *n)
        : pos(n->getPoint()), teamId(n->getTeamId()) {}

    mmp::ui::Point getPoint() const { return pos; }
    int getTeamId() const { return teamId; }
};

class Checker : public mmp::ui::Checker
{
    mmp::ui::Point pos;
    int teamId;

public:
    Checker(const mmp::ui::Point &p, int teamId)
        : pos(p), teamId(teamId) {}
    Checker(const mmp::ui::Checker *c)
        : pos(c->getPoint()), teamId(c->getTeamId()) {}

    mmp::ui::Point getPoint() const { return pos; }
    int getTeamId() const { return teamId; }
};

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
