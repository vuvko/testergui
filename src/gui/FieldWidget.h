#ifndef FIELDWIDGET_HPP
#define FIELDWIDGET_HPP

#include "IMMPGui.h"
#include "QFieldObject.h"

#include <QPainter>
#include <QWidget>
#include <QPixmap>
#include <QImage>
#include <QColor>
#include <QPen>
#include <QResizeEvent>
#include <QWidget>

#include <QDebug>

namespace mmp
{
namespace gui
{

class FieldWidget : public QWidget, public QFieldObject
{
    Q_OBJECT

private:
    QPen pen;
    mmp::gui::IMMPGui *gui;

public:
    FieldWidget(QWidget *parent = 0);

    void drawObject(const mmp::gui::Point&, const QPixmap&);
    void setSize(int size);
    int getSize() const;

    void clear()
    {
        QPainter painter(this);
        painter.setPen(pen);
        painter.drawRect(0, 0, CELL_NUM * cellSize, CELL_NUM * cellSize);
    }

    void setGui(mmp::gui::IMMPGui *gui)
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
