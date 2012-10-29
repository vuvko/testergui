#include "FieldWidget.h"

#include "IMMPGuiImpl.h"

#include <QtGui>

FieldWidget::FieldWidget(QWidget *parent) :
    QWidget(parent),
    pen(QColor(0, 0, 255, 96))
{
    setFixedSize(200, 200);
    pen.setWidth(200);
    //pen.setCapStyle(Qt::RoundCap);
    //pen.setJoinStyle(Qt::RoundJoin);
}

void FieldWidget::drawObject(const mmp::gui::Point& point, const QPixmap& pixmap)
{
    QPainter painter(this);
    painter.drawPixmap(point.x * CELL_SIZE, point.y * CELL_SIZE, pixmap);
}

void FieldWidget::paintEvent(QPaintEvent*)
{
    gui->Paint();
}
