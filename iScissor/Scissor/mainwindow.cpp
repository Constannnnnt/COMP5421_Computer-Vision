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

    // head_node = new pixelNode();
    // idx = 0;
    ctrl_count = 0;

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
    delete current_node;
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

    scrollArea->setWidget(ui->label);
    setCentralWidget(scrollArea);
}

void MainWindow::print_node(pixelNode* n)
{
    pixelNode* p = n;
    while(p->getParent() != NULL){
        cout << p->getRow() << '\t' << p->getCol() << endl;
        p = p->getParent();
    }
}

void MainWindow::draw_contour(int x, int y){

    int draw_value;
    while( this->parentMap.at<uchar>( cv::Point(x,y) ) != 255 ){

        draw_value = this->parentMap.at<uchar>( cv::Point(x,y) );

        contour.at<uchar>( cv::Point(x,y) ) = 255;

        cout << "draw value is: " << draw_value << endl;

        // right is x (cols), down is y (rows)
        // 0, 3, 6
        // 1, 4, 7
        // 2, 5, 8
        switch (draw_value) {
        case 0:
            x += 1;
            y += 1;
            continue;
        case 1:
            x += 1;
            continue;
        case 2:
            x += 1;
            y -= 1;
            continue;
        case 3:
            y += 1;
            continue;
        case 4:
            cout << "self loop detected" << endl;
            exit(1);
            break;
        case 5:
            y -= 1;
            continue;
        case 6:
            x -= 1;
            y += 1;
            continue;
        case 7:
            x -= 1;
            continue;
        case 8:
            x -= 1;
            y -= 1;
            continue;
        //default:
        //    continue;
        }
    }
}

/* helper function ends here */

// open a image
void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("Open Image"), ".", tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
    image = cv::imread(fileName.toLatin1().data());
    contour_image = cv::imread(fileName.toLatin1().data());
    contour = cv::Mat::zeros(image.size(), CV_8UC1);

    // convert cv::Mat to QImage
    cv::cvtColor(image, image, CV_BGR2RGB);
    cv::cvtColor(contour_image, contour_image, CV_BGR2RGB);

    QImage Q_img = QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);

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
    this->print_node(current_node);

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
    display_image(contour_image);
}


void MainWindow::on_actionReset_Contour_triggered()
{
    cout << "reset contour" << endl;
    contour = cv::Mat::zeros(contour.size(), CV_8UC3);

}


void MainWindow::on_actionFinish_Contour_triggered()
{
    // close the loop
    // cv::line(contour, cv::Point(x_list[0],y_list[0]), cv::Point(x_list[list_size-1], y_list[list_size-1]), Scalar(255,0,0));
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


// overloading QT functions
void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        ctrl_enabled = true;
        if (scissor_enabled)
            ctrl_count += 1;
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
         (strcmp(watched->metaObject()->className(), "MainWindow")) == 0 )
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

        head_node = new pixelNode(p.x(), p.y(), 0);
        current_node = head_node;

        // draw a dot
        cv::circle(contour_image, cv::Point(p.x(),p.y()), 1, CV_RGB(0,0,255), 1);
        if(contour_enabled)
            display_image(contour_image);

        // compute cost graph
        costgraph_init();

        // Dijkstra algorithm
        Dijstras(head_node);

        cout << "pass Dijkstra algorithm" << endl;
        first_seed_flag = true;

    }

    // Mouse move event
    if ( (event->type() == QEvent::MouseMove) && (first_seed_flag == true) ){
        QMouseEvent* me = static_cast<QMouseEvent*> (event);
        QPoint p = ui->label->mapFrom(this, me->pos());
        p /= img_scale;

        QString myText = QString("Intelligent Scissor ");
        statusBar()->showMessage(QString("(%1, %2) ").arg(p.x()).arg(p.y()) + myText);

        cout << "debug 2" << endl;

        this->draw_contour(p.x(), p.y());
        if (contour_enabled){
            cout << "debug 3" << endl;
            display_image(contour_image);
            cout << "debug 4" << endl;
        }

    }

    // left click: follwing seeds
    if ( (event->type() == QEvent::MouseButtonPress) && (!ctrl_enabled) &&
         (strcmp(watched->metaObject()->className(), "MainWindow")) == 0)
    {
        QMouseEvent* me = static_cast<QMouseEvent*> (event);
        QPoint p = ui->label->mapFrom(this, me->pos());
        p /= img_scale;
        cout << p.x() << " " << p.y() << endl;
        if (!scissor_enabled) {
            cout << "scissor is not enabled" << endl;
            return false;
        }
        cout << "left click" << endl;

        pixelNode* nod = new pixelNode(p.x(), p.y(), 100000);
        nod->setParent(current_node);
        current_node = nod;

        /*test positions in the image*/
        // QImage Q_img = QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
        // cout << "pixel at the pos" << Q_img.pixel(p.x(),p.y()) <<  endl;
        // draw the path based on the movement of the mouse
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


// compute 8 number of link graph, which could be accessed separatelyss
void MainWindow::costgraph_init(){
    if( scissor_enabled == false){
        cout << " scissor not enabled" << endl;
        return;
    }

    // sequence number follow the website notation
    // Kernel[8] specify self-loop to be inf cost
    // this actualy could be ignored in the main while loop, since we only explore 8 neightbors.
    cv::Mat Kernel[9];

    Kernel[0] = ( cv::Mat_<float>(3,3) <<  0,   0.25,   0.25,
                                           0,      0,      0,
                                           0,  -0.25,  -0.25 );

    Kernel[1] = ( cv::Mat_<float>(3,3) <<  0,  0.707,      0,
                                           0,      0, -0.707,
                                           0,      0,      0 );

    Kernel[2] = ( cv::Mat_<float>(3,3) <<  0.25,   0,  -0.25,
                                           0.25,   0,  -0.25,
                                              0,   0,      0 );

    Kernel[3] = ( cv::Mat_<float>(3,3) <<  0,  0.707,      0,
                                      -0.707,      0,      0,
                                           0,      0,      0 );

    Kernel[4] = ( cv::Mat_<float>(3,3) <<   0.25,  0.25,   0,
                                               0,     0,   0,
                                           -0.25, -0.25,   0 );

    Kernel[5] = ( cv::Mat_<float>(3,3) <<  0,      0,      0,
                                       0.707,      0,      0,
                                           0, -0.707,      0 );

    Kernel[6] = ( cv::Mat_<float>(3,3) <<     0,   0,      0,
                                           0.25,   0,  -0.25,
                                           0.25,   0,  -0.25 );

    Kernel[7] = ( cv::Mat_<float>(3,3) <<  0,      0,      0,
                                           0,      0,  0.707,
                                           0, -0.707,      0 );

    Kernel[8] = ( cv::Mat_<float>(3,3) <<  0,      0,     0,
                                           0, 100000,     0,
                                           0,      0,     0 );

    cv::Mat temp;

    // compute 8 direction cost graphs separately
    for(int i=0; i<=8; i++){
        cv::filter2D(image, temp, CV_32FC3, Kernel[i]);

        vector<cv::Mat> rgbChannels(3);
        cv::split(temp, rgbChannels);

        cv::Mat B = rgbChannels[0];
        cv::Mat R = rgbChannels[0];
        cv::Mat G = rgbChannels[0];

        B = 255 - B;
        R = 255 - R;
        G = 255 - G;

        // elementwise multiplication
        costgraph_weight[i] = ( B.mul(B) + R.mul(R) + G.mul(G) )/3;
    }


}


// main algorithm
// find shortest path from current click seed
void MainWindow::Dijstras(pixelNode* seed){

    // Debug
    // cout << "seed row " << seed->getRow() << endl;
    // cout << "seed col " << seed->getCol() << endl;

    // initialize the priority queue pq to be empty;
    priority_queue<pixelNode*, std::vector<pixelNode*>, compareQueue> pqueue;

    // initialize each node to the INITIAL state;
    visitedMap = cv::Mat::zeros(image.size(), CV_8UC1);
    parentMap = cv::Mat::zeros(image.size(), CV_8UC1);
    //activeMap = cv::Mat::zeros(image.size(), CV_8UC1);

    // set the total cost of seed to be zero and make seed the root of the minimum path tree ( pointing to NULL ) ;
    // parentMap stores the parent of each pixel, 255: root, others listed below
    // 0, 3, 6
    // 1, 4, 7
    // 2, 5, 8
    graphCost = cv::Mat::ones(image.size(), CV_32FC1);
    graphCost *= 10000000;
    graphCost.at<float>(cv::Point(seed->getRow(),seed->getCol())) = 0;
    parentMap.at<uchar>(cv::Point(seed->getRow(),seed->getCol())) = 255;

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

        // mark q as EXPANDED;
        visitedMap.at<uchar>(cv::Point(q->getRow(),q->getCol())) = 1;


        // for each neighbor node r of q
        for(int i=-1; i<=1; i++){

            for(int j=-1; j<=1; j++){

                r = cv::Point(q->getRow()+i,q->getCol()+j);

                // if r has been EXPANDED
                if (visitedMap.at<uchar>(r) == 1)
                    continue;

                costgraph_index = (i+1)*3 + (j+1);
                costgraph_tmp = costgraph_weight[costgraph_index];

                float oldCost = graphCost.at<float>(r);
                float newCost = graphCost.at<float>(cv::Point(q->getRow(),q->getCol()))
                                + costgraph_tmp.at<float>(cv::Point(q->getRow(),q->getCol()));

                if (newCost < oldCost){
                    graphCost.at<float>(r) = newCost;

                    parentMap.at<uchar>(r) = costgraph_index;

                    // check boundary
                    if ( (q->getRow()+i) > 1 && (q->getCol()+j) > 1 &&
                         (q->getRow()+i) < (image.rows-1) && (q->getCol()+j) < (image.cols-1)) {
                        pqueue.push(new pixelNode(r.x, r.y, graphCost.at<float>(r)) );
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







