//Name: Jennifer Riley

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QDebug>
#include <cmath>
#include <QTime>


//Initialize variables
int MainWindow::num_alive_ = 0;
int MainWindow::percent_alive_ = 0;
int MainWindow::turns_ = 0;
int MainWindow::slider_speed = 0;

QColor color_dead(76, 0, 153); //Gray for dead
QColor color_alive(27,165,91); //Green for alive

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    qDebug() << "MainWindow Constrcutor";

    timer = new QTimer;
    //setup the UI before the scene is drawn
    ui->setupUi(this);

    //QGraphicsView is the UI element that contains the scene that we willa ctually get to draw on
    QGraphicsView * view = ui->Board_graphicsView;
    QGraphicsView *graphView = ui->Graph_graphicsView;

    scene = new QGraphicsScene;
    sceneGraph = new QGraphicsScene;

    view->setScene(scene);
    graphView->setScene(sceneGraph);

    //makes the scene the same size as the view containing it
    view->setSceneRect(0,0,view->frameSize().width(), view->frameSize().height());
    graphView->setSceneRect(0,0,view->frameSize().width(), view->frameSize().height());

    QColor color(255,255,255);
    int row = 0;
    int col = 0;

    for (int j = 20; j < 220; j+= 10) {
        for (int i = 50; i < 550; i+= 10) {
            //Creates a new cell
            Cell *c = new Cell(color, i, j);
            scene->addItem(c);

            //Connect Signals to Slots
            connect(c, &Cell::LeftClicked, this, &MainWindow::LeftClickedSlot);
            connect(c, &Cell::RightClicked, this, &MainWindow::RightClickedSlot);
            matrix[row][col] = c;
            col++;
        }
        row++;
        col = 0;
    }

    //Connect Signals to Slots
    connect(ui->stepButton, &QAbstractButton::clicked, this, &MainWindow::stepbutton_clicked);
    connect(ui->playButton, &QAbstractButton::clicked, this, &MainWindow::playbutton_clicked);
    connect(ui->pauseButton, &QAbstractButton::clicked, this, &MainWindow::pausebutton_clicked);
    connect(ui->speedSlider, &QAbstractSlider::valueChanged, this, &MainWindow::slider_moved);
    connect(timer, SIGNAL(timeout()), this, SLOT(PlayThru()));


    StartGame(view);
}

void MainWindow::StartGame(QGraphicsView *view) {
    //Sets up the Initial Board with the cells being randomized for alive vs dead

    QTransform t;
    num_alive_ = 0;
    turns_ = 0;
    //seed random number generator
    srand(time(0));

    for (int j = 20; j < 220; j += 10) {
        for (int i = 50; i < 550; i += 10) {

            QPointF p(i, j);
            QGraphicsItem *current = scene->itemAt(p,t);
            //precondition
            assert(current != nullptr);
            Cell *current_ = dynamic_cast<Cell *>(current);

            int prob_alive = rand() % 2;
            current_->set_alive(prob_alive);
            if (current_->is_alive()) {
                current_->setColor(color_alive);
                num_alive_ ++;
                //qDebug() << "Number alive = " << num_alive_;
            }
            else {
                current_->setColor(color_dead);

            }
        }
    }
//  ******** POTENTIAL FUNCTION TO CLEAN UP CODE ***********
    //Print Population to PopLabel
    //qDebug() << "Number alive = " << num_alive_;
    percent_alive_ = int((num_alive_*100)/1000);
    //qDebug() << percent_alive_;
    std::string s = "Population: " + std::to_string(num_alive_) + " (" + std::to_string(percent_alive_) + "%)";
    QString qs(s.c_str());
    ui->popLabel->setText(qs);

    //Print  Turn to TurnLabel
    //qDebug() << "Number of Turns: " << turns_;
    std::string s2 = "Turns: " + std::to_string(turns_);
    QString qs2(s2.c_str());
    ui->turnLabel->setText(qs2);

    update();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CellSelectedSlot(Cell *c) {
    qDebug() << c->get_x() << ":" << c->get_y();
}

void MainWindow::LeftClickedSlot(int x, int y) {
    QTransform t;
    QPointF p(x, y);
    QGraphicsView *view = ui->Board_graphicsView;
    QGraphicsItem *current = view->scene()->itemAt(p,t);
    Cell *current_ = dynamic_cast<Cell *>(current);

    if (!current_->is_alive()) {
        current_->set_alive(true);

        current_->setColor(color_alive);
        num_alive_ ++;

//  ******** POTENTIAL FUNCTION TO CLEAN UP CODE ***********
        percent_alive_ = int((num_alive_*100)/1000);
        //qDebug() << percent_alive_;
        std::string s = "Population: " + std::to_string(num_alive_) + " (" + std::to_string(percent_alive_) + "%)";
        QString qs(s.c_str());
        ui->popLabel->setText(qs);
        //qDebug() << num_alive_;
        view->scene()->update();
        //update();
        repaint();
    }
}

void MainWindow::RightClickedSlot(int x, int y) {
    QTransform t;
    QPointF p(x, y);
    QGraphicsView *view = ui->Board_graphicsView;
    QGraphicsItem *current = view->scene()->itemAt(p,t);
    Cell *current_ = dynamic_cast<Cell *>(current);

    if (current_->is_alive()) {
        current_->set_alive(false);
        current_->setColor(color_dead);
        num_alive_ --;

//  ******** POTENTIAL FUNCTION TO CLEAN UP CODE ***********
        percent_alive_ = int((num_alive_*100)/1000);
        //qDebug() << num_alive_;
        std::string s = "Population: " + std::to_string(num_alive_) + " (" + std::to_string(percent_alive_) + "%)";
        QString qs(s.c_str());
        ui->popLabel->setText(qs);

        view->scene()->update();
        //update();
        repaint();
    }
}

void MainWindow::PlayThru() {
    Turn();
}

void MainWindow::stepbutton_clicked() {
    qDebug() << "Step Button Clicked";
    Turn();
}

void MainWindow::playbutton_clicked() {
    //Game auto plays based on time setting
    qDebug() << "Play Button Clicked";
    timer->start();
}

void MainWindow::pausebutton_clicked() {
    qDebug() << "Pause Button Clicked";
    timer->stop();
}

void MainWindow::slider_moved(int msec) {
    //qDebug() << "Speed Slider Moved";
    slider_speed = 0; //1000msec = 1 Second
    slider_speed = int(100-msec);
    timer->setInterval(slider_speed);

    std::string s3 = "Speed: " + std::to_string(slider_speed);
    QString qs3(s3.c_str());
    ui->speed->setText(qs3);
}


/* A single Turn that compares the cells to it's neighbors
  Updates the cells from alive/dead based on neighbor cells
  Increases the Turns counter and updates the Graph */

void MainWindow::Turn() {
    //qDebug() << "Turn Called";

    num_alive_ = 0;
    
    //need a temp 2d matrix to store cell status (alive/dead)
    bool temp_matrix[cols][rows];

    for (int x = 0; x < cols; x++) {
        for (int y = 0; y < rows; y++) {
            temp_matrix[x][y] = matrix[x][y]->is_alive();
        }
        //qDebug() <<"Turn: temp_matrix created";
    }
    
    //4 RULES OF THE GAME
    for (int x = 0; x < cols; x++) {
        for (int y = 0; y < rows; y++) {
            //qDebug() << "NeighborLogic Called";
            int neighbors = NeighborLogic(x, y);
            //qDebug() << "Turn: 4rules of game called. << Neighbor for (" << x << "," << y << "Neighbors alived = " << neighbors;

            //any cell with fewer than 2 live neighbors dies
            if (neighbors < 2 && matrix[x][y]->is_alive()) {
                temp_matrix[x][y] = false;
                //qDebug() << "Turn: Rule1 ";
            }
            
            //any live cell with 2 or 3 neighbors remains alive
            else if ((neighbors == 2 || neighbors == 3) && matrix[x][y]->is_alive()) {
                temp_matrix[x][y] = true;
                //qDebug() << "Turn: Rule2 ";
            }
            
            //any live cell with more than 3 neighbors dies
            else if (neighbors > 3 && matrix[x][y]->is_alive()) {
                temp_matrix[x][y] = false;
                //qDebug() << "Turn: Rule3 ";
            }
            
            //Any dead cell with exactly 3 live neighbors becomes a live cell
            else if (neighbors == 3 && !(matrix[x][y]->is_alive()) ) {
                temp_matrix[x][y] = true;
                //qDebug() << "Turn: Rule4 ";
            }
        }
    }
    
    for (int x = 0; x < cols; x++) {
        for (int y = 0; y < rows; y++) {
            if (temp_matrix[x][y] == true) {
                num_alive_ ++;
                matrix[x][y]->set_alive(true);
                matrix[x][y]->setColor(color_alive);
               // qDebug() << "Turn: Updated Matrix[][] with alive ";
            }
            else {
                matrix[x][y]->set_alive(false);
                matrix[x][y]->setColor(color_dead);
                //qDebug() << "Turn: Updated Matrix[][] with dead ";
            }
        }
    }
    
    //Update Board
    turns_ ++;
    QGraphicsView *view = ui->Board_graphicsView;

//  ******** POTENTIAL FUNCTION TO CLEAN UP CODE ***********
    percent_alive_ = int((num_alive_*100)/1000);

    std::string s = "Population: " + std::to_string(num_alive_) + " (" + std::to_string(percent_alive_) + "%)";
    QString qs(s.c_str());
    ui->popLabel->setText(qs);
    
    std::string s2 = "Turns: " + std::to_string(turns_);
    QString qs2(s2.c_str());
    ui->turnLabel->setText(qs2);
    

    //BarGraph
    if (bar_graph.size() == 50) {
        qDebug() <<"Graph Vector is full";
        //copying n+1's height to n, which will change how to renders
        for (size_t i = 0; i < 49; i++) {
            QRectF r = bar_graph.at(i+1)->rect();
            qreal y = r.y();
            qreal h = r.height();
            QRectF r2 = bar_graph.at(i)->rect();
            bar_graph.at(i)->setRect(QRectF(r2.x(), y, r2.width(), h));
        }
        QRectF r = bar_graph.back()->rect();
        qDebug() << "0: QRectF" << r;
        r.setY(100 - percent_alive_);
        r.setHeight(percent_alive_);
        bar_graph.back()->setRect(r);
    }
    else {
        if (bar_graph.empty()) {
            auto bar = new QGraphicsRectItem(0, 100 - percent_alive_, 10, percent_alive_);
            bar->setBrush(QBrush(QColor(178,255,102)));
            bar_graph.push_back(bar);
            qDebug() << "1: bargraph" << bar;
            sceneGraph->addItem(bar);
        }
        else {
            auto last_bar = bar_graph.back();
            qDebug() << "2: bargraph" << last_bar;
            auto bar = new QGraphicsRectItem(last_bar->rect().x() + 10, 100 - percent_alive_, 10, percent_alive_);
            bar->setBrush(QBrush(QColor(178,255,102)));
            bar_graph.push_back(bar);

            sceneGraph->addItem(bar);
        }
    }



    view->scene()->update();
    update();
    repaint();
    qDebug() << "Turn: Updated Board ";
}

//This passes in the 8 neighbors of the cell into the NeighborAliveFunctions
int MainWindow::NeighborLogic(int x, int y) {

    int neighbors_alive = 0;
    //qDebug() << "From NeighborLogic, NeighborAlived called";
    if (NeighborAlive(x, y - 1)) {neighbors_alive++;} //UP
    if (NeighborAlive(x, y + 1)) {neighbors_alive++;} //DOWN
    if (NeighborAlive(x - 1, y)) {neighbors_alive++;} //LEFT
    if (NeighborAlive(x + 1, y)) {neighbors_alive++;} //RIGHT
    if (NeighborAlive(x - 1, y - 1)) {neighbors_alive++;} //UP-LEFT diagonal
    if (NeighborAlive(x + 1, y - 1)) {neighbors_alive++;} //UP-RIGHT diagonal
    if (NeighborAlive(x - 1, y + 1)) {neighbors_alive++;} //DOWN-LEFT diagonal
    if (NeighborAlive(x + 1, y + 1)) {neighbors_alive++;} //DOWN-RIGHT diagonal

    //qDebug() << "NeighborLogic Called neighbors_alive = " << neighbors_alive;
    return neighbors_alive;
}

//This takes care of any wrapping issues for the edge cases
std::pair<int, int> MainWindow::Normalized(int x, int y) {
    //qDebug() << "Normalized Called";
    int x1 = x;
    int y1 = y;

    if (x < 0 ) {
        x1 += cols;
    }
    if (x >= cols) {
        x1 -= cols;
    }

    if (y < 0) {
        y1 += rows;
    }

    if (y >= rows) {
        y1 -= rows;
    }
    //qDebug() << "Normalized Pair (" << x1 <<", " << y1 << ")";
    return std::make_pair(x1, y1);
}

bool MainWindow::NeighborAlive(int x, int y) {
    //qDebug() << "NeighborAlive Called";
    auto [x_, y_] = Normalized(x, y);

    return matrix[x_][y_]->is_alive();
}

void MainWindow::DrawGraph() {
    ui->setupUi(this);

    //QGraphicsView is the UI element that contains the scene that we willa ctually get to draw on
    QGraphicsView * view = ui->Graph_graphicsView;

    scene = new QGraphicsScene;

    view->setScene(scene);

    //makes the scene the same size as the view containing it
    view->setSceneRect(0,0,view->frameSize().width(), view->frameSize().height());

}
