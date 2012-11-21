#ifndef IMMPGUIIMPL_H
#define IMMPGUIIMPL_H

#include <QWidget>
#include <QImage>
#include <QPen>
#include <QColor>
#include <QMessageBox>
#include <QPainter>
#include <QMutex>
#include <QEvent>
#include <QResizeEvent>

#include <map>
#include <algorithm>

#include "IMMPui.h"
#include "FieldWidget.h"
#include "QFieldObject.h"

using namespace std;

namespace mmp
{
namespace gui
{
using namespace ui;
class GuiImplementedStar : public Star
{
    Point pos;

public:
    GuiImplementedStar(const Point &p) : pos(p) {}
    GuiImplementedStar(const Star *s)
        : pos(s->getPoint()) {}

    Point getPoint() const { return pos; }
};

class GuiImplementedBlock : public Block
{
    Point pos;

public:
    GuiImplementedBlock(const Point &p) : pos(p) {}
    GuiImplementedBlock(const Block *b)
        : pos(b->getPoint()) {}

    Point getPoint() const { return pos; }
};

class GuiImplementedEmpty : public Empty
{
    Point pos;

public:
    GuiImplementedEmpty(const Point &p) : pos(p) {}
    GuiImplementedEmpty(const Empty *e)
        : pos(e->getPoint()) {}

    Point getPoint() const { return pos; }
};

class GuiImplementedNumber : public Number
{
    Point pos;
    int teamId;

public:
    GuiImplementedNumber(const Point &p, int teamId)
        : pos(p), teamId(teamId) {}
    GuiImplementedNumber(const Number *n)
        : pos(n->getPoint()), teamId(n->getTeamId()) {}

    Point getPoint() const { return pos; }
    int getTeamId() const { return teamId; }
};

class GuiImplementedChecker : public Checker
{
    Point pos;
    int teamId;

public:
    GuiImplementedChecker(const Point &p, int teamId)
        : pos(p), teamId(teamId) {}
    GuiImplementedChecker(const Checker *c)
        : pos(c->getPoint()), teamId(c->getTeamId()) {}

    Point getPoint() const { return pos; }
    int getTeamId() const { return teamId; }
};

class MMPQtGuiImpl : public IMMPui
{
    FieldWidget *widget;
    QMutex paintLock;
    map<Point, const Star *> starMap;
    map<Point, const Block *> blockMap;
    map<Point, const Empty *> emptyMap;
    map<Point, const Number *> numberMap;
    map<Point, const Checker *> checkerMap;

public:
    map<int, QColor> teamColor;

    MMPQtGuiImpl()
    {
        starMap.clear();
        blockMap.clear();
        emptyMap.clear();
        numberMap.clear();
        checkerMap.clear();
    }

    void init(FieldWidget *widget_)
    {
        widget = widget_;
        teamColor.insert(pair<int, QColor>(0, QColor(Qt::red)));
        teamColor.insert(pair<int, QColor>(1, QColor(Qt::blue)));
    }

    virtual void SetStar(const Star *star)
    {
        GuiImplementedStar *new_star = new GuiImplementedStar(star);
        starMap.insert(pair<Point, const Star *>(new_star->getPoint(), new_star));
    }

    virtual void SetBlock(const Block *block)
    {
        GuiImplementedBlock *new_block = new GuiImplementedBlock(block);
        blockMap.insert(pair<Point, const Block *>(new_block->getPoint(), new_block));
    }

    virtual void SetEmpty(const Empty *empty)
    {
        GuiImplementedEmpty *new_empty = new GuiImplementedEmpty(empty);
        emptyMap.insert(pair<Point, const Empty *>(new_empty->getPoint(), new_empty));
    }

    virtual void SetNumber(const Number *number)
    {
        GuiImplementedNumber *new_number = new GuiImplementedNumber(number);
        numberMap.insert(pair<Point, const Number *>(new_number->getPoint(), new_number));
    }

    virtual void SetChecker(const Checker *checker)
    {
        GuiImplementedChecker *new_checker = new GuiImplementedChecker(checker);
        checkerMap.insert(pair<Point, const Checker *>(new_checker->getPoint(), new_checker));
    }

    virtual void Clear() // And why not destructor?
    //~MMPQtGuiImpl()
    {
        for_each(starMap.begin(), starMap.end(), deleteMap());
        for_each(blockMap.begin(), blockMap.end(), deleteMap());
        for_each(emptyMap.begin(), emptyMap.end(), deleteMap());
        for_each(numberMap.begin(), numberMap.end(), deleteMap());
        for_each(checkerMap.begin(), checkerMap.end(), deleteMap());
        starMap.clear();
        blockMap.clear();
        emptyMap.clear();
        numberMap.clear();
        checkerMap.clear();
    }

    virtual void Paint();

    virtual void BeginPaint()
    {
        paintLock.lock();
        Clear();
    }

    virtual void EndPaint()
    {
        paintLock.unlock();
    }
};

} // end of gui namespace
} // end of mmp namespace

#endif
