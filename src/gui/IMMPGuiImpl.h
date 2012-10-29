#ifndef IMMPGUIIMPL_H
#define IMMPGUIIMPL_H

#include "IMMPGui.h"
#include "FieldWidget.h"

#include <QMessageBox>
#include <QPainter>
#include <QMutex>

#include <map>
#include <algorithm>

// TODO: make CELL_SIZE changeable (as field for example)
enum {CELL_SIZE = 25};

using namespace std;

namespace mmp
{
namespace gui
{
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

    class QFieldObject
    {
    protected:
        QPixmap pix;
        QFieldObject() : pix(CELL_SIZE, CELL_SIZE) {}
    public:
        const QPixmap& pixmap() const
        {
            return pix;
        }
    };

    class QStarPainter : public QFieldObject
    {
        const Star &star;
    public:
        QStarPainter(const Star &star) : star(star) {}

        virtual void draw()
        {
            QPainter painter(&pix);
            painter.setBrush(QBrush(QColor(Qt::white)));
            painter.drawRect(0, 0, CELL_SIZE-1, CELL_SIZE-1);
            painter.setBrush(QBrush(QColor(Qt::gray)));
            painter.setFont(QFont("Arial", CELL_SIZE));
            painter.drawText(CELL_SIZE / 4, CELL_SIZE * 5 / 4, "*");
        }
    };

    class QBlockPainter : public QFieldObject
    {
        const Block &block;
    public:
        QBlockPainter(const Block &block) : block(block) {}

        virtual void draw()
        {
            QPainter painter(&pix);
            painter.setBrush(QBrush(QColor(Qt::darkGray)));
            painter.drawRect(0, 0, CELL_SIZE-1, CELL_SIZE-1);
        }
    };

    class QEmptyPainter : public QFieldObject
    {
        const Empty &empty;
    public:
        QEmptyPainter(const Empty &empty) : empty(empty) {}

        virtual void draw()
        {
            QPainter painter(&pix);
            painter.setBrush(QBrush(QColor(Qt::white)));
            painter.drawRect(0, 0, CELL_SIZE-1, CELL_SIZE-1);
        }
    };

    class QNumberPainter : public QFieldObject
    {
        const Number &number;
    public:
        QNumberPainter(const Number &number) : number(number) {}

        virtual void draw(int teamId)
        {
            QPainter painter(&pix);
            painter.setBrush(QBrush(QColor(Qt::white)));
            painter.drawRect(0, 0, CELL_SIZE-1, CELL_SIZE-1);
            painter.setBrush(QBrush(QColor(Qt::gray)));
            painter.setFont(QFont("Arial", CELL_SIZE * 5 / 9));
            if (teamId == 1)
            {
                painter.drawText(CELL_SIZE / 4 + 2, CELL_SIZE * 3 / 4 + 1, "1");
            }
            else
            {
                painter.drawText(CELL_SIZE / 4 + 2, CELL_SIZE * 3 / 4 + 1, "2");
            }
        }
    };

    class QCheckerPainter : public QFieldObject
    {
        const Checker &checker;
    public:
        QCheckerPainter(const Checker &checker) : checker(checker) {}

        virtual void draw(QColor color)
        {
            QPainter painter(&pix);
            painter.setBrush(QBrush(Qt::white));
            painter.drawRect(0, 0, CELL_SIZE-1, CELL_SIZE-1);
            painter.setBrush(QBrush(color));
            painter.drawEllipse(2, 2, CELL_SIZE-5, CELL_SIZE-5);
        }
    };

    class deleteMap
    {
    public:
        template <class T> void operator() (T m)
        {
            delete m.second;
        }
    };

    class MMPQtGuiImpl : public IMMPGui
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

        void SetStar(const Star *star)
        {
            GuiImplementedStar *new_star = new GuiImplementedStar(star);
            starMap.insert(pair<Point, const Star *>(new_star->getPoint(), new_star));
        }

        void SetBlock(const Block *block)
        {
            GuiImplementedBlock *new_block = new GuiImplementedBlock(block);
            blockMap.insert(pair<Point, const Block *>(new_block->getPoint(), new_block));
        }

        void SetEmpty(const Empty *empty)
        {
            GuiImplementedEmpty *new_empty = new GuiImplementedEmpty(empty);
            emptyMap.insert(pair<Point, const Empty *>(new_empty->getPoint(), new_empty));
        }

        void SetNumber(const Number *number)
        {
            GuiImplementedNumber *new_number = new GuiImplementedNumber(number);
            numberMap.insert(pair<Point, const Number *>(new_number->getPoint(), new_number));
        }

        void SetChecker(const Checker *checker)
        {
            GuiImplementedChecker *new_checker = new GuiImplementedChecker(checker);
            checkerMap.insert(pair<Point, const Checker *>(new_checker->getPoint(), new_checker));
        }

        void Clear() // And why not destructor?
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

        void Paint();

        void BeginPaint()
        {
            paintLock.lock();
            Clear();
        }

        void EndPaint()
        {
            paintLock.unlock();
        }
    };
} // end of gui namespace
} // end of mmp namespace

#endif
