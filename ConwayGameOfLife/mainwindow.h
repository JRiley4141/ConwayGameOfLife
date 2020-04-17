//
//Name: Jennifer Riley
//
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <vector>
#include <QTimer>
#include <QSlider>


#include "cell.h"



namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void StartGame(QGraphicsView *view);
    
    int NeighborLogic(int x, int y);

    bool NeighborAlive(int x, int y);

    std::pair<int, int> Normalized(int x, int y);
    
    void Turn();

    ~MainWindow();

private slots:

    void stepbutton_clicked();
    void playbutton_clicked();
    void pausebutton_clicked();

    //intervals btwn generations
    void slider_moved(int msec);

    void CellSelectedSlot(Cell *c);
    void RightClickedSlot(int x_, int y_);
    void LeftClickedSlot(int x_, int y_);

    void PlayThru();

private:

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsScene *sceneGraph;

    static int turns_;
    static int num_alive_;
    static int percent_alive_;

    static int slider_speed;
    QTimer *timer;

    //matrix width & height
    static const int cols = 20;
    static const int rows = 50;
    Cell* matrix[cols][rows];

    //Graph
    static int number_of_bars;
    std::vector<QGraphicsRectItem*> bar_graph;
    void DrawGraph();
    void DeleteGraph();


};
#endif // MAINWINDOW_H
