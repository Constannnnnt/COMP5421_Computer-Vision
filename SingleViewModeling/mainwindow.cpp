#include "mainwindow.h"
#include "ui_mainwindow.h"

#define REF_LENGTH_X 300
#define REF_LENGTH_Y 400
#define REF_LENGTH_Z 183
//#define REF_LENGTH_X 620
//#define REF_LENGTH_Y 284
//#define REF_LENGTH_Z 224

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
    get3d_mode = false;

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
    ui->actionGet_origin->setChecked(false);
    ui->actionGet_3D_point->setChecked(false);
    getVanish_mode_x = getVanish_mode_y = getVanish_mode_z = getVanish_mode_o =  get3d_mode = false;
}

void MainWindow::on_actionGet_vanish_x_triggered(bool checked){
    getVanish_mode_x = checked;

    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    ui->actionGet_origin->setChecked(false);
    getVanish_mode_y = getVanish_mode_z = getVanish_mode_o = false;
}
void MainWindow::on_actionGet_vanish_y_triggered(bool checked){
    getVanish_mode_y = checked;

    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    ui->actionGet_origin->setChecked(false);
    getVanish_mode_x = getVanish_mode_z = getVanish_mode_o = false;
}
void MainWindow::on_actionGet_vanish_z_triggered(bool checked){
    getVanish_mode_z = checked;

    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_origin->setChecked(false);
    getVanish_mode_x = getVanish_mode_y = getVanish_mode_o = false;
}
void MainWindow::on_actionGet_origin_triggered(bool checked){
    getVanish_mode_o = checked;

    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    getVanish_mode_x = getVanish_mode_y = getVanish_mode_z = false;
}

void MainWindow::on_actionGet_3D_point_triggered(bool checked){
    get3d_mode = checked;

    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    getVanish_mode_x = getVanish_mode_y = getVanish_mode_z = false;
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
    cal3DPosition();

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
        else if(getVanish_mode_o){
            Origin = cv::Point3f(p.x(), p.y(), 1);
            cv::circle(contour_image, cv::Point(p.x(),p.y()), 1, CV_RGB(128,128,128), 5);
        }
        else if(get3d_mode){

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

    // M = (cv::Mat_<float>(3,3) << 1,2,3, 4,5,6, 7,8,9.1);
    // temp_M = (cv::Mat_<float>(3,3) << 1,0,0, 0,0,0, 0,0,0);
    // cout << "temp_M is:  " << temp_M << endl;

    // get vanishing point x
    M = (cv::Mat_<float>(3,3) << 0,0,0, 0,0,0, 0,0,0);
    for(uint i=0; i<vanish_x.size()/4; i++){
        point1 = cv::Point3f(vanish_x[4*i], vanish_x[4*i+1], 1);
        point2 = cv::Point3f(vanish_x[4*i+2], vanish_x[4*i+3], 1);
        line = point1.cross(point2);

        temp_M = (cv::Mat_<float>(3,3) << line.x*line.x, line.x*line.y, line.x*line.z,
                                          line.x*line.y, line.y*line.y, line.y*line.z,
                                          line.x*line.z, line.y*line.z, line.z*line.z);
        M = M + temp_M;
    }

    cv::eigen(M, EigenValue, EigenVector, DBL_EPSILON);
    cout << "eigen values x: " << EigenValue << endl;
    cout << "eigen vectors x: " << EigenVector << endl << endl;

    vanishPt_x = cv::Point3f(EigenVector.at<float>(2,0)/EigenVector.at<float>(2,2),
                             EigenVector.at<float>(2,1)/EigenVector.at<float>(2,2), 1);
    cout << "Vanishing Point x: " << vanishPt_x << endl;


    // get vanishing point y
    M = (cv::Mat_<float>(3,3) << 0,0,0, 0,0,0, 0,0,0);
    for(uint i=0; i<vanish_y.size()/4; i++){
        point1 = cv::Point3f(vanish_y[4*i], vanish_y[4*i+1], 1);
        point2 = cv::Point3f(vanish_y[4*i+2], vanish_y[4*i+3], 1);
        line = point1.cross(point2);

        temp_M = (cv::Mat_<float>(3,3) << line.x*line.x, line.x*line.y, line.x*line.z,
                                          line.x*line.y, line.y*line.y, line.y*line.z,
                                          line.x*line.z, line.y*line.z, line.z*line.z);
        M = M + temp_M;
    }

    cv::eigen(M, EigenValue, EigenVector, DBL_EPSILON);
    cout << "eigen values y: " << EigenValue << endl;
    cout << "eigen vectors y: " << EigenVector << endl << endl;

    vanishPt_y = cv::Point3f(EigenVector.at<float>(2,0)/EigenVector.at<float>(2,2),
                             EigenVector.at<float>(2,1)/EigenVector.at<float>(2,2), 1);
    cout << "Vanishing Point y: " << vanishPt_y << endl;


    // get vanishing point z
    M = (cv::Mat_<float>(3,3) << 0,0,0, 0,0,0, 0,0,0);
    for(uint i=0; i<vanish_z.size()/4; i++){
        point1 = cv::Point3f(vanish_z[4*i], vanish_z[4*i+1], 1);
        point2 = cv::Point3f(vanish_z[4*i+2], vanish_z[4*i+3], 1);
        line = point1.cross(point2);

        temp_M = (cv::Mat_<float>(3,3) << line.x*line.x, line.x*line.y, line.x*line.z,
                                          line.x*line.y, line.y*line.y, line.y*line.z,
                                          line.x*line.z, line.y*line.z, line.z*line.z);
        M = M + temp_M;
    }

    cv::eigen(M, EigenValue, EigenVector, DBL_EPSILON);
    cout << "eigen values z: " << EigenValue << endl;
    cout << "eigen vectors z: " << EigenVector << endl << endl;

    vanishPt_z = cv::Point3f(EigenVector.at<float>(2,0)/EigenVector.at<float>(2,2),
                             EigenVector.at<float>(2,1)/EigenVector.at<float>(2,2), 1);
    cout << "Vanishing Point z: " << vanishPt_z << endl << endl;

}


// step 2: Calculate Projection Matrix
void MainWindow::calProjectionMatrix(){
    /**/
    refx = cv::Point3f(607, 430, 1);
    refy = cv::Point3f(173, 367, 1);
    refz = cv::Point3f(392, 399, 1);
    vanishPt_x = cv::Point3f(4045.92, -1393.23, 1);
    vanishPt_y = cv::Point3f(-1788.97, -1205.26, 1);
    vanishPt_z = cv::Point3f(370.262, 3659.41, 1);
    Origin = cv::Point3f(391, 542, 1);

    /*
    refx = cv::Point2f(2660, 1580);
    refy = cv::Point2f(803, 1680);
    refz = cv::Point2f(1387, 1433);
    vanishPt_x = cv::Point3f(5345.87, 345.158, 1);
    vanishPt_y = cv::Point3f(-966.982, 204.371, 1);
    vanishPt_z = cv::Point3f(1443.49, 9398.12, 1);
    Origin = cv::Point3f(1390, 2173, 1);
    */

    // scale_z could have up to a double deviation from correct if refz.x = Origin.x = 391
    /*
    cout << "\n scale_x \n";
    cout << (refx.x - Origin.x)/(vanishPt_x.x - refx.x) << endl;
    cout << (refx.y - Origin.y)/(vanishPt_x.y - refx.y) << endl;
    cout << "\n scale_y \n";
    cout << (refy.x - Origin.x)/(vanishPt_y.x - refy.x) << endl;
    cout << (refy.y - Origin.y)/(vanishPt_y.y - refy.y) << endl;
    cout << "\n scale_z \n";
    cout << (refz.x - Origin.x)/(vanishPt_z.x - refz.x) << endl;
    cout << (refz.y - Origin.y)/(vanishPt_z.y - refz.y) << endl;
    */

    scale_x = (0.5 * (refx.x - Origin.x)/(vanishPt_x.x - refx.x)
             + 0.5 * (refx.y - Origin.y)/(vanishPt_x.y - refx.y)) / REF_LENGTH_X;

    scale_y = (0.5 * (refy.x - Origin.x)/(vanishPt_y.x - refy.x)
             + 0.5 * (refy.y - Origin.y)/(vanishPt_y.y - refy.y)) / REF_LENGTH_Y;

    scale_z = (0.5 * (refz.x - Origin.x)/(vanishPt_z.x - refz.x)
             + 0.5 * (refz.y - Origin.y)/(vanishPt_z.y - refz.y)) / REF_LENGTH_Z;

    cout << "scale_x is: " << scale_x << endl;
    cout << "scale_y is: " << scale_y << endl;
    cout << "scale_z is: " << scale_z << endl << endl;

    ProjMatrix = (cv::Mat_<float>(3,4) << scale_x*vanishPt_x.x, scale_y*vanishPt_y.x, scale_z*vanishPt_z.x, Origin.x,
                                          scale_x*vanishPt_x.y, scale_y*vanishPt_y.y, scale_z*vanishPt_z.y, Origin.y,
                                          scale_x*vanishPt_x.z, scale_y*vanishPt_y.z, scale_z*vanishPt_z.z, Origin.z);

    cout << "Projection Matrix is: " << ProjMatrix << endl << endl;

}


// step 3: Use homography matrix to get texture map
void MainWindow::getTextureMap(){

    cv::Mat Hxy = (cv::Mat_<float>(3,3) << scale_x*vanishPt_x.x, scale_y*vanishPt_y.x, Origin.x,
                                           scale_x*vanishPt_x.y, scale_y*vanishPt_y.y, Origin.y,
                                           scale_x*vanishPt_x.z, scale_y*vanishPt_y.z, Origin.z);

    cv::Mat Hxz = (cv::Mat_<float>(3,3) << scale_x*vanishPt_x.x, scale_z*vanishPt_z.x, Origin.x,
                                           scale_x*vanishPt_x.y, scale_z*vanishPt_z.y, Origin.y,
                                           scale_x*vanishPt_x.z, scale_z*vanishPt_z.z, Origin.z);

    cv::Mat Hyz = (cv::Mat_<float>(3,3) << scale_y*vanishPt_y.x, scale_z*vanishPt_z.x, Origin.x,
                                           scale_y*vanishPt_y.y, scale_z*vanishPt_z.y, Origin.y,
                                           scale_y*vanishPt_y.z, scale_z*vanishPt_z.z, Origin.z);

    /*
    cv::warpPerspective(image, dstImage, Hxy.inv(), tempImage.size());
    QImage Q_img = QImage ((const unsigned char*)(dstImage.data), dstImage.cols, dstImage.rows, dstImage.step, QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(Q_img).scaled(QPixmap::fromImage(Q_img).width()*img_scale,
                                                          QPixmap::fromImage(Q_img).height()*img_scale, Qt::KeepAspectRatio));
    */

    cv::Mat dstImage;
    cv::Mat perspective_matrix;
    cv::Mat tempImage;

    Point2f v1 = Point2f(162, 227);
    Point2f v2 = Point2f(173, 367);
    Point2f v3 = Point2f(391, 542);
    Point2f v4 = Point2f(392, 399);
    Point2f v5 = Point2f(607, 430);
    Point2f v6 = Point2f(618, 290);
    Point2f v7 = Point2f(379, 139);
    //Point2f v8 = Point2f(382, 277);

    // patch xy
    Point2f pts1[] = {v4, v6, v1, v7};
    Point2f pts2[] = {Point2f(0,0), Point2f(REF_LENGTH_X,0), Point2f(0,REF_LENGTH_Y), Point2f(REF_LENGTH_X,REF_LENGTH_Y)};

    perspective_matrix = cv::getPerspectiveTransform(pts1, pts2);
    tempImage = cv::Mat(REF_LENGTH_Y, REF_LENGTH_X, image.type());

    cv::warpPerspective(image, dstImage, perspective_matrix, tempImage.size(), INTER_LINEAR);
    cv::cvtColor(dstImage, dstImage, CV_BGR2RGB);
    imwrite("/home/jguoaj/Desktop/SingleViewModel/temp/xy_patch.jpg", dstImage);


    // patch xz
    Point2f pts3[] = {v3, v5, v4, v6};
    Point2f pts4[] = {Point2f(0,0), Point2f(REF_LENGTH_X,0), Point2f(0,REF_LENGTH_Z), Point2f(REF_LENGTH_X,REF_LENGTH_Z)};

    perspective_matrix = cv::getPerspectiveTransform(pts3, pts4);
    tempImage = cv::Mat(REF_LENGTH_Z, REF_LENGTH_X, image.type());

    cv::warpPerspective(image, dstImage, perspective_matrix, tempImage.size(), INTER_LINEAR);
    cv::cvtColor(dstImage, dstImage, CV_BGR2RGB);
    imwrite("/home/jguoaj/Desktop/SingleViewModel/temp/xz_patch.jpg", dstImage);


    // patch yz
    Point2f pts5[] = {v3, v2, v4, v1};
    Point2f pts6[] = {Point2f(0,0), Point2f(REF_LENGTH_Y,0), Point2f(0,REF_LENGTH_Z), Point2f(REF_LENGTH_Y,REF_LENGTH_Z)};

    perspective_matrix = cv::getPerspectiveTransform(pts5, pts6);
    tempImage = cv::Mat(REF_LENGTH_Z, REF_LENGTH_Y, image.type());

    cv::warpPerspective(image, dstImage, perspective_matrix, tempImage.size(), INTER_LINEAR);
    cv::cvtColor(dstImage, dstImage, CV_BGR2RGB);
    imwrite("/home/jguoaj/Desktop/SingleViewModel/temp/yz_patch.jpg", dstImage);


    /*
    cv::Mat tempImage = cv::Mat(image.size().height*2, image.size().width*2, image.type());

    cv::warpPerspective(image, dstImage, Hxy.inv(), tempImage.size(), INTER_LINEAR);
    cv::cvtColor(dstImage, dstImage, CV_BGR2RGB);
    imwrite("/home/jguoaj/Desktop/SingleViewModel/temp/Hxy_image.jpg", dstImage);
    cv::warpPerspective(dstImage, invImage, Hxy, tempImage.size(), INTER_LINEAR);
    imwrite("/home/jguoaj/Desktop/SingleViewModel/temp/Inv_Hxy_image.jpg", invImage);


    cv::warpPerspective(image, dstImage, Hxz.inv(), tempImage.size(), INTER_LINEAR);
    cv::cvtColor(dstImage, dstImage, CV_BGR2RGB);
    imwrite("/home/jguoaj/Desktop/SingleViewModel/temp/Hxz_image.jpg", dstImage);
    cv::warpPerspective(dstImage, invImage, Hxz, tempImage.size(), INTER_LINEAR);
    imwrite("/home/jguoaj/Desktop/SingleViewModel/temp/Inv_Hxz_image.jpg", invImage);


    cv::warpPerspective(image, dstImage, Hyz.inv(), tempImage.size(), INTER_LINEAR);
    cv::cvtColor(dstImage, dstImage, CV_BGR2RGB);
    imwrite("/home/jguoaj/Desktop/SingleViewModel/temp/Hyz_image.jpg", dstImage);
    cv::warpPerspective(dstImage, invImage, Hyz, tempImage.size(), INTER_LINEAR);
    imwrite("/home/jguoaj/Desktop/SingleViewModel/temp/Inv_Hyz_image.jpg", invImage);
    */

}


// step 4: Mark interesting points
inline float norml2(Point3f p){
    return sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
}

float MainWindow::getRefHeight(Point3f r, Point3f b){

    // we want to find Reference height R
    float R;
    float H = REF_LENGTH_Z;
    Point3f b0, v, t, temp;
    b0 = Point3f(Origin.x, Origin.y, 1);
    temp = ( b.cross(b0) ).cross( vanishPt_x.cross(vanishPt_y) );
    v = Point3f(temp.x/temp.z, temp.y/temp.z, 1);
    temp = ( v.cross(refz) ).cross( r.cross(b) );
    t = Point3f(temp.x/temp.z, temp.y/temp.z, 1);

    R = H * norml2(r-b) * norml2(vanishPt_z-t) / ( norml2(t-b) * norml2(vanishPt_z-r) );
    return R;
}

Point3f MainWindow::get3dCoor(Point3f r, Point3f b){

    float z0 = getRefHeight(r, b);
    //cout << "\n 3D height z0 is:  " << z0 << endl;

    Mat point_3d;
    Mat point_2d = ( cv::Mat_<float>(3,1) << r.x, r.y, r.z );
    Mat Hz = (cv::Mat_<float>(3,3) << scale_x*vanishPt_x.x, scale_y*vanishPt_y.x, scale_z*z0*vanishPt_z.x + Origin.x,
                                      scale_x*vanishPt_x.y, scale_y*vanishPt_y.y, scale_z*z0*vanishPt_z.y + Origin.y,
                                      scale_x*vanishPt_x.z, scale_y*vanishPt_y.z, scale_z*z0*vanishPt_z.z + Origin.z);

    point_3d = Hz.inv() * point_2d;

    Point3f p = Point3f(point_3d.at<float>(0,0)/point_3d.at<float>(2,0),
                        point_3d.at<float>(1,0)/point_3d.at<float>(2,0), z0);
    return p;
}

void MainWindow::cal3DPosition(){

    /*
    Point2f v1 = Point2f(162, 227);
    Point2f v2 = Point2f(173, 367);
    Point2f v3 = Point2f(391, 542);
    Point2f v4 = Point2f(392, 399);
    Point2f v5 = Point2f(607, 430);
    Point2f v6 = Point2f(618, 290);
    Point2f v7 = Point2f(379, 139);
    */

    Point3f r1 = Point3f(618, 290, 1);
    Point3f b1 = Point3f(607, 430, 1);
    Point3f p1 = get3dCoor(r1, b1);
    cout << "p1 coordinate is: " << p1 << endl;



}


// step 5: Generate 3D vrml models
void SingleViewModel::generateVRML(const string &prefix)
{
    string fname = prefix + ".wrl";
    ofstream ofile(fname.c_str());
    ofile << "#VRML V2.0 utf8" << endl << endl;
    ofile << "Collision {" << endl;
    ofile << "    collide FALSE" << endl;
    ofile << "    children [" << endl;

    for(int i=0; i<faces.size(); i++){
        Face *face = faces[i];
        ofile << "Shape{" << endl;
        ofile << "    appearance  Appearance{" << endl;
        ofile << "        texture  ImageTexture{" << endl;
        ofile << "           url \"" << face->TexFileName() << "\"" << endl;
        ofile << "        }" << endl;
        ofile << "    }" << endl;
        ofile << "    geometry IndexedFaceSet {" << endl;
        ofile << "       coord Coordinate {" << endl;
        ofile << "         point[";
        for(int j=3; j>0; j--)
        {
            cv::Point3d p = face->realvertexs[j]->Coor3d();
            ofile << p.x << " " << p.y << " " << p.z << ", ";
        }
        cv::Point3d p = face->realvertexs[0]->Coor3d();
        ofile << p.x << " " << p.y << " " << p.z << "]" << endl;
        ofile<<"       }" << endl;
        ofile<<"       coordIndex [0,1,2,3,-1]" << endl;
        ofile<<"       ccw TRUE" << endl;
        ofile<<"       solid FALSE" << endl;
        ofile<<"       texCoord TextureCoordinate {" << endl;
        ofile<<"       point [0  0, 1  0, 1  1, 0  1]" << endl;
        ofile<<"       }" << endl;
        ofile<<"       texCoordIndex[0 1 2 3 -1]" << endl;
        ofile<<"    }" << endl;
        ofile<<"}" << endl;
    }

}



/*
void SingleViewModel::generateVRMLCode(const string &prefix)
{
    for(int i=0;i<faces.size();i++)
    {
        Face *face=faces[i];
        char fID[20];
        sprintf(fID,"_%.3d.png",face->ID());
        string fname(fID);
        fname=prefix+fname;
        face->Texture().save(fname.c_str());
        face->textureFileName=fname;
    }
    string fname=prefix+".wrl";
    ofstream ofile(fname.c_str());
    ofile<<"#VRML V2.0 utf8"<<endl;
    ofile<<"Transform {"<<endl;
    ofile<<"  translation "<<camCenter.x<<" "<<camCenter.y<<" "<<camCenter.z<<endl;
    ofile<<"  children ["<<endl;
    ofile<<"    Shape {"<<endl;
    ofile<<"      geometry Sphere {"<<endl;
    ofile<<"        radius 0.15"<<endl;
    ofile<<"      }"<<endl;
    ofile<<"    }"<<endl;
    ofile<<"  ]"<<endl;
    ofile<<"}"<<endl;
    for(int i=0;i<faces.size();i++)
    {
        Face *face=faces[i];
        ofile<<"Shape{"<<endl;
        ofile<<"    appearance  Appearance{"<<endl;
        ofile<<"        texture  ImageTexture{"<<endl;
        ofile<<"           url \""<<face->TexFileName()<<"\""<<endl;
        ofile<<"        }"<<endl;
        ofile<<"    }"<<endl;
        ofile<<"    geometry IndexedFaceSet {"<<endl;
        ofile<<"       coord Coordinate {"<<endl;
        ofile<<"         point[";
        for(int j=3;j>0;j--)
        {
            cv::Point3d p=face->realvertexs[j]->Coor3d();
            ofile<<p.x<<" "<<p.y<<" "<<p.z<<", ";
        }
        cv::Point3d p=face->realvertexs[0]->Coor3d();
        ofile<<p.x<<" "<<p.y<<" "<<p.z<<"]"<<endl;
        ofile<<"       }"<<endl;
        ofile<<"       coordIndex [0,1,2,3,-1]"<<endl;
        ofile<<"       ccw TRUE"<<endl;
        ofile<<"       solid FALSE"<<endl;
        ofile<<"       texCoord TextureCoordinate {"<<endl;
        ofile<<"       point [0  0, 1  0, 1  1, 0  1]"<<endl;
        ofile<<"       }"<<endl;
        ofile<<"       texCoordIndex[0 1 2 3 -1]"<<endl;
        ofile<<"    }"<<endl;
        ofile<<"}"<<endl;
    }
}
*/




















