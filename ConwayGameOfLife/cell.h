#ifndef CELL_H
#define CELL_H


#include <QColor>
#include <QGraphicsItem>

class Cell: public QObject, public QGraphicsItem {
    Q_OBJECT //allows us to emit signals

public:
    Cell(QColor color, const int x, const int y); //constructor

    //inline member functions
    int get_x() const { return x_;}
    int get_y() const { return y_;}

    //Status of Cell...alive or dead
    bool is_alive() {return is_alive_;}

    //Sets state of Cell
    void set_alive(bool alive) {is_alive_ = alive;}

    void setColor(QColor color) {color_ = color;}

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    //static int get_width() {return width_;}

signals:
    void CellSelected(Cell *c);
    void RightClicked(int x, int y);
    void LeftClicked(int x, int y);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int x_;
    int y_;
    QColor color_;
    static const int width_ = 20; //cell width and height
    bool is_alive_; //Alive or Dead
    friend bool operator==(const Cell &first, const Cell &other);

};


#endif // CELL_H
