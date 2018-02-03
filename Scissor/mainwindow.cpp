#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // resize(800, 600);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("Open Image"), ".", tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
    image = cv::imread(fileName.toLatin1().data());

    cv::cvtColor(image, image, CV_BGR2RGB);
    QImage img = QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);

    ui->label->setPixmap(QPixmap::fromImage(img));
    ui->label->resize(ui->label->pixmap()->size());
    //ui->label->show();

}

void MainWindow::on_actionSave_Contour_triggered()
{

}

void MainWindow::on_actionSave_Mask_triggered()
{

}

void MainWindow::on_actionExit_triggered()
{

}

void MainWindow::on_actionZoom_In_triggered()
{

}

void MainWindow::on_actionZoom_Out_triggered()
{

}

void MainWindow::on_actionHelp_triggered()
{

}


// Work Mode
void MainWindow::on_actionScissor_triggered(bool checked)
{

}

void MainWindow::on_actionDisplay_Contour_triggered(bool checked)
{

}

void MainWindow::on_actionFinish_Contour_triggered()
{

}

// Debug Mode
void MainWindow::on_actionPixel_Node_triggered()
{

}

void MainWindow::on_actionCost_Graph_triggered()
{

}

void MainWindow::on_actionPath_Tree_triggered()
{

}

void MainWindow::on_actionMin_Path_triggered()
{

}


