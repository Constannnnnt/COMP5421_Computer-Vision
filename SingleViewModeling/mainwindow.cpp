#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->installEventFilter( this );

    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(ui->label);
    setCentralWidget(scrollArea);

    img_scale = 1.0;
    img_scale_min = 1.0;

    getVanish_mode_x = false;
    getVanish_mode_y = false;
    getVanish_mode_z = false;

}

MainWindow::~MainWindow()
{
    delete ui;
}

// on_action functions
void MainWindow::on_actionOpen_Image_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("Open Image"), ".", tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
    /*
    bool is_gif = fileName.contains("gif");

    if (is_gif) {
        Magick::InitializeMagick("");
        Magick::Image mimage(fileName.toStdString());
        int w = mimage.columns();
        int h = mimage.rows();

        // Make OpenCV Mat of same size with 8-bit and 3 channels
        image = cv::Mat::zeros(h, w, CV_8UC3);
        // Unpack Magick++ pixels into OpenCV Mat structure
        mimage.write(0, 0, w, h,"BGR",Magick::CharPixel,image.data);

    } else {
        image = cv::imread(fileName.toLatin1().data());
    }
    */
    image = cv::imread(fileName.toLatin1().data());

    // convert cv::Mat to QImage
    cv::cvtColor(image, image, CV_BGR2RGB);

    int width_im = image.cols;
    int height_im = image.rows;
    double width_m = ui->label->width();
    double height_m = ui->label->height();

    if (width_im < width_m || height_im < height_m) {
        img_scale = (width_m / width_im) > (height_m / height_im) ? (width_m / width_im) : (height_m / height_im);
        img_scale_min = img_scale;
    }

    contour_image = image.clone();

    QImage Q_img = QImage((const unsigned char*)(image.data), image.cols, image.rows, image.step, QImage::Format_RGB888);

    ui->label->setPixmap(QPixmap::fromImage(Q_img).scaled(QPixmap::fromImage(Q_img).width()*img_scale,
                                                          QPixmap::fromImage(Q_img).height()*img_scale, Qt::KeepAspectRatio));
}

void MainWindow::display_image(){
    cv::Mat myImage;
    if(getVanish_mode_x || getVanish_mode_y || getVanish_mode_z)
        myImage = contour_image;
    else
        myImage = image;

    QImage Q_img = QImage((const unsigned char*)(myImage.data), myImage.cols, myImage.rows, myImage.step, QImage::Format_RGB888);
    QPixmap p = QPixmap::fromImage(Q_img);
    ui->label->setPixmap( p.scaled(p.width()*img_scale, p.height()*img_scale, Qt::KeepAspectRatio) );
}

void MainWindow::on_actionZoom_in_triggered(){
    img_scale += 0.1;
    display_image();
}
void MainWindow::on_actionZoom_out_triggered(){
    img_scale -= 0.1;
    display_image();
}

void MainWindow::on_actionReset_all_triggered(){
    this->resetAll();
}

void MainWindow::resetAll(){
    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    getVanish_mode_x = getVanish_mode_y = getVanish_mode_z = false;
}

void MainWindow::on_actionGet_vanish_x_triggered(bool checked){
    getVanish_mode_x = checked;

    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    getVanish_mode_y = getVanish_mode_z = false;
}
void MainWindow::on_actionGet_vanish_y_triggered(bool checked){
    getVanish_mode_y = checked;

    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    getVanish_mode_x = getVanish_mode_z = false;
}
void MainWindow::on_actionGet_vanish_z_triggered(bool checked){
    getVanish_mode_z = checked;

    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_y->setChecked(false);
    getVanish_mode_x = getVanish_mode_y = false;
}

void MainWindow::on_actionDraw_vanish_triggered(){
    cout << "size of vanish x: " << vanish_x.size() << endl;
    cout << "size of vanish y: " << vanish_y.size() << endl;
    cout << "size of vanish z: " << vanish_z.size() << endl;

    if( vanish_x.size()%2 != 0 || vanish_y.size()%2 != 0 || vanish_z.size()%2 != 0 ){
        cout << "size of vanish_ is not even! " << endl;
        return;
    }

    if(vanish_x.size()>0)
        for(uint i=0; i<vanish_x.size()/4; i++){
            cv::line(contour_image, cv::Point(vanish_x[4*i], vanish_x[4*i+1]),
                                    cv::Point(vanish_x[4*i+2], vanish_x[4*i+3]), CV_RGB(255,0,0), 1); // blue
        }
    if(vanish_y.size()>0)
        for(uint i=0; i<vanish_x.size()/4; i++){
            cv::line(contour_image, cv::Point(vanish_y[4*i], vanish_y[4*i+1]),
                                    cv::Point(vanish_y[4*i+2], vanish_y[4*i+3]), CV_RGB(0,255,0), 1); // green
        }
    if(vanish_z.size()>0)
        for(uint i=0; i<vanish_x.size()/4; i++){
            cv::line(contour_image, cv::Point(vanish_z[4*i], vanish_z[4*i+1]),
                                    cv::Point(vanish_z[4*i+2], vanish_z[4*i+3]), CV_RGB(0,0,255), 1); // red
        }

    display_image();

    calVanishingPt();
}

// event filter
bool MainWindow::eventFilter(QObject *watched, QEvent *event) {

    // getVanish_mode + left click:
    if( (event->type() == QEvent::MouseButtonPress) &&
        (strcmp(watched->metaObject()->className(), "MainWindow")) == 0 )
    {
        QMouseEvent* me = static_cast<QMouseEvent*> (event);
        QPoint p = ui->label->mapFrom(this, me->pos());
        p /= img_scale;

        cout << p.x() << "\t" << p.y() << endl;

        if(getVanish_mode_x){
            vanish_x.push_back( p.x() );
            vanish_x.push_back( p.y() );
        }
        else if(getVanish_mode_y){
            vanish_y.push_back( p.x() );
            vanish_y.push_back( p.y() );
        }
        else if(getVanish_mode_z){
            vanish_z.push_back( p.x() );
            vanish_z.push_back( p.y() );
        }
        else
            return false;

        // draw lines
        //dots->push_back( QPoint(head_node->getCol(), head_node->getRow()) );
        //cv::circle(contour_image, cv::Point(head_node->getCol(),head_node->getRow()), 1, CV_RGB(0,0,255), 2);

        display_image();
    }

    return false;
}


// Actual functions start here
// step 1: Calculate vanishing points, Bob Collin's method
void MainWindow::calVanishingPt(){
    cv::Mat M, EigenVector, EigenValue;
    cv::Mat temp_M;
    cv::Point3d point1, point2;
    cv::Point3d line;

    // M = (cv::Mat_<float>(3,3) << 1,2,3, 4,5,6, 7,8,9.1);
    // temp_M = (cv::Mat_<float>(3,3) << 1,0,0, 0,0,0, 0,0,0);
    // cout << "temp_M is:  " << temp_M << endl;

    // get vanishing point x
    M = cv::Mat_<float>(3,3);
    for(uint i=0; i<vanish_x.size()/4; i++){
        point1 = cv::Point3f(vanish_x[4*i], vanish_x[4*i+1], 1);
        point2 = cv::Point3f(vanish_x[4*i+2], vanish_x[4*i+3], 1);
        line = point1.cross(point2);
        //line = cv::Point3f(line.x/line.z, line.y/line.z, 1);

        temp_M = (cv::Mat_<float>(3,3) << line.x*line.x, line.x*line.y, line.x*line.z,
                                          line.x*line.y, line.y*line.y, line.y*line.z,
                                          line.x*line.z, line.y*line.z, line.z*line.z);
        M = M + temp_M;
    }

    cv::eigen(M, EigenValue, EigenVector, DBL_EPSILON);
    cout << "eigen values " << EigenValue << endl;
    cout << "eigen vectors " << EigenVector << endl;

    vanishPt_x = cv::Point3f(EigenVector.at<float>(2,0)/EigenVector.at<float>(2,2),
                             EigenVector.at<float>(2,1)/EigenVector.at<float>(2,2), 1);
    cout << "Vanishing Point x: " << vanishPt_x << endl;


    // get vanishing point y
    M = cv::Mat_<float>(3,3);
    for(uint i=0; i<vanish_y.size()/4; i++){
        point1 = cv::Point3f(vanish_y[4*i], vanish_y[4*i+1], 1);
        point2 = cv::Point3f(vanish_y[4*i+2], vanish_y[4*i+3], 1);
        line = point1.cross(point2);
        //line = cv::Point3f(line.x/line.z, line.y/line.z, 1);

        temp_M = (cv::Mat_<float>(3,3) << line.x*line.x, line.x*line.y, line.x*line.z,
                                          line.x*line.y, line.y*line.y, line.y*line.z,
                                          line.x*line.z, line.y*line.z, line.z*line.z);
        M = M + temp_M;
    }

    cv::eigen(M, EigenValue, EigenVector, DBL_EPSILON);
    cout << "eigen values " << EigenValue << endl;
    cout << "eigen vectors " << EigenVector << endl;

    vanishPt_y = cv::Point3f(EigenVector.at<float>(2,0)/EigenVector.at<float>(2,2),
                             EigenVector.at<float>(2,1)/EigenVector.at<float>(2,2), 1);
    cout << "Vanishing Point y: " << vanishPt_y << endl;


    // get vanishing point z
    M = cv::Mat_<float>(3,3);
    for(uint i=0; i<vanish_z.size()/4; i++){
        point1 = cv::Point3f(vanish_z[4*i], vanish_z[4*i+1], 1);
        point2 = cv::Point3f(vanish_z[4*i+2], vanish_z[4*i+3], 1);
        line = point1.cross(point2);
        //line = cv::Point3f(line.x/line.z, line.y/line.z, 1);

        temp_M = (cv::Mat_<float>(3,3) << line.x*line.x, line.x*line.y, line.x*line.z,
                                          line.x*line.y, line.y*line.y, line.y*line.z,
                                          line.x*line.z, line.y*line.z, line.z*line.z);
        M = M + temp_M;
    }

    cv::eigen(M, EigenValue, EigenVector, DBL_EPSILON);
    cout << "eigen values " << EigenValue << endl;
    cout << "eigen vectors " << EigenVector << endl;

    vanishPt_z = cv::Point3f(EigenVector.at<float>(2,0)/EigenVector.at<float>(2,2),
                             EigenVector.at<float>(2,1)/EigenVector.at<float>(2,2), 1);
    cout << "Vanishing Point z: " << vanishPt_z << endl;

}


// step 2: Choose Reference Points





// step 3: Compute 3D Positions





// step 4: Compute Texture Maps




















