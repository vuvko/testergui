#ifndef MANAGER_H
#define MANAGER_H

#include "errors.h"

#include "IMMPGui.h"
#include "IMMPGuiImpl.h"

#include <QFile>

#include <list>

using mmp::gui::Point;

using namespace std;

namespace mmp
{
namespace logic
{
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

    struct Map
    {
        static const int width = 8;
        static const int height = 8;

        char _map[width][height];

        char at(int x, int y) const;
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

        Map map;
    };

    class Manager
    {
        static const int width = 8;
        static const int height = 8;

    public:
        static Position parsePos(const char* matrixPath, int gameId);
        static char* parseTurn(Position& p1, Position &p2);
        static void paintPos(const Position& p, mmp::gui::IMMPGui* gui);
    };
}
}

#endif
