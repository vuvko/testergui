#include "FieldWidget.h"

mmp::gui::FieldWidget::FieldWidget(QWidget *parent) :
    QWidget(parent),
    mmp::gui::QFieldObject(DEFAULT_CELL_SIZE),
    pen(QColor(0, 0, 255, 96))
{
    int w = size().width();
    int h = size().height();

    int s = qMin(w, h);
    cellSize = s / CELL_NUM;
}

void mmp::gui::FieldWidget::setSize(int size_ = 0)
{
    cellSize = size_ / CELL_NUM;
}

int mmp::gui::FieldWidget::getSize() const
{
    return cellSize;
}

void mmp::gui::FieldWidget::drawObject(const mmp::gui::Point &point, const QPixmap &pixmap)
{
    QPainter painter(this);
    painter.drawPixmap(point.x * cellSize, point.y * cellSize, pixmap);
}
