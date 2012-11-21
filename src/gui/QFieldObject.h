#ifndef QFIELDOBJECT_H
#define QFIELDOBJECT_H

#include <QPixmap>
#include <QPainter>

enum {DEFAULT_CELL_SIZE = 25};

namespace mmp
{
namespace gui
{
using namespace mmp::ui;
class QFieldObject
{
protected:
    QPixmap pix;
    int cellSize;
    QFieldObject(int size) : pix(size, size), cellSize(size) {}
public:
    enum {CELL_NUM = 8};
    const QPixmap &pixmap() const
    {
        return pix;
    }
    virtual ~QFieldObject() {}
};

class QStarPainter : public QFieldObject
{
    const Star &star;
public:
    QStarPainter(const Star &star, int size) : QFieldObject(size), star(star) {}

    virtual void draw()
    {
        QPainter painter(&pix);
        painter.setBrush(QBrush(QColor(Qt::white)));
        painter.drawRect(0, 0, cellSize - 1, cellSize - 1);
        painter.setBrush(QBrush(QColor(Qt::gray)));
        painter.setFont(QFont("Arial", cellSize));
        painter.drawText(cellSize / 4, cellSize * 5 / 4, "*");
    }
};

class QBlockPainter : public QFieldObject
{
    const Block &block;
public:
    QBlockPainter(const Block &block, int size) : QFieldObject(size), block(block) {}

    virtual void draw()
    {
        QPainter painter(&pix);
        painter.setBrush(QBrush(QColor(Qt::darkGray)));
        painter.drawRect(0, 0, cellSize - 1, cellSize - 1);
    }
};

class QEmptyPainter : public QFieldObject
{
    const Empty &empty;
public:
    QEmptyPainter(const Empty &empty, int size) : QFieldObject(size), empty(empty) {}

    virtual void draw()
    {
        QPainter painter(&pix);
        painter.setBrush(QBrush(QColor(Qt::white)));
        painter.drawRect(0, 0, cellSize - 1, cellSize - 1);
    }
};

class QNumberPainter : public QFieldObject
{
    const Number &number;
public:
    QNumberPainter(const Number &number, int size) : QFieldObject(size), number(number) {}

    virtual void draw(int teamId)
    {
        QPainter painter(&pix);
        painter.setBrush(QBrush(QColor(Qt::white)));
        painter.drawRect(0, 0, cellSize - 1, cellSize - 1);
        painter.setBrush(QBrush(QColor(Qt::gray)));
        painter.setFont(QFont("Arial", cellSize * 5 / 9));
        if (teamId == 1)
        {
            painter.drawText(cellSize / 4 + 2, cellSize * 3 / 4 + 1, "1");
        }
        else
        {
            painter.drawText(cellSize / 4 + 2, cellSize * 3 / 4 + 1, "2");
        }
    }
};

class QCheckerPainter : public QFieldObject
{
    const Checker &checker;
public:
    QCheckerPainter(const Checker &checker, int size) : QFieldObject(size), checker(checker) {}

    virtual void draw(QColor color)
    {
        QPainter painter(&pix);
        painter.setBrush(QBrush(Qt::white));
        painter.drawRect(0, 0, cellSize - 1, cellSize - 1);
        painter.setBrush(QBrush(color));
        painter.drawEllipse(2, 2, cellSize - 5, cellSize - 5);
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

} // end of gui namespace
} // end of mmp namespace

#endif // QFIELDOBJECT_H
