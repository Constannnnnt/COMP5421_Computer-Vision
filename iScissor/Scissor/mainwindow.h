#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <fstream>
#include <queue>
#include <math.h>
#include <vector>

#include <QMainWindow>
#include <QScrollArea>
#include <QImage>
#include <QPoint>
#include <QMouseEvent>

#include <QString>
#include <QFileDialog>
#include <QMessageBox>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "pixelnode.h"
#include "math.h"
#include "workstates.h"

using namespace cv;
using namespace  std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    int workstates;

    void addImage();

    void keyPressEvent(QKeyEvent *event);

    void keyReleaseEvent(QKeyEvent *event);

    bool eventFilter(QObject *watched, QEvent *event);

    void Dijstras(pixelNode* p);

    void costgraph_init();

    void updatePathTree();

    // helper function
    void print_node(pixelNode* n);

    void draw_contour(int x, int y);

    void computeCostFunc();

    void channelTransform(QRgb rgb, int color[3]);

    double getDLink(int, int, int);

    void getPath(int, int, vector<QPoint>&);

    QImage drawPathTree();

    void resetAll();

    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionSave_Contour_triggered();

    void on_actionSave_Mask_triggered();

    void on_actionExit_triggered();

    void on_actionZoom_In_triggered();

    void on_actionZoom_Out_triggered();

    void on_actionHelp_triggered();


    void on_actionScissor_triggered(bool checked);

    void on_actionDisplay_Contour_triggered(bool checked);

    void on_actionFinish_Contour_triggered();

    void on_actionReset_Contour_triggered();


    void on_actionPixel_Node_triggered(bool checked);

    void on_actionCost_Graph_triggered(bool checked);

    void on_actionPath_Tree_triggered(bool checked);

    void on_actionMin_Path_triggered(bool checked);


    void on_actionGuassian_3_triggered(bool checked);

    void on_actionGaussian_5_triggered(bool checked);

private:
    Ui::MainWindow *ui;

    Mat image;
    Mat contour;
    Mat contour_image;
    Mat mask_image;
    Mat current_image;
    Mat previous_image;
    QImage* Qimg;
    QImage* pathTree;
    QImage* Mask;
    int path_id;
    vector<QPoint>* dots;
    vector<vector<QPoint>>* paths;
    QPainter* painter;

    // Dij algorithm
    Mat* costgraph_weight;
    Mat visitedMap;
    //Mat activeMap;
    Mat parentMap;
    Mat graphCost;

    pixelNode* head_node;   // the head of node list, always the first seed
    pixelNode* current_node;   // store the address of current_node, for setting parent during click
    vector<vector<pixelNode*> > pixelnodes;

    // click list
    int* x_list;
    int* y_list;
    int list_size;

    bool scissor_enabled;
    bool contour_enabled;
    bool ctrl_enabled;
    bool first_seed_flag;

    double img_scale;
    int idx; // mark the length of the nodes

    void display_image(cv::Mat im);
    QScrollArea* scrollArea;

};


// compare function for priority queue
struct compareQueue{
    bool operator() ( pixelNode* a, pixelNode* b ){
        return (a->getTotalCost() >= b->getTotalCost());
    }
};


#endif // MAINWINDOW_H



