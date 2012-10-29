#ifndef FIELDWIDGET_HPP
#define FIELDWIDGET_HPP

#include "IMMPGui.h"

#include <QPainter>
#include <QWidget>
#include <QPixmap>
#include <QImage>
#include <QColor>
#include <QPen>

class FieldWidget : public QWidget
{
    Q_OBJECT

private:
    QPixmap pix_;
    QPen pen_;
    mmp::gui::IMMPGui* gui;

public:
    FieldWidget(QWidget *parent = 0);

    void drawObject(const mmp::gui::Point&, const QPixmap&);

    QPixmap const& pixmap() const { return pix_; }

    void clear()
    {
        QPainter painter(this);
        painter.setPen(pen_);
        painter.drawRect(0, 0, width(), height());
    }

    void setGui(mmp::gui::IMMPGui* gui)
    {
        this->gui = gui;
    }

protected:
    void paintEvent(QPaintEvent* ev);
};

#endif
