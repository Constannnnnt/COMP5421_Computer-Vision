#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <fstream>
#include <queue>
#include <math.h>
#include <vector>
#include <string>

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
    struct Face{
        int id;
        Mat texture;
        string textureFileName;
        vector<Point3f> coor2d;
        vector<Point3f> coor3d;

    };

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

    void on_actionset_reference_x_triggered(bool checked);

    void on_actionset_reference_y_triggered(bool checked);

    void on_actionset_reference_z_triggered(bool checked);

    void on_actionGet_origin_triggered(bool checked);

    void on_actionGet_3D_point_triggered(bool checked);

private:
    // private data member
    Ui::MainWindow *ui;

    Mat image;
    Mat contour_image;
    Mat ProjMatrix;

    QScrollArea* scrollArea;
    double img_scale;
    double img_scale_min;

    bool getVanish_mode_x, getVanish_mode_y, getVanish_mode_z, getVanish_mode_o, get3d_mode;
    double scale_x, scale_y, scale_z;
    vector<int> vanish_x, vanish_y, vanish_z;
    cv::Point3f vanishPt_x, vanishPt_y, vanishPt_z, Origin;
    cv::Point3f refx, refy, refz;

    bool setReference_x;
    bool setReference_y;
    bool setReference_z;
    bool refx_m, refy_m, refz_m, origin_m;
    int REF_LENGTH_X;
    int REF_LENGTH_Y;
    int REF_LENGTH_Z;

    vector<Face> faces;

    // private helper functions
    void display_image();
    void resetAll();

    // private functions
    void calVanishingPt();
    void calProjectionMatrix();
    void getTextureMap();
    void cal3DPosition();
    float getRefHeight(Point3f r, Point3f b);
    Point3f get3dCoor(Point3f r, Point3f b);
    Point3f get3dCoor(Point3f r, float h);
    void generateVRML(const string &);

};



#endif // MAINWINDOW_H












