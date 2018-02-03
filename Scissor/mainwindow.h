#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <fstream>
#include <queue>
//#include <vector>

#include <QMainWindow>
#include <QImage>
#include <QPoint>
#include <QMouseEvent>

#include <QString>
#include <QFileDialog>
#include <QMessageBox>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
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

    void on_actionPixel_Node_triggered();

    void on_actionCost_Graph_triggered();

    void on_actionPath_Tree_triggered();

    void on_actionMin_Path_triggered();

    void on_actionFinish_Contour_triggered();

private:
    Ui::MainWindow *ui;

    Mat image;
    Mat contour_image;
    Mat mask_image;

    // int* xclick;
    // int* yclick;
    // double scale;


};

#endif // MAINWINDOW_H
