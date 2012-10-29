#include "IMMPGuiImpl.h"

#include <map>

using namespace std;

namespace mmp
{
namespace gui
{
    class PaintStar
    {
        FieldWidget *widget;
        MMPQtGuiImpl& mmpGuiImpl;

    public:
        void operator() (const map<Point, const Star*>::value_type& p)
        {
            const Star* star = p.second;
            QStarPainter starPainter(*star);
            starPainter.draw();
            widget->drawObject(star->getPoint(), starPainter.pixmap());
        }

        PaintStar(FieldWidget *widget, MMPQtGuiImpl& mmpGuiImpl) :widget(widget), mmpGuiImpl(mmpGuiImpl) {}
    };

    class PaintBlock
    {
        FieldWidget *widget;
        MMPQtGuiImpl& mmpGuiImpl;

    public:
        void operator() (const map<Point, const Block*>::value_type& p)
        {
            const Block* block = p.second;
            QBlockPainter blockPainter(*block);
            blockPainter.draw();
            widget->drawObject(block->getPoint(), blockPainter.pixmap());
        }

        PaintBlock(FieldWidget *widget, MMPQtGuiImpl& mmpGuiImpl) :widget(widget), mmpGuiImpl(mmpGuiImpl) {}
    };

    class PaintEmpty
    {
        FieldWidget *widget;
        MMPQtGuiImpl& mmpGuiImpl;

    public:
        void operator() (const map<Point, const Empty*>::value_type& p)
        {
            const Empty* empty = p.second;
            QEmptyPainter emptyPainter(*empty);
            emptyPainter.draw();
            widget->drawObject(empty->getPoint(), emptyPainter.pixmap());
        }

        PaintEmpty(FieldWidget *widget, MMPQtGuiImpl& mmpGuiImpl) :widget(widget), mmpGuiImpl(mmpGuiImpl) {}
    };

    class PaintNumber
    {
        FieldWidget *widget;
        MMPQtGuiImpl& mmpGuiImpl;

    public:
        void operator() (const map<Point, const Number*>::value_type& p)
        {
            const Number* number = p.second;
            QNumberPainter NumberPainter(*number);
            NumberPainter.draw(number->getTeamId());
            widget->drawObject(number->getPoint(), NumberPainter.pixmap());
        }

        PaintNumber(FieldWidget *widget, MMPQtGuiImpl& mmpGuiImpl) :widget(widget), mmpGuiImpl(mmpGuiImpl) {}
    };

    class PaintChecker
    {
        FieldWidget *widget;
        MMPQtGuiImpl& mmpGuiImpl;

    public:
        void operator() (const map<Point, const Checker*>::value_type& p)
        {
            const Checker* checker = p.second;
            QCheckerPainter checkerPainter(*checker);
            checkerPainter.draw(mmpGuiImpl.teamColor[checker->getTeamId()]);
            widget->drawObject(checker->getPoint(), checkerPainter.pixmap());
        }

        PaintChecker(FieldWidget *widget, MMPQtGuiImpl& mmpGuiImpl) :widget(widget), mmpGuiImpl(mmpGuiImpl) {}
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
}
}
