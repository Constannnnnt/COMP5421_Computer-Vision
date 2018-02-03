#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFileDialog>
#include <QMessageBox>

#include <QImage>
#include <QPoint>
#include <QMouseEvent>


#include <opencv2/opencv.hpp>


#include <iostream>
#include <fstream>

#include <queue>          // std::priority_queue
#include <vector>         // std::vector
#include <functional>     // std::greater

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    QImage cvMat2QImage(cv::Mat& mat);

    //void mousePressEvent( QMouseEvent* ev );
    //void mouseMoveEvent(QMouseEvent *ev);

    bool eventFilter(QObject *obj, QEvent *event);
    
    ~MainWindow();
    
private slots:
    void on_actionOpen_triggered();

    void on_actionWrite_Path_triggered();

    void on_actionWrite_Mask_triggered();

    void on_actionScissor_triggered(bool checked);

    void on_actionReset_Contours_triggered();

    void on_actionQuit_triggered();

    void on_actionAuthors_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actionOverlay_Active_triggered(bool checked);

    void on_actionZoom_In_triggered();

    void on_actionZoom_Out_triggered();

    void on_actionHelp_triggered();

    void on_actionBlur_3x3_triggered();

    void on_actionBlur_5x5_triggered();

    void on_actionGaussian_Blur_triggered();

    void on_actionGaussian_Blur_5x5_triggered();

    void on_actionFinish_Contour_triggered();

private:
    Ui::MainWindow *ui;

    cv::Mat im;
    cv::Mat out;
    cvMat2QImage qim;
    double scaleFactor; //scale factor

    void paintImage(cv::Mat a);

    //to hold list of all the clicks
    int currentSize;
    int maxSize;
    int * allClicksX;
    int * allClicksY;


    //create float Mat
    //8-Mats to store weights of 8 neighbours
    //       top is Mat[0], top-right is Mat[1]
    cv::Mat * graphWeights;

    //Computes the weights for each node. note that each pixel is considered as a node of graph.
    //image is gray (1-channel).
    void computeGraphWeights();


    cv::Mat isVisited;
    cv::Mat dijstrasCost;
    //cv::Mat parentX;
    //cv::Mat parentY;
    cv::Mat previous;
    void doDijstras( int clickedX, int clickedY );
    void backTrackFrom(int startX, int startY);

    cv::Mat path;
    cv::Mat mask;
    void registerPath(int startX, int startY);

    void overlayPath( cv::Mat a, cv::Mat path, cv::Vec3b intensity , cv::Mat out );


    void mergeChannels(cv::Mat &tmp, cv::Mat &dst, float epsilon);


    //Tools->Activate Scissor. Checkable button. Run the algorithms only if this is true
    bool scissorActive;

    //Tools->overlayPathActive. If this is true display with path if any. If false show only the original image.
    bool overlayPathActive;


    void resetStateVariables();


};





class SpecialType
{
public:
    float num;
    int theX;
    int theY;

    SpecialType(float a, int x, int y )
    {
        num = a;
        theX = x;
        theY = y;
    }

};


class mycomparison
{
    bool reverse;
public:
    mycomparison(const bool& revparam=false)
    {reverse=revparam;}
    bool operator() (const SpecialType& lhs, const SpecialType&rhs) const
    {
        if (reverse) return (lhs.num<rhs.num);
        else return (lhs.num>rhs.num);
    }
};


#endif // MAINWINDOW_H
