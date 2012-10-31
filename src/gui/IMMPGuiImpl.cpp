#include <map>

#include "IMMPGuiImpl.h"

using namespace std;

namespace mmp
{
namespace gui
{

class PaintStar
{
    FieldWidget *widget;
    MMPQtGuiImpl &mmpGuiImpl;

public:
    void operator() (const pair<Point, const Star *> &p)
    {
        const Star *star = p.second;
        QStarPainter starPainter(*star, widget->getSize());
        starPainter.draw();
        widget->drawObject(star->getPoint(), starPainter.pixmap());
    }

    PaintStar(FieldWidget *widget, MMPQtGuiImpl &mmpGuiImpl) :widget(widget), mmpGuiImpl(mmpGuiImpl) {}
};

class PaintBlock
{
    FieldWidget *widget;
    MMPQtGuiImpl &mmpGuiImpl;

public:
    void operator() (const pair<Point, const Block *> &p)
    {
        const Block* block = p.second;
        QBlockPainter blockPainter(*block, widget->getSize());
        blockPainter.draw();
        widget->drawObject(block->getPoint(), blockPainter.pixmap());
    }

    PaintBlock(FieldWidget *widget, MMPQtGuiImpl &mmpGuiImpl) :widget(widget), mmpGuiImpl(mmpGuiImpl) {}
};

class PaintEmpty
{
    FieldWidget *widget;
    MMPQtGuiImpl &mmpGuiImpl;

public:
    void operator() (const pair<Point, const Empty *> &p)
    {
        const Empty *empty = p.second;
        QEmptyPainter emptyPainter(*empty, widget->getSize());
        emptyPainter.draw();
        widget->drawObject(empty->getPoint(), emptyPainter.pixmap());
    }

    PaintEmpty(FieldWidget *widget, MMPQtGuiImpl &mmpGuiImpl) :widget(widget), mmpGuiImpl(mmpGuiImpl) {}
};

class PaintNumber
{
    FieldWidget *widget;
    MMPQtGuiImpl &mmpGuiImpl;

public:
    void operator() (const pair<Point, const Number *> &p)
    {
        const Number *number = p.second;
        QNumberPainter NumberPainter(*number, widget->getSize());
        NumberPainter.draw(number->getTeamId());
        widget->drawObject(number->getPoint(), NumberPainter.pixmap());
    }

    PaintNumber(FieldWidget *widget, MMPQtGuiImpl &mmpGuiImpl) :widget(widget), mmpGuiImpl(mmpGuiImpl) {}
};

class PaintChecker
{
    FieldWidget *widget;
    MMPQtGuiImpl &mmpGuiImpl;

public:
    void operator() (const pair<Point, const Checker *> &p)
    {
        const Checker *checker = p.second;
        QCheckerPainter checkerPainter(*checker, widget->getSize());
        checkerPainter.draw(mmpGuiImpl.teamColor[checker->getTeamId()]);
        widget->drawObject(checker->getPoint(), checkerPainter.pixmap());
    }

    PaintChecker(FieldWidget *widget, MMPQtGuiImpl &mmpGuiImpl) :widget(widget), mmpGuiImpl(mmpGuiImpl) {}
};

void MMPQtGuiImpl::Paint()
{
    paintLock.lock();
    widget->clear();
    PaintStar paintStar(widget, *this);
    PaintBlock paintBlock(widget, *this);
    PaintEmpty paintEmpty(widget, *this);
    PaintNumber paintNumber(widget, *this);
    PaintChecker paintChecker(widget, *this);
    for_each(starMap.begin(), starMap.end(), paintStar);
    for_each(blockMap.begin(), blockMap.end(), paintBlock);
    for_each(emptyMap.begin(), emptyMap.end(), paintEmpty);
    for_each(numberMap.begin(), numberMap.end(), paintNumber);
    for_each(checkerMap.begin(), checkerMap.end(), paintChecker);
    paintLock.unlock();
}

} // end of gui namespace
} // end of mmp namespace
