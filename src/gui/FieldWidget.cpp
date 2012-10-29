#include "FieldWidget.h"

#include "IMMPGuiImpl.h"

#include <QtGui>

FieldWidget::FieldWidget(QWidget *parent) :
    QWidget(parent),
    pen_(QColor(0, 0, 255, 96))
{
    setFixedSize(200, 200);
    pen_.setWidth(200);
    //pen_.setCapStyle(Qt::RoundCap);
    //pen_.setJoinStyle(Qt::RoundJoin);
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
