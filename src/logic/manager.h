#ifndef MANAGER_H
#define MANAGER_H

// TODO: separate logic from gui. Specifically make logic namespace do not use gui namespace.
//       It might be usefull when implementing console version... Maybe not to do it...

#include <QFile>
#include <QDebug>

#include <list>
#include <fstream>

#include "errors.h"

#include "IMMPGui.h"

using mmp::gui::Point;

using namespace std;

namespace mmp
{
namespace logic
{
static const int field_width = 8;
static const int field_height = 8;
static const double eps = 0.001;

class Star : public mmp::gui::Star
{
    Point pos;

public:
    Star(const Point &p) : pos(p) {}
    Star(const mmp::gui::Star *s)
        : pos(s->getPoint()) {}

    Point getPoint() const { return pos; }
};

class Block : public mmp::gui::Block
{
    Point pos;

public:
    Block(const Point &p) : pos(p) {}
    Block(const mmp::gui::Block *b)
        : pos(b->getPoint()) {}

    Point getPoint() const { return pos; }
};

class Empty : public mmp::gui::Empty
{
    Point pos;

public:
    Empty(const Point &p) : pos(p) {}
    Empty(const mmp::gui::Empty *e)
        : pos(e->getPoint()) {}

    Point getPoint() const { return pos; }
};

class Number : public mmp::gui::Number
{
    Point pos;
    int teamId;

public:
    Number(const Point &p, int teamId)
        : pos(p), teamId(teamId) {}
    Number(const mmp::gui::Number *n)
        : pos(n->getPoint()), teamId(n->getTeamId()) {}

    Point getPoint() const { return pos; }
    int getTeamId() const { return teamId; }
};

class Checker : public mmp::gui::Checker
{
    Point pos;
    int teamId;

public:
    Checker(const Point &p, int teamId)
        : pos(p), teamId(teamId) {}
    Checker(const mmp::gui::Checker *c)
        : pos(c->getPoint()), teamId(c->getTeamId()) {}

    Point getPoint() const { return pos; }
    int getTeamId() const { return teamId; }
};

struct Field
{
    list<Star> stars;
    list<Block> blocks;
    list<Empty> emptys;
    list<Number> numbers;
    list<Checker> checkers;
};

class CharField
{
    char field[field_width][field_height];
public:
    char at(int x, int y) const;
    int width() const;
    int height() const;

    void set(int x, int y, char c);
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

class Manager
{
public:
    static Position parsePos(const char *matrixPath, int gameId);
    static char *parseTurn(Position &p1, Position &p2, double realTime = 30);
    static void paintPos(const Position &p, mmp::gui::IMMPGui *gui);

private:
    static const double IOTime = 0.5;
};

} // end of logic namespace
} // end of mmp namespace

#endif
