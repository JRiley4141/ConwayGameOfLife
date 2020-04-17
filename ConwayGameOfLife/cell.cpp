
#include "cell.h"

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QtWidgets>


Cell::Cell(QColor color, const int x, const int y){

    this->color_ = color;
    x_ = x;
    y_ = y;
    is_alive_ = false;

}

//so Qt knows "where the user would be interacting with this object
QRectF Cell::boundingRect() const {
    return QRectF(x_, y_, width_, width_);
}

//define the shape of the object
QPainterPath Cell::shape() const {
    QPainterPath path;
    path.addRect(x_, y_, width_, width_);
    return path;
}

//called by Qt to display the Cell
void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget);

    QBrush b = painter->brush();

    painter->setBrush(QBrush(color_));
    painter->drawRect(QRect(this->x_, this->y_, this->width_, this->width_));
    painter->setBrush(b);
}

void Cell::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "Cell Clicked!";

    if(event->button() ==Qt::RightButton){
        qDebug() << "Right Clicked the Cell!";
        //emit CellSelected(this);
        emit RightClicked(this->x_, this->y_ );
    }

    if(event->button()==Qt::LeftButton){
        qDebug() << "Left Clicked the Cell!";
        //emit CellSelected(this);
        emit LeftClicked(this->x_, this->y_);
    }

}

