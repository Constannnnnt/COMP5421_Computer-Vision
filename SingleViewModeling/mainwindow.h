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
#include <QImageWriter>

#include <QString>
#include <QFileDialog>
#include <QMessageBox>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <Magick++.h>
#include <ctime>
#include "math.h"

using namespace cv;
using namespace std;
//using namespace Magick;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    bool eventFilter(QObject *watched, QEvent *event);

    ~MainWindow();

private slots:
    void on_actionOpen_Image_triggered();

    void on_actionZoom_in_triggered();

    void on_actionZoom_out_triggered();

    void on_actionReset_all_triggered();

    void on_actionGet_vanish_x_triggered(bool checked);

    void on_actionGet_vanish_y_triggered(bool checked);

    void on_actionGet_vanish_z_triggered(bool checked);

    void on_actionDraw_vanish_triggered();

    void on_actionGet_origin_triggered(bool checked);

private:
    // private data member
    Ui::MainWindow *ui;

    Mat image;
    Mat contour_image;
    Mat ProjMatrix;

    QScrollArea* scrollArea;
    double img_scale;
    double img_scale_min;

    bool getVanish_mode_x;
    bool getVanish_mode_y;
    bool getVanish_mode_z;
    bool getVanish_mode_o;
    double scale_x;
    double scale_y;
    double scale_z;
    vector<int> vanish_x;
    vector<int> vanish_y;
    vector<int> vanish_z;
    cv::Point3f vanishPt_x;
    cv::Point3f vanishPt_y;
    cv::Point3f vanishPt_z;
    cv::Point3f Origin;
    cv::Point2f refx;
    cv::Point2f refy;
    cv::Point2f refz;

    // 2D to 3D pespective transform matrix
    // Mat Hxy, Hxz, Hyz;

    // private helper functions
    void display_image();
    void resetAll();

    // private functions
    void calVanishingPt();
    void calProjectionMatrix();
    void getTextureMap();
    //Point3f cal3DPosition(Point3f);
    void cal3DPosition();

};

#endif // MAINWINDOW_H












