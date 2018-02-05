#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <fstream>
#include <queue>
//#include <vector>

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

using namespace cv;

//extern Mat image;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    void keyPressEvent(QKeyEvent *event);

    void keyReleaseEvent(QKeyEvent *event);

    bool eventFilter(QObject *watched, QEvent *event);

    // helper function
    void print_node(pixelNode* n);

    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionAdd_Image_triggered();

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


    void on_actionPixel_Node_triggered();

    void on_actionCost_Graph_triggered();

    void on_actionPath_Tree_triggered();

    void on_actionMin_Path_triggered();


private:
    Ui::MainWindow *ui;

    Mat image;
    Mat contour;
    Mat contour_image;
    Mat mask_image;

    pixelNode* head_node;   // the head of node list, always the first seed
    pixelNode* prev_node;   // store the address of prev_node, for setting parent during click

    // click list
    int* x_list;
    int* y_list;
    int list_size;

    bool scissor_enabled;
    bool contour_enabled;
    bool ctrl_enabled;
    int ctrl_count;

    double img_scale;
    int idx; // mark the length of the nodes

    void display_image(cv::Mat im);
    QScrollArea* scrollArea;

};


#endif // MAINWINDOW_H



