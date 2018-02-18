#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

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

    // ui->scrollArea->setWidgetResizable(true);
    // ui->scrollArea->setWidget(ui->label);
    // setCentralWidget(ui->scrollArea);

    costgraph_weight = new cv::Mat[9];
    img_scale = 1.0;

    // ui->actionDisplay_Contour->setChecked(true);
    contour_enabled = false;
    scissor_enabled = false;
    first_seed_flag = false;

    // test
    //Mat C = (Mat_<double>(2,2) << 0, 1, 2, 3);
    //cout << C << endl << endl;
    //cout << C.mul(C) << endl;
}


MainWindow::~MainWindow()
{
    delete ui;
    delete scrollArea;

    delete head_node;

    delete[] costgraph_weight;
}

/* helper function starts here  */
// display the image
void MainWindow::display_image(Mat im)
{
    cv::Mat img_tmp = im.clone();

    // cv::cvtColor(img_tmp, img_tmp, CV_BGR2RGB);
    QImage Q_img = QImage((const unsigned char*)(img_tmp.data),img_tmp.cols,img_tmp.rows,QImage::Format_RGB888);

    QPixmap p = QPixmap::fromImage(Q_img);
    ui->label->setPixmap( p.scaled(p.width()*img_scale, p.height()*img_scale, Qt::KeepAspectRatio) );
}

void MainWindow::print_node(pixelNode* n)
{
    pixelNode* p = n;
    while(p->getParent() != NULL){
        cout << p->getCol() << '\t' << p->getRow() << endl;
        p = p->getParent();
    }
}

void MainWindow::draw_contour(int x, int y){

    int draw_value;
    cv::Vec3b fill(0,255,0);
    //contour = cv::Mat::zeros(image.size(), CV_8UC3);
    contour_image = image.clone();

    while( this->parentMap.at<uchar>( cv::Point(x,y) ) != 255 ){

        draw_value = this->parentMap.at<uchar>( cv::Point(x,y) );

        contour_image.at<cv::Vec3b>( cv::Point(x,y) ) = fill;
        //contour.at<cv::Vec3b>( cv::Point(x,y) ) = fill;

        //cout << "draw value is: " << draw_value << endl;

        //  0   1   2
        //  3   4   5
        //  6   7   8
        switch (draw_value) {
        case 0:
            y -= 1;
            x -= 1;
            continue;
        case 1:
            y -= 1;
            continue;
        case 2:
            y -= 1;
            x += 1;
            continue;
        case 3:
            x -= 1;
            continue;
        case 4:
            cout << "self loop detected" << endl;
            exit(1);
            break;
        case 5:
            x += 1;
            continue;
        case 6:
            y += 1;
            x -= 1;
            continue;
        case 7:
            y += 1;
            continue;
        case 8:
            y += 1;
            x += 1;
            continue;
        //default:
        //    continue;
        }
    }
}

void MainWindow::resetAll(){
    ui->actionPixel_Node->setChecked(false);
    ui->actionCost_Graph->setChecked(false);
    ui->actionGaussian_5->setChecked(false);
    ui->actionGuassian_3->setChecked(false);
    ui->actionDisplay_Contour->setChecked(false);
    ui->actionScissor->setChecked(false);
    img_scale = 1.0;
    contour_enabled = false;
    scissor_enabled = false;
    first_seed_flag = false;
    return;
}

QImage MainWindow::drawPathTree(){
    int w=Qimg->width(),h=Qimg->height();
    QImage qi(3*w,3*h,Qimg->format());
    qi.fill(qRgb(0,0,0));
    for(int j = 0; j < h; j++)
        for(int i = 0; i < w; i++)
        {
                pixelNode *pn=pixelnodes[j][i];
                qi.setPixel(3*i+1,3*j+1,Qimg->pixel(i,j));
                pixelNode *prevpn=pn->prevNode;
                if(prevpn!=NULL)
                {
                    int c = 3 * i + 1 + prevpn->getCol() - pn->getCol();
                    int r = 3 * j + 1 + prevpn->getRow() - pn->getRow();
                    qi.setPixel(c,r,qRgb(255,255,0));
                    c=3 * prevpn->getCol() + 1 + pn->getCol()- prevpn->getCol();
                    r=3 * prevpn->getRow() + 1 + pn->getRow() - prevpn->getRow();
                    qi.setPixel(c,r,qRgb(127,127,0));
                }
        }
    return qi;
}

void MainWindow::computeCostFunc(){
    int w = Qimg->width(), h = Qimg->height();
    double maxD = -1.0;
    for (int j = 0; j < h; j ++)
        for (int i = 0; i < w; i++) {
            pixelNode *pn=pixelnodes[j][i];
            for(int k=0;k<8;k++)
            {
                pn->setLinkCost(k,getDLink(i,j,k));
                if(pn->getLinkCost(k)>maxD)
                    maxD=pn->getLinkCost(k);
            }
        }
    for (int j = 0; j < h; j ++)
        for (int i = 0; i < w; i ++) {
            pixelNode *pn = pixelnodes[j][i];
            for (int k = 0; k < 8; k++) {
                double length = k % 2 == 0 ? 1.0 : sqrt(2);
                double DLink = pn->getLinkCost(k);
                pn->setLinkCost(k, (maxD - DLink) * length);
            }
        }
}

void MainWindow::channelTransform(QRgb rgb, int color[3]) {
    color[0] = qRed(rgb);
    color[1] = qGreen(rgb);
    color[2] = qBlue(rgb);
}

double MainWindow::getDLink(int i, int j, int k) {
    double DLink = 0.0;
    double D[3];
    int c0[3], c1[3], c2[3], c3[3];
    switch (k) {
    case 0:
        if(j == 0 || j == Qimg->height() - 1 || i == Qimg->width() - 1) return -1.0; // boundary exceed;
        channelTransform(Qimg->pixel(i, j - 1), c0);
        channelTransform(Qimg->pixel(i + 1, j - 1), c1);
        channelTransform(Qimg->pixel(i, j + 1), c2);
        channelTransform(Qimg->pixel(i + 1, j + 1), c3);
        for (int t = 0; t <3; t ++) {
            D[t]=fabs((c0[t] + c1[t]) / 2.0-(c2[t] + c3[t]) / 2.0) / 2.0;
        }
        break;
    case 1:
        if (j == 0 || i == Qimg->width() - 1) return -1.0;
        channelTransform(Qimg->pixel(i + 1, j), c0);
        channelTransform(Qimg->pixel(i, j - 1), c1);
        for (int t = 0; t <3; t ++) {
            D[t]=fabs(1.0 * (c0[t] - c1[t])) / sqrt(2);
        }
        break;
    case 2:
        if (i ==0 || i == Qimg->width() - 1 || j == 0) return -1.0;
        channelTransform(Qimg->pixel(i - 1, j), c0);
        channelTransform(Qimg->pixel(i - 1, j - 1), c1);
        channelTransform(Qimg->pixel(i + 1, j), c2);
        channelTransform(Qimg->pixel(i + 1, j - 1), c3);
        for (int t = 0; t <3; t ++) {
            D[t]=fabs((c0[t] + c1[t]) / 2.0 - (c2[t] + c3[t]) / 2.0) / 2.0;
        }
        break;
    case 3:
        if (i == 0 || j == 0) return -1.0;
        channelTransform(Qimg->pixel(i - 1, j), c0);
        channelTransform(Qimg->pixel(i, j - 1), c1);
        for (int t = 0; t <3; t ++) {
            D[t]=fabs(1.0 * (c0[t] - c1[t])) / sqrt(2);
        }
        break;
    case 4:
        if (i == 0 || j == 0 || j == Qimg->height() -1) return -1.0;
        channelTransform(Qimg->pixel(i, j - 1), c0);
        channelTransform(Qimg->pixel(i - 1, j - 1), c1);
        channelTransform(Qimg->pixel(i - 1, j + 1), c2);
        channelTransform(Qimg->pixel(i, j + 1), c3);
        for (int t = 0; t <3; t ++) {
            D[t]=fabs((c0[t] + c1[t]) / 2.0 - (c2[t] + c3[t]) / 2.0) / 2.0;
        }
        break;
    case 5:
        if (i == 0 || j == Qimg->height() - 1) return -1.0;
        channelTransform(Qimg->pixel(i - 1, j), c0);
        channelTransform(Qimg->pixel(i, j + 1), c1);
        for (int t = 0; t <3; t ++) {
            D[t]=fabs(1.0 * (c0[t] - c1[t])) / sqrt(2);
        }
        break;
    case 6:
        if (i == 0 || i == Qimg->width() - 1 || j == Qimg->height() -1) return -1.0;
        channelTransform(Qimg->pixel(i - 1, j), c0);
        channelTransform(Qimg->pixel(i - 1, j + 1), c1);
        channelTransform(Qimg->pixel(i + 1, j), c2);
        channelTransform(Qimg->pixel(i + 1, j + 1), c3);
        for (int t = 0; t <3; t ++) {
            D[t]=fabs((c0[t] + c1[t]) / 2.0 - (c2[t] + c3[t]) / 2.0) / 2.0;
        }
        break;
    case 7:
        if (i == Qimg->width() - 1 || j == Qimg->height() - 1) return -1.0;
        channelTransform(Qimg->pixel(i + 1, j), c0);
        channelTransform(Qimg->pixel(i, j + 1), c1);
        for (int t = 0; t <3; t ++) {
            D[t]=fabs(1.0 * (c0[t] - c1[t])) / sqrt(2);
        }
        break;
    default:
        return - 1.0;
    }
    for (int t = 0; t < 3; t ++)
        DLink += D[t] * D[t];
    DLink = sqrt(DLink) / 3;
    return DLink;
}

/* helper function ends here */

// open a image
void MainWindow::on_actionOpen_triggered()
{
    resetAll();

    QString fileName = QFileDialog::getOpenFileName(
                this, tr("Open Image"), ".", tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
    image = cv::imread(fileName.toLatin1().data());
    contour_image = cv::imread(fileName.toLatin1().data());
    contour = cv::Mat::zeros(image.size(), CV_8UC3);
    current_image = image;
    previous_image = image;

    // convert cv::Mat to QImage
    cv::cvtColor(image, image, CV_BGR2RGB);
    cv::cvtColor(contour_image, contour_image, CV_BGR2RGB);

    QImage Q_img = QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);

    Qimg = new QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
    pixelnodes.resize(Q_img.height());
    for(int i=0;i<Q_img.height();i++)
        for(int j=0;j<Q_img.width();j++)
            pixelnodes[i].push_back(new pixelNode(j,i));
    QImage mask(Qimg->width(),Qimg->height(),Qimg->format());
    mask.fill(qRgb(255, 255, 255));
    Mask = &mask;
    head_node = NULL;
    pathTree = new QImage(drawPathTree());
    computeCostFunc();
    cout << "cost finished" << endl;

    ui->label->setPixmap(QPixmap::fromImage(Q_img));

}

// Add image
void MainWindow::addImage() {

}


// Save image
void MainWindow::on_actionSave_Contour_triggered()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Save ", "Save image with contour marked?",
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
    if(reply == QMessageBox::Yes) {
        // cv::imwrite("/home/jguoaj/Desktop/mask_image.jpg", mask_image);

    }
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
        display_image(current_image);
}


void MainWindow::on_actionZoom_Out_triggered()
{
    img_scale -= 0.1;
    if(contour_enabled == true)
        display_image(contour_image);
    else
        display_image(current_image);
}


// display help message
void MainWindow::on_actionHelp_triggered()
{
    // this->print_node(current_node);
    cout << "Image size is: " << image.rows << '\t' << image.cols << endl;

    // QString text = QString("Nothing for help !");
    // QMessageBox::about(this, "Help", text);
}


// Work Mode
void MainWindow::on_actionScissor_triggered(bool checked)
{
    scissor_enabled = checked;
    if (scissor_enabled) {

    }
}


void MainWindow::on_actionDisplay_Contour_triggered(bool checked)
{
    contour_enabled = checked;
    if (contour_enabled) {
        display_image(contour_image);
    }
    else {
        display_image(current_image);
    }
}


void MainWindow::on_actionReset_Contour_triggered()
{
    cout << "reset contour" << endl;
    contour_image = image.clone();

}


void MainWindow::on_actionFinish_Contour_triggered()
{
    // close the loop
    // cv::line(contour, cv::Point(x_list[0],y_list[0]), cv::Point(x_list[list_size-1], y_list[list_size-1]), Scalar(255,0,0));
}


// Debug Mode
void MainWindow::on_actionPixel_Node_triggered(bool checked)
{
    if (checked) {
        QImage* Q_img = new QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
        int w=Q_img->width(), h=Q_img->height();
        QImage png(3*w,3*h,Q_img->format());
        png.fill(qRgb(0,0,0));
        for(int j=0;j<h;j++)
            for(int i=0;i<w;i++)
                png.setPixel(3*i+1,3*j+1,Q_img->pixel(i,j));
        QPixmap p = QPixmap::fromImage(png);
        ui->label->setPixmap(p.scaled(p.width()*img_scale, p.height()*img_scale, Qt::KeepAspectRatio));
//        QImage Q_img_tmp = Q_img->rgbSwapped();
        previous_image = current_image;
        current_image = cv::Mat( png.height(), png.width(),
                                 CV_8UC3,
                                 const_cast<uchar*>(png.bits()),
                                 static_cast<size_t>(png.bytesPerLine())
                                 ).clone();
        delete Q_img;
    } else {
        current_image = previous_image;
        display_image(current_image);
    }
}


void MainWindow::on_actionCost_Graph_triggered(bool checked)
{
    if (checked) {
//        if (graphCost.empty()) {
//            cout << "No dijkstra' algorithm completed" << endl;
//            ui->actionCost_Graph->setChecked(false);
//            return;
//        }
        QImage* Q_img = new QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
        int w=Q_img->width(), h=Q_img->height();
        QImage png(3*w,3*h,Q_img->format());
        png.fill(qRgb(255,255,255));
        for(int j=0;j<h;j++) {
            for(int i=0;i<w;i++) {
                pixelNode *pn=pixelnodes[j][i];
                png.setPixel(3*i+1,3*j+1,Q_img->pixel(i,j)); // i, j
                if (i + 1 < w) png.setPixel(3*i+2,3*j+1,qRgb(pn->getLinkCost(0) * 1.5,
                                                             pn->getLinkCost(0) * 1.5,
                                                             pn->getLinkCost(0) * 1.5)); // link 0
                if (i + 1 < w && j - 1 >=0) png.setPixel(3*i+2,3*j,qRgb(pn->getLinkCost(1) * 1.5,
                                                                        pn->getLinkCost(1) * 1.5,
                                                                        pn->getLinkCost(1) * 1.5)); // link 1
                if (j - 1 >= 0) png.setPixel(3*i+1,3*j,qRgb(pn->getLinkCost(2) * 1.5,
                                                            pn->getLinkCost(2) * 1.5,
                                                            pn->getLinkCost(2) * 1.5)); // link 2
                if (i - 1 >=0 && j - 1 >=0) png.setPixel(3*i,3*j,qRgb(pn->getLinkCost(3) * 1.5,
                                                                      pn->getLinkCost(3) * 1.5,
                                                                      pn->getLinkCost(3) * 1.5)); // link 3
                if (i - 1 >= 0) png.setPixel(3*i,3*j+1,qRgb(pn->getLinkCost(4) * 1.5,
                                                            pn->getLinkCost(4) * 1.5,
                                                            pn->getLinkCost(4) * 1.5)); // link 4
                if (i - 1 >= 0 && j + 1 < h) png.setPixel(3*i,3*j+2,qRgb(pn->getLinkCost(5) * 1.5,
                                                                         pn->getLinkCost(5) * 1.5,
                                                                         pn->getLinkCost(5) * 1.5)); // link 5
                if (j + 1 < h) png.setPixel(3*i+1,3*j+2,qRgb(pn->getLinkCost(6) * 1.5,
                                                             pn->getLinkCost(6) * 1.5,
                                                             pn->getLinkCost(6) * 1.5)); // link 6
                if (i + 1 < w && j + 1 < h) png.setPixel(3*i+2,3*j+2,qRgb(pn->getLinkCost(7) * 1.5,
                                                                          pn->getLinkCost(7) * 1.5,
                                                                          pn->getLinkCost(7) * 1.5)); // link 7
            }
        }
        QPixmap p = QPixmap::fromImage(png);
        ui->label->setPixmap(p.scaled(p.width()*img_scale, p.height()*img_scale, Qt::KeepAspectRatio));
//        QImage Q_img_tmp = Q_img->rgbSwapped();
        previous_image = current_image;
        current_image = cv::Mat( png.height(), png.width(),
                                 CV_8UC3,
                                 const_cast<uchar*>(png.bits()),
                                 static_cast<size_t>(png.bytesPerLine())
                                 ).clone();

    } else {
        current_image = previous_image;
        display_image(current_image);
    }
}


void MainWindow::on_actionPath_Tree_triggered()
{

}


void MainWindow::on_actionMin_Path_triggered()
{

}


// overloading QT functions
void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        ctrl_enabled = true;
    }
    return;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        ctrl_enabled = false;
    }
    return;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {

    // control + left click: first seed
    if ( (event->type() == QEvent::MouseButtonPress) && (ctrl_enabled) &&
         (strcmp(watched->metaObject()->className(), "MainWindow")) == 0 && !first_seed_flag)
    {
        QMouseEvent* me = static_cast<QMouseEvent*> (event);
        QPoint p = ui->label->mapFrom(this, me->pos());
        p /= img_scale;
        cout << p.x() << " " << p.y() << endl; // get the pos of the first seed
        if (!scissor_enabled) {
            cout << "scissor is not enabled" << endl;
            return false;
        }
        cout << "ctrl + left click" << endl;

        head_node = pixelnodes[p.x()][p.y()];
        current_node = head_node;

        // draw a dot
        cv::circle(contour_image, cv::Point(p.x(),p.y()), 1, CV_RGB(0,0,255), 2);
        if(contour_enabled)
            display_image(contour_image);

//        // compute cost graph
//        costgraph_init();

//        // Dijkstra algorithm
//        Dijstras(head_node);
        updatePathTree();

        cout << "pass Dijkstra algorithm" << endl;
        first_seed_flag = true;

    }

    // left click: follwing seeds
    if ( (event->type() == QEvent::MouseButtonPress) && (!ctrl_enabled) &&
         (strcmp(watched->metaObject()->className(), "MainWindow")) == 0)
    {
        QMouseEvent* me = static_cast<QMouseEvent*> (event);
        QPoint p = ui->label->mapFrom(this, me->pos());
        p /= img_scale;

        // p.y() - image.rows - down direction
        // p.x() - image.cols - right direction
        cout << p.x() << " " << p.y() << endl;
        if (!scissor_enabled) {
            cout << "scissor is not enabled" << endl;
            return false;
        }
        cout << "left click" << endl;

//        pixelNode* nod = new pixelNode(p.x(), p.y(), 100000);
//        nod->setParent(current_node);
//        current_node = nod;

        current_node = pixelnodes[p.x()][p.y()];

        updatePathTree();

        /*test positions in the image*/
        // QImage Q_img = QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
        // cout << "pixel at the pos" << Q_img.pixel(p.x(),p.y()) <<  endl;
        // draw the path based on the movement of the mouse
    }

    // Mouse move event
    if ( (event->type() == QEvent::MouseMove) && ui->label->underMouse() && (first_seed_flag == true) ){
        QMouseEvent* me = static_cast<QMouseEvent*> (event);
        //QPoint p = ui->label->mapFrom(this, me->pos());
        QPoint p = me->pos();
        p /= img_scale;

        QString myText = QString("Intelligent Scissor ");
        statusBar()->showMessage(QString("(%1, %2) ").arg(p.x()).arg(p.y()) + myText);

        // cout << "p pos is: " << p.x() << '\t' << p.y() << endl << endl;
        // check boundary, this causes the crash
        if ( (p.y() > 0) && (p.x() > 0) &&
             (p.y() < image.rows-1) && (p.x() < image.cols-1)){

//            this->draw_contour(p.x(), p.y());
//            if (contour_enabled)
//                display_image(contour_image);
        }

    }


    if ( event->type() == QEvent::KeyPress){
        QKeyEvent* event_key = static_cast<QKeyEvent*> (event);

        // enter: finish the current
        if ( (event_key->key()==Qt::Key_Enter) && (!ctrl_enabled) ){
            this->on_actionFinish_Contour_triggered();
        }

        // ctrl + enter: not sure what it means
        if ( (event_key->key()==Qt::Key_Enter) && ctrl_enabled ){
            this->on_actionFinish_Contour_triggered();
        }

        // backspace when scissoring
        if ( (event_key->key()==Qt::Key_Backspace) && scissor_enabled ){

        }

        // backspace when not scissoring
        if ( (event_key->key()==Qt::Key_Backspace) && (!scissor_enabled) ){

        }
    }

    return false;
}

void MainWindow::updatePathTree()
{
    int h=Qimg->height(),w=Qimg->width();
    for(int j=0;j<h;j++)
        for(int i=0;i<w;i++)
        {
                pixelnodes[j][i]->totalCost=DBL_MAX;
                pixelnodes[j][i]->state=pixelNode::INITIAL;
        }
     current_node->resetTotalCost(0);
     current_node->resetPrevNode();
     FibHeap heap;
     heap.Insert(current_node);
     pixelNode * q;
     while (heap.Minimum() != NULL) {
         q = (pixelNode*) heap.ExtractMin();
         q->state = pixelNode::EXPANDED;
         int c, r;
         for (int i = 0; i < 8 ;i ++) {
             q->Neighbor(i, c, r);
             if (c >= 0  && c < Qimg->width()
                     && r >= 0 && r < Qimg->height()
                     ) {
                 pixelNode* pn = pixelnodes[r][c];
                 if (pn->state != pixelNode::EXPANDED) {
                     double cost = q->getLinkCost(i);
                     if (pn->state == pixelNode::INITIAL) {
                         pn->setParent(q);
                         pn->totalCost = cost + q->totalCost;
                         pn->state = pixelNode::ACTIVE;
                         heap.Insert(pn);
                     } else if (pn->state == pixelNode::ACTIVE) {
                         if (cost + q->totalCost < pn -> totalCost) {
                             pn->setParent(q);
                             pixelNode newpn = pixelNode(pn->getCol(), pn->getRow());
                             newpn.totalCost = cost + q->totalCost;
                             heap.DecreaseKey(pn, newpn);
                         }
                     }
                 }
             }
         }
     }
}


// compute 8 number of link graph, which could be accessed separatelyss
void MainWindow::costgraph_init(){
    if( scissor_enabled == false){
        cout << " scissor not enabled" << endl;
        return;
    }

    //  0   1   2
    //  3   4   5
    //  6   7   8
    // Kernel[4] specify self-loop to be inf cost
    // this actualy could be ignored in the main while loop, since we only explore 8 neightbors.
    cv::Mat Kernel[9];

    Kernel[0] = (cv::Mat_<float>(3,3)  <<      0,  0.707,      0,
                                          -0.707,      0,      0,
                                               0,      0,      0  );
    Kernel[1] = (cv::Mat_<float>(3,3)<<    -0.25,      0,   0.25,
                                           -0.25,      0,   0.25,
                                               0,      0,      0  );
    Kernel[2] = (cv::Mat_<float>(3,3)<<        0,  0.707,      0,
                                               0,      0, -0.707,
                                               0,      0,      0  );
    Kernel[3] = (cv::Mat_<float>(3,3)<<     0.25,   0.25,      0,
                                               0,      0,      0,
                                           -0.25,  -0.25,      0  );
    Kernel[4] = (cv::Mat_<float>(3,3)<<        0,      0,      0,
                                               0, 100000,      0,
                                               0,      0,      0  );
    Kernel[5] = (cv::Mat_<float>(3,3)<<        0,  -0.25,  -0.25,
                                               0,      0,      0,
                                               0,   0.25,   0.25  );
    Kernel[6] = (cv::Mat_<float>(3,3)<<        0,      0,      0,
                                          -0.707,      0,      0,
                                               0,  0.707,      0  );
    Kernel[7] = (cv::Mat_<float>(3,3)<<        0,      0,      0,
                                           -0.25,      0,  -0.25,
                                            0.25,      0,  -0.25  );
    Kernel[8] = (cv::Mat_<float>(3,3)<<        0,      0,      0,
                                               0,      0,  0.707,
                                               0, -0.707,      0  );

    cv::Mat temp;
    float maxD = -1.0;

    // compute 8 direction cost graphs separately
    for(int i=0; i<=8; i++){
        cv::filter2D(image, temp, CV_32FC3, Kernel[i]);

        vector<cv::Mat> rgbChannels(3);
        cv::split(temp, rgbChannels);

        cv::Mat tmp = cv::Mat::zeros(image.rows,image.cols,CV_32FC1);

        cv::Mat B = rgbChannels[0];
        cv::Mat R = rgbChannels[0];
        cv::Mat G = rgbChannels[0];

        B = 255 - B;
        R = 255 - R;
        G = 255 - G;

        // elementwise multiplication
        tmp = B.mul(B) + R.mul(R) + G.mul(G);
        tmp.copyTo(costgraph_weight[i]);

        // cout << "tmp is: \n\n\n\n\n\n\n" << tmp << endl;
        // cv::imshow("test", tmp);
    }

    for (int i = 0; i < costgraph_weight->rows; i ++) {
        for (int j = 0; j < costgraph_weight->cols; j ++) {
            if (costgraph_weight->at<float>(cv::Point(i,j)) > maxD) {
                maxD = costgraph_weight->at<float>(cv::Point(i,j));
            }
        }
    }


    for (int i = 0; i < costgraph_weight->rows; i ++) {
        for (int j = 0; j < costgraph_weight->cols; j ++) {
            cout << costgraph_weight->at<float>(cv::Point(i,j)) << endl;
            costgraph_weight->at<float>(cv::Point(i,j)) = maxD - costgraph_weight->at<float>(cv::Point(i,j));
            cout << costgraph_weight->at<float>(cv::Point(i,j)) << endl;
        }
    }

}


// main algorithm
// find shortest path from current click seed
void MainWindow::Dijstras(pixelNode* seed){

    // initialize the priority queue pq to be empty;
    priority_queue<pixelNode*, std::vector<pixelNode*>, compareQueue> pqueue;

    // initialize each node to the INITIAL state;
    visitedMap = cv::Mat::zeros(image.size(), CV_8UC1);
    parentMap = cv::Mat::zeros(image.size(), CV_8UC1);
    //activeMap = cv::Mat::zeros(image.size(), CV_8UC1);

    // set the total cost of seed to be zero and make seed the root of the minimum path tree ( pointing to NULL ) ;
    // parentMap stores the parent of each pixel, 255: root, others listed below

    graphCost = cv::Mat::ones(image.size(), CV_32F);
    graphCost *= 100000000.0;
    graphCost.at<float>(cv::Point(seed->getCol(),seed->getRow())) = 0.0;
    parentMap.at<uchar>(cv::Point(seed->getCol(),seed->getRow())) = 255;

    // insert seed into pq;
    pqueue.push(seed);

    // init
    cv::Point r;
    int costgraph_index;
    cv::Mat costgraph_tmp;


    // while pq is not empty
    while(!pqueue.empty()){

        // extract the node q with the minimum total cost in pq;
        pixelNode* q = pqueue.top();
        pqueue.pop();

        while (visitedMap.at<uchar>(cv::Point(q->getCol(), q->getRow())) == 1) {
            q = pqueue.top();
            pqueue.pop();
            if (pqueue.empty()){
                cout << "every node explored" << endl;
                break;
            }
        }

        // mark q as EXPANDED;
        visitedMap.at<uchar>(cv::Point(q->getCol(),q->getRow())) = 1;


        // for each neighbor node r of q
        for(int i=-1; i<=1; i++){

            for(int j=-1; j<=1; j++){

                if( i==0 && j==0 )
                    continue;

                // note the i, j
                r = cv::Point( q->getCol()+j, q->getRow()+i );

                // if r has been EXPANDED
                if (visitedMap.at<uchar>(r) == 1)
                    continue;

                costgraph_index = (i+1)*3 + (j+1);
                costgraph_tmp = costgraph_weight[costgraph_index];

                float oldCost = graphCost.at<float>(r);
                float newCost = graphCost.at<float>(cv::Point(q->getCol(),q->getRow()))
                                + costgraph_tmp.at<float>(cv::Point(q->getCol(),q->getRow()));

                // cout << "newCost is: " << newCost << endl;

                if (newCost < oldCost){
                    // cout << "update" << endl;

                    graphCost.at<float>(r) = newCost;

                    parentMap.at<uchar>(r) = (1-i)*3 + (1-j);

                    // check boundary
                    if ( (q->getCol()+j) > 0 && (q->getRow()+i) > 0 &&
                         (q->getCol()+j) < (image.cols-1) && (q->getRow()+i) < (image.rows-1)) {
                        pqueue.push(new pixelNode(r.x, r.y) );
                    }
                }
            }
        } // end of neightbor search loop

        // satety check
        if (pqueue.empty()){
            cout << "every node explored" << endl;
            break;
        }

    } // end of while loop

}


// Gaussian filter Smooth
void MainWindow::on_actionGuassian_3_triggered(bool checked){

    if (!image.empty() && checked){
        cv::GaussianBlur(image, image, cv::Size(3, 3), 3, 3);
        contour_image = image.clone();
        display_image(image);
    }
}

void MainWindow::on_actionGaussian_5_triggered(bool checked){

    if (!image.empty() && checked){
        cv::GaussianBlur(image, image, cv::Size(5, 5), 5, 5);
        contour_image = image.clone();
        display_image(image);
    }
}
