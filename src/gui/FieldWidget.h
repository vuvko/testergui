#ifndef FIELDWIDGET_HPP
#define FIELDWIDGET_HPP

#include <QPainter>
#include <QWidget>
#include <QPixmap>
#include <QImage>
#include <QColor>
#include <QPen>
#include <QResizeEvent>
#include <QWidget>

#include <QDebug>

#include "IMMPui.h"
#include "QFieldObject.h"

namespace mmp
{
namespace gui
{

class FieldWidget : public QWidget, public QFieldObject
{
    Q_OBJECT

private:
    QPen pen;
    mmp::ui::IMMPui *gui;

public:
    FieldWidget(QWidget *parent = 0);

    void drawObject(const mmp::ui::Point &, const QPixmap&);
    void setSize(int size);
    int getSize() const;

    void clear()
    {
        QPainter painter(this);
        painter.setPen(pen);
        painter.drawRect(0, 0, CELL_NUM * cellSize, CELL_NUM * cellSize);
    }

    void setGui(mmp::ui::IMMPui *gui)
    {
        this->gui = gui;
    }

protected:
    virtual void paintEvent(QPaintEvent *)
    {
        gui->Paint();
    }
};

} // end of gui namespace
} // end of mmp namespace

#endif
