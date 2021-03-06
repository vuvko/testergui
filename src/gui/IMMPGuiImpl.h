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

typedef Symbol GuiImplementedStar;
typedef Symbol GuiImplementedBlock;
typedef Symbol GuiImplementedEmpty;

typedef Token GuiImplementedNumber;
typedef Token GuiImplementedChecker;


class MMPQtGuiImpl : public IMMPui
{
    FieldWidget *widget;
    QMutex paintLock;
    map<Point, const Symbol *> starMap;
    map<Point, const Symbol *> blockMap;
    map<Point, const Symbol *> emptyMap;
    map<Point, const Token *> numberMap;
    map<Point, const Token *> checkerMap;

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
