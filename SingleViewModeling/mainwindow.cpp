#include "mainwindow.h"
#include "ui_mainwindow.h"

//#define REF_LENGTH_X 300
//#define REF_LENGTH_Y 400
//#define REF_LENGTH_Z 180

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
    getVanish_mode_o = false;
    setReference_x = false;
    setReference_y = false;
    setReference_z = false;
    refx_m = false;
    refy_m = false;
    refz_m = false;

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
    if(getVanish_mode_x || getVanish_mode_y || getVanish_mode_z || getVanish_mode_o || refx_m || refy_m || refz_m )
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
    ui->actionGet_origin->setChecked(false);
    getVanish_mode_x = getVanish_mode_y = getVanish_mode_z = getVanish_mode_o = false;
    setReference_x = false;
    setReference_y = false;
    setReference_z = false;
    refx_m = false;
    refy_m = false;
    refz_m = false;
}

void MainWindow::on_actionGet_vanish_x_triggered(bool checked){
    getVanish_mode_x = checked;

    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    ui->actionGet_origin->setChecked(false);
    ui->actionset_reference_x->setChecked(false);
    ui->actionset_reference_y->setChecked(false);
    ui->actionset_reference_z->setChecked(false);
    getVanish_mode_y = getVanish_mode_z = getVanish_mode_o = setReference_x = setReference_y = setReference_z = false;
}
void MainWindow::on_actionGet_vanish_y_triggered(bool checked){
    getVanish_mode_y = checked;

    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    ui->actionGet_origin->setChecked(false);
    ui->actionset_reference_x->setChecked(false);
    ui->actionset_reference_y->setChecked(false);
    ui->actionset_reference_z->setChecked(false);
    getVanish_mode_x = getVanish_mode_z = getVanish_mode_o = setReference_x = setReference_y = setReference_z = false;
}
void MainWindow::on_actionGet_vanish_z_triggered(bool checked){
    getVanish_mode_z = checked;

    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_origin->setChecked(false);
    ui->actionset_reference_x->setChecked(false);
    ui->actionset_reference_y->setChecked(false);
    ui->actionset_reference_z->setChecked(false);
    getVanish_mode_y = getVanish_mode_x = getVanish_mode_o = setReference_x = setReference_y = setReference_z = false;
}
void MainWindow::on_actionGet_origin_triggered(bool checked){
    getVanish_mode_o = checked;

    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    ui->actionset_reference_x->setChecked(false);
    ui->actionset_reference_y->setChecked(false);
    ui->actionset_reference_z->setChecked(false);
    getVanish_mode_y = getVanish_mode_z = getVanish_mode_x = setReference_x = setReference_y = setReference_z = false;
}

void MainWindow::on_actionset_reference_x_triggered(bool checked){
    setReference_x = checked;

    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    ui->actionGet_origin->setChecked(false);
    ui->actionset_reference_y->setChecked(false);
    ui->actionset_reference_z->setChecked(false);
    getVanish_mode_x = getVanish_mode_y = getVanish_mode_z = getVanish_mode_o = setReference_y = setReference_z = false;
}

void MainWindow::on_actionset_reference_y_triggered(bool checked){
    setReference_y = checked;

    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    ui->actionGet_origin->setChecked(false);
    ui->actionset_reference_x->setChecked(false);
    ui->actionset_reference_z->setChecked(false);
    getVanish_mode_x = getVanish_mode_y = getVanish_mode_z = getVanish_mode_o = setReference_x = setReference_z = false;
}

void MainWindow::on_actionset_reference_z_triggered(bool checked){
    setReference_z = checked;

    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    ui->actionGet_origin->setChecked(false);
    ui->actionset_reference_y->setChecked(false);
    ui->actionset_reference_x->setChecked(false);
    getVanish_mode_x = getVanish_mode_y = getVanish_mode_z = getVanish_mode_o = setReference_y = setReference_x = false;
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
                                    cv::Point(vanish_x[4*i+2], vanish_x[4*i+3]), CV_RGB(255,0,0), 4); // blue
        }
    if(vanish_y.size()>0)
        for(uint i=0; i<vanish_y.size()/4; i++){
            cv::line(contour_image, cv::Point(vanish_y[4*i], vanish_y[4*i+1]),
                                    cv::Point(vanish_y[4*i+2], vanish_y[4*i+3]), CV_RGB(0,255,0), 4); // green
        }
    if(vanish_z.size()>0)
        for(uint i=0; i<vanish_z.size()/4; i++){
            cv::line(contour_image, cv::Point(vanish_z[4*i], vanish_z[4*i+1]),
                                    cv::Point(vanish_z[4*i+2], vanish_z[4*i+3]), CV_RGB(0,0,255), 4); // red
        }

    display_image();

    calVanishingPt();
    calProjectionMatrix();
    getTextureMap();
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
            vanish_x.push_back( p.x());
            vanish_x.push_back( p.y());
        }
        else if(getVanish_mode_y){
            vanish_y.push_back( p.x());
            vanish_y.push_back( p.y());
        }
        else if(getVanish_mode_z){
            vanish_z.push_back( p.x());
            vanish_z.push_back( p.y());
        }
        else if(getVanish_mode_o){
            Origin = cv::Point3f(p.x(), p.y(), 1);
            cv::circle(contour_image, cv::Point(p.x(),p.y()), 1, CV_RGB(128,128,128), 5);
            origin_m = true;
        }
        else if (setReference_x) {
            refx = cv::Point3f(p.x() , p.y() , 1);
            cv::circle(contour_image, cv::Point(p.x(),p.y()), 1, CV_RGB(128,128,128), 5);
            refx_m = true;
        } else if (setReference_y) {
            refy = cv::Point3f(p.x() , p.y() , 1);
            cv::circle(contour_image, cv::Point(p.x(),p.y()), 1, CV_RGB(128,128,128), 5);
            refy_m = true;
        } else if (setReference_z) {
            refz = cv::Point3f(p.x() , p.y() , 1);
            cv::circle(contour_image, cv::Point(p.x(),p.y()), 1, CV_RGB(128,128,128), 5);
            refz_m = true;
        }
        else
            return false;

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

    // get vanishing point x
    M = (cv::Mat_<double>(3,3) << 0,0,0, 0,0,0, 0,0,0);
    for(uint i=0; i<vanish_x.size()/4; i++){
        point1 = cv::Point3f(vanish_x[4*i], vanish_x[4*i+1], 1);
        point2 = cv::Point3f(vanish_x[4*i+2], vanish_x[4*i+3], 1);
        line = point1.cross(point2);

        temp_M = (cv::Mat_<double>(3,3) << line.x*line.x, line.x*line.y, line.x*line.z,
                                          line.x*line.y, line.y*line.y, line.y*line.z,
                                          line.x*line.z, line.y*line.z, line.z*line.z);
        M = M + temp_M;
    }

    cv::eigen(M, EigenValue, EigenVector);
    cout << "eigen values x: " << EigenValue << endl;
    cout << "eigen vectors x: " << EigenVector << endl << endl;

    vanishPt_x = cv::Point3f(EigenVector.at<double>(2,0)/EigenVector.at<double>(2,2),
                             EigenVector.at<double>(2,1)/EigenVector.at<double>(2,2), 1);
    cout << "Vanishing Point x: " << vanishPt_x << endl;


    // get vanishing point y
    M = (cv::Mat_<double>(3,3) << 0,0,0, 0,0,0, 0,0,0);
    for(uint i=0; i<vanish_y.size()/4; i++){
        point1 = cv::Point3f(vanish_y[4*i], vanish_y[4*i+1], 1);
        point2 = cv::Point3f(vanish_y[4*i+2], vanish_y[4*i+3], 1);
        line = point1.cross(point2);

        temp_M = (cv::Mat_<double>(3,3) << line.x*line.x, line.x*line.y, line.x*line.z,
                                          line.x*line.y, line.y*line.y, line.y*line.z,
                                          line.x*line.z, line.y*line.z, line.z*line.z);
        M = M + temp_M;
    }

    cv::eigen(M, EigenValue, EigenVector);
    cout << "eigen values y: " << EigenValue << endl;
    cout << "eigen vectors y: " << EigenVector << endl << endl;

    vanishPt_y = cv::Point3f(EigenVector.at<double>(2,0)/EigenVector.at<double>(2,2),
                             EigenVector.at<double>(2,1)/EigenVector.at<double>(2,2), 1);
    cout << "Vanishing Point y: " << vanishPt_y << endl;


    // get vanishing point z
    M = (cv::Mat_<double>(3,3) << 0,0,0, 0,0,0, 0,0,0);
    for(uint i=0; i<vanish_z.size()/4; i++){
        point1 = cv::Point3f(vanish_z[4*i], vanish_z[4*i+1], 1);
        point2 = cv::Point3f(vanish_z[4*i+2], vanish_z[4*i+3], 1);
        line = point1.cross(point2);

        temp_M = (cv::Mat_<double>(3,3) << line.x*line.x, line.x*line.y, line.x*line.z,
                                          line.x*line.y, line.y*line.y, line.y*line.z,
                                          line.x*line.z, line.y*line.z, line.z*line.z);
        M = M + temp_M;
    }

    cv::eigen(M, EigenValue, EigenVector);
    cout << "eigen values z: " << EigenValue << endl;
    cout << "eigen vectors z: " << EigenVector << endl << endl;

    vanishPt_z = cv::Point3f(EigenVector.at<double>(2,0)/EigenVector.at<double>(2,2),
                             EigenVector.at<double>(2,1)/EigenVector.at<double>(2,2), 1);
    cout << "Vanishing Point z: " << vanishPt_z << endl << endl;

}


// step 2: Calculate Projection Matrix
void MainWindow::calProjectionMatrix(){
    // Choose the Origin First
    if (!origin_m) {
        cout << "not selected origin" << endl;
        return;
    }
    if (!refx_m) {
        cout << "not selected x-axis" << endl;
        return;
    }
    if (!refy_m) {
        cout << "not selected y-axis" << endl;
        return;
    }
    if (!refz_m) {
        cout << "not selected z-axis" << endl;
        return;
    }

    int REF_LENGTH_X = sqrt((refx.x - Origin.x)*(refx.x - Origin.x) + (refx.y - Origin.y)*(refx.y - Origin.y));
    int REF_LENGTH_Y = sqrt((refy.x - Origin.x)*(refy.x - Origin.x) + (refy.y - Origin.y)*(refy.y - Origin.y));
    int REF_LENGTH_Z = sqrt((refz.x - Origin.x)*(refz.x - Origin.x) + (refz.y - Origin.y)*(refz.y - Origin.y));


    scale_x = (0.5 * (refx.x - Origin.x)/(vanishPt_x.x - refx.x)
                 + 0.5 * (refx.y - Origin.y)/(vanishPt_x.y - refx.y)) / REF_LENGTH_X;

    scale_y = (0.5 * (refy.x - Origin.x)/(vanishPt_y.x - refy.x)
             + 0.5 * (refy.y - Origin.y)/(vanishPt_y.y - refy.y)) / REF_LENGTH_Y;

    scale_z = (0.5 * (refz.x - Origin.x)/(vanishPt_z.x - refz.x)
             + 0.5 * (refz.y - Origin.y)/(vanishPt_z.y - refz.y)) / REF_LENGTH_Z;

    cout << "scale_x is: " << scale_x << endl;
    cout << "scale_y is: " << scale_y << endl;
    cout << "scale_z is: " << scale_z << endl << endl;

    ProjMatrix = (cv::Mat_<double>(3,4) << scale_x*vanishPt_x.x, scale_y*vanishPt_y.x, scale_z*vanishPt_z.x, Origin.x,
                                          scale_x*vanishPt_x.y, scale_y*vanishPt_y.y, scale_z*vanishPt_z.y, Origin.y,
                                          scale_x*vanishPt_x.z, scale_y*vanishPt_y.z, scale_z*vanishPt_z.z, Origin.z);

    cout << "Projection Matrix is: " << ProjMatrix << endl << endl;

}


// step 3: Use homography matrix to get texture map
void MainWindow::getTextureMap(){
    cv::Mat Hxy = (cv::Mat_<double>(3,3) << scale_x*vanishPt_x.x, scale_y*vanishPt_y.x, Origin.x,
                                           scale_x*vanishPt_x.y, scale_y*vanishPt_y.y, Origin.y,
                                           scale_x*vanishPt_x.z, scale_y*vanishPt_y.z, Origin.z);

    cv::Mat Hxz = (cv::Mat_<double>(3,3) << scale_x*vanishPt_x.x, scale_z*vanishPt_z.x, Origin.x,
                                           scale_x*vanishPt_x.y, scale_z*vanishPt_z.y, Origin.y,
                                           scale_x*vanishPt_x.z, scale_z*vanishPt_z.z, Origin.z);

    cv::Mat Hyz = (cv::Mat_<double>(3,3) << scale_y*vanishPt_y.x, scale_z*vanishPt_z.x, Origin.x,
                                           scale_y*vanishPt_y.y, scale_z*vanishPt_z.y, Origin.y,
                                           scale_y*vanishPt_y.z, scale_z*vanishPt_z.z, Origin.z);

    cv::Mat dstImage;
    cout << "texture map starts" << endl << endl;
    cv::Mat tempImage = cv::Mat(image.size().height, image.size().width, image.type());


    // inverse warping
    cv::Mat invImage;
    cv::warpPerspective(image, dstImage, Hxy.inv(), tempImage.size(), INTER_LINEAR);
    cv::cvtColor(dstImage, dstImage, CV_BGR2RGB);
    imwrite("../../../../SingleViewModeling/SVM/temp/Hxy_image.jpg", dstImage);
    cv::warpPerspective(dstImage, invImage, Hxy, tempImage.size(), INTER_LINEAR);
    imwrite("../../../../SingleViewModeling/SVM/temp/Inv_Hxy_image.jpg", invImage);


    cv::warpPerspective(image, dstImage, Hxz.inv(), tempImage.size(), INTER_LINEAR);
    cv::cvtColor(dstImage, dstImage, CV_BGR2RGB);
    imwrite("../../../../SingleViewModeling/SVM/temp/Hxz_image.jpg", dstImage);
    cv::warpPerspective(dstImage, invImage, Hxz, tempImage.size(), INTER_LINEAR);
    imwrite("../../../../SingleViewModeling/SVM/temp/Inv_Hxz_image.jpg", invImage);


    cv::warpPerspective(image, dstImage, Hyz.inv(), tempImage.size(), INTER_LINEAR);
    cv::cvtColor(dstImage, dstImage, CV_BGR2RGB);
    imwrite("../../../../SingleViewModeling/SVM/temp/Hyz_image.jpg", dstImage);
    cv::warpPerspective(dstImage, invImage, Hyz, tempImage.size(), INTER_LINEAR);
    imwrite("../../../../SingleViewModeling/SVM/temp/Inv_Hyz_image.jpg", invImage);

}




// step 4: Mark interesting points





// step 5: Generate 3D vrml models






















