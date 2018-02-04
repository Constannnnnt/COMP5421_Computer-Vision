#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(ui->label);
    setCentralWidget(scrollArea);

    //ui->scrollArea->setWidgetResizable(true);
    //ui->scrollArea->setWidget(ui->label);
    //setCentralWidget(ui->scrollArea);

    img_scale = 1.0;

    ui->actionDisplay_Contour->setChecked(true);
    contour_enabled = true;
    scissor_enabled = false;

}


MainWindow::~MainWindow()
{
    delete ui;
    delete scrollArea;
}


// helper function: display the image
void MainWindow::display_image(Mat im)
{
    cv::Mat img_tmp = im.clone();

    cv::cvtColor(img_tmp, img_tmp, CV_BGR2RGB);
    QImage Q_img = QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);

    QPixmap p = QPixmap::fromImage(Q_img);
    ui->label->setPixmap( p.scaled(p.width()*img_scale, p.height()*img_scale, Qt::KeepAspectRatio) );

    scrollArea->setWidget(ui->label);
    setCentralWidget(scrollArea);
}


// open a image
void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("Open Image"), ".", tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
    image = cv::imread(fileName.toLatin1().data());

    // convert cv::Mat to QImage
    cv::cvtColor(image, image, CV_BGR2RGB);
    QImage Q_img = QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);

    ui->label->setPixmap(QPixmap::fromImage(Q_img));
    // ui->label->resize(ui->label->pixmap()->size());

}


// Save image
void MainWindow::on_actionSave_Contour_triggered()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Save Contour", "Save image with contour marked?",
                                                              QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes){
        cv::imwrite("/home/jguoaj/Desktop/contour_image.jpg", contour_image);
        // cv::imwrite("/home/jguoaj/contour_image.jpg", image);
    }
}


void MainWindow::on_actionSave_Mask_triggered()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Save Mask", "Save compositing mask for PhotoShop?",
                                                              QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes)
        cv::imwrite("/home/jguoaj/Desktop/mask_image.jpg", mask_image);

}


// close the program
void MainWindow::on_actionExit_triggered()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Exit", "Are you sure to close this program?",
                                                              QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes)
        QApplication::quit();
}


// zoom in and out
void MainWindow::on_actionZoom_In_triggered()
{
    img_scale += 0.1;
    if(contour_enabled == true)
        display_image(contour_image);
    else
        display_image(image);
}


void MainWindow::on_actionZoom_Out_triggered()
{
    img_scale -= 0.1;
    if(contour_enabled == true)
        display_image(contour_image);
    else
        display_image(image);
}


// display help message
void MainWindow::on_actionHelp_triggered()
{
    QString text = QString("Nothing for help !");
    QMessageBox::about(this, "Help", text);
}


// Work Mode
void MainWindow::on_actionScissor_triggered(bool checked)
{
    scissor_enabled = checked;


}


void MainWindow::on_actionDisplay_Contour_triggered(bool checked)
{
    this->contour_enabled = checked;
    this->display_image(contour_image);
}


void MainWindow::on_actionReset_Contour_triggered()
{
    cout << "reset contour" << endl;
    contour = cv::Mat::zeros(contour.size(), CV_8UC3);

}


void MainWindow::on_actionFinish_Contour_triggered()
{
    // close the loop
    cv::line(contour, cv::Point(x_list[0],y_list[0]), cv::Point(x_list[list_size-1], y_list[list_size-1]), Scalar(255,0,0));
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





