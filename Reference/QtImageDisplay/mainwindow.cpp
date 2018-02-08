#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qApp->installEventFilter(this);
    ui->setupUi(this);

    const int maxSize = 200;
    currentSize=0;
    allClicksX = new int[maxSize];
    allClicksY = new int[maxSize];
    for( int i=0 ; i<maxSize ; i++ )
    {
        allClicksX[i] = 0;
        allClicksY[i] = 0;
    }

    //allocate array of Mats weights
    graphWeights = new cv::Mat[9];


    scaleFactor=1;
    overlayPathActive = true;
    ui->actionOverlay_Active->setChecked(true);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete [] allClicksX;
    delete [] allClicksY;
}


QImage MainWindow::cvMat2QImage(cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(mat.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        //qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), QDir::currentPath(), tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));

    im = cv::imread( qPrintable(fileName),CV_LOAD_IMAGE_COLOR );

    QImage qim = cvMat2QImage( im );
    qim = qim.scaledToHeight(im.rows*scaleFactor);

    ui->label->setPixmap(QPixmap::fromImage(qim));

    computeGraphWeights();

}

void MainWindow::computeGraphWeights()
{
    // Store weights for each neighboour in different Mat. Size is same as image height
    //  K[i] are computed as filtering operations for sake for speedy computations.
    //  note that doing this yourself is too slow. Storage output is of file float CV_32F

    if( scissorActive == false )
    {
        std::cout<<"Scissor tool not active\n";
        return;
    }

    // [  0   1   2  ]
    // [  3   4   5  ]
    // [  6   7   8  ]

    cv::Mat K[9];
/*
    //My Intuition
    K[0] = (cv::Mat_<float>(3,3)<<-1.41,0,0,  0,1.41,0,  0,0,0  );
    K[1] = (cv::Mat_<float>(3,3)<<0,-1,0,  0,1,0,  0,0,0  );
    K[2] = (cv::Mat_<float>(3,3)<<0,0,-1.41,  0,1.41,0,  0,0,0  ); //sqrt(2)*diff
    K[3] = (cv::Mat_<float>(3,3)<<0,0,0,  -1,1,0,  0,0,0  );
    K[4] = (cv::Mat_<float>(3,3)<<0,0,0,  0,100000,0,  0,0,0  );        //to say that self-loops have inf cost
    K[5] = (cv::Mat_<float>(3,3)<<0,0,0,   0,1,-1,  0,0,0  );
    K[6] = (cv::Mat_<float>(3,3)<<0,0,0,  0,1.41,0,  -1.41,0,0  );
    K[7] = (cv::Mat_<float>(3,3)<<0,0,0,  0,1,0,  0,-1,0  );
    K[8] = (cv::Mat_<float>(3,3)<<0,0,0,  0,1.41,0,  0,0,-1.41  );
*/



    //as described in course document
    K[0] = (cv::Mat_<float>(3,3)<<   0,    .7,        0,
                                      -.7,        0,     0,
                                      0,        0,      0  );
    K[1] = (cv::Mat_<float>(3,3)<<  -.25,0,.25,
                                    -.25,0,.25,
                                    0,0,0  );
    K[2] = (cv::Mat_<float>(3,3)<<  0,      .7,     0,
                                    0,       0,     -.7,
                                    0,      0,      0  ); //sqrt(2)*diff
    K[3] = (cv::Mat_<float>(3,3)<<   .25,.25,0,
                                     0,0,0,
                                     -.25,-.25,0  );
    K[4] = (cv::Mat_<float>(3,3)<<  0,0,0,
                                    0,100000,0,
                                    0,0,0  );        //to say that self-loops have inf cost
    K[5] = (cv::Mat_<float>(3,3)<<0,-.25,-.25,
                                  0,0,0,
                                  0,.25,.25  );
    K[6] = (cv::Mat_<float>(3,3)<<  0,        0,      0,
                                    -.7,      0,    0,
                                     0,      .7,    0  );
    K[7] = (cv::Mat_<float>(3,3)<<0,0,0,
                                  -.25,0,-.25,
                                  .25,0,-.25  );
    K[8] = (cv::Mat_<float>(3,3)<<  0,  0,      0,
                                    0,   0,   .7,
                                    0,  -.7,   0  );

    cv::Mat tmp;
    char fname[100];


    std::ofstream outfile;
    for( int i=0 ; i<=8 ; i++ )
    {
        //sprintf( fname, "%d.txt",i);
        //outfile.open(fname);


        cv::filter2D(im,tmp,CV_32FC3,K[i]);
        //tmp = cv::abs(tmp) + .0001;  ///Epison added to the matrix to avoid 0 weights.
        //note that Dijstras does not work in presense of -ve or 0 edges.
        //tmp = cv::Mat::ones(im.size(),CV_32FC1);


        //now tmp is a 3-channel image. Need to combine channels like
        // R^2 + G^2 + B^2  
        cv::Mat dst = cv::Mat::zeros(im.rows,im.cols,CV_32FC1);

        float epsilon = 0 ;//1+((i+1)%2); //the '%' part is done to penalize diagonal edges.
        mergeChannels(tmp,dst,epsilon);



        dst.copyTo(graphWeights[i]);


        //outfile<<format(graphWeights[i],"csv");
        //outfile.close();
    }

    //std::ofstream outfile("new.txt");
    //outfile<<format(graphWeights[1],"csv");



}


void MainWindow::mergeChannels(cv::Mat &tmp, cv::Mat &dst, float epsilon)
{
    std::vector<cv::Mat> channels(3);
    cv::split(tmp, channels);

    cv::Mat B = channels[0];
    cv::Mat G = channels[1];
    cv::Mat R = channels[2];

    //-ve of gradient. If it is a strong edge, the difference (computed with filter2D()) is going to be
    //large, so take a complement to prefer larger differences. See lecture notes.
    B = 255-B;
    G = 255-G;
    R = 255-R;


    dst = B.mul(B) + G.mul(G) + R.mul(R) + epsilon;

}






void MainWindow::overlayPath( cv::Mat a, cv::Mat path, cv::Vec3b intensity , cv::Mat out )
{
    for( int i=0 ; i<a.rows ; i++ )
    {
        for( int j=0 ; j<a.cols ; j++ )
        {
            if( path.at<uchar>(i,j) != 0 )
                out.at<cv::Vec3b>(i,j) = intensity;
        }
    }

}



// Paints the image passed on the display area
void MainWindow::paintImage(cv::Mat a)
{
    cv::Mat b = a.clone();
    if( currentSize>1 && overlayPathActive)
    {
        overlayPath(a,path,cv::Vec3b(255,0,0),b);
        a=b;
    }

    qim = cvMat2QImage( a );
    qim = qim.scaledToHeight(im.rows*scaleFactor);


    ui->label->setPixmap(QPixmap::fromImage(qim));
}


void MainWindow::doDijstras( int clickedX, int clickedY )
{
    std::cout<<"---------------DijStras------------------\n";
    //The Dijstras algorithm with anchor node at (clickedX,clickedY).
    //   Computes the shortest paths to all the other nodes.

    //init
    // set isVisited to "not visited" for each node
    // set dijstrasCost to "inf" for each node
    isVisited = cv::Mat::zeros(im.size(),CV_8UC1);
    previous = cv::Mat::zeros(im.size(),CV_8UC1);
    dijstrasCost = cv::Mat::ones(im.size(),CV_32F);
    //parentX = cv::Mat::zeros(im.size(), CV_32SC1);
    //parentY = cv::Mat::zeros(im.size(), CV_32SC1);

    

    dijstrasCost = 1000000000000.0*dijstrasCost; //setting to "inf"


    int currX = clickedX;
    int currY = clickedY;
    dijstrasCost.at<float>(cv::Point(currX,currY)) = 0.0; //cost at root is zero
    //parentX.at<int>(cv::Point(currX,currY)) = -1;
    //parentY.at<int>(cv::Point(currX,currY)) = -1;

    previous.at<uchar>( cv::Point(currX,currY) ) = 255;
    //here we use relative addresses, ie. 0=>top-left, 1=>top, 2=>top-right and so on.
    //so the values are going to be from [0,8];. 255 denotes no parent, ie the root node


    //proprity queue
    std::priority_queue<SpecialType,std::vector<SpecialType>,mycomparison> pqueue;


    int maxitr=2;

    //while((maxitr--)>0)
    while(1)
    {


        //set current node as visited
        isVisited.at<uchar>(cv::Point(currX,currY)) = 1;

        //loop through all neighboours of currX,currY
        for( int i=-1 ; i<=1 ; i++ )
        {
            for( int j=-1 ; j<=1 ; j++ )
            {
                if( isVisited.at<uchar>(cv::Point(currX+j,currY+i)) >= 1 )
                    continue;

                //oldCost is the existing cost at the neighbour node
                float oldCost = dijstrasCost.at<float>(cv::Point(currX+j,currY+i));


                //new cost is the sum of currCost & cost at the neighbour node
                int xx = (i+1)*3 + (j+1);
                cv::Mat tmp = graphWeights[xx];


/*
            char fname[100];
            sprintf( fname, "%d.txt",xx);
            std::ofstream outfile(fname);
            outfile<<format(tmp,"csv");
*/


                float newCost = dijstrasCost.at<float>(cv::Point(currX,currY)) + tmp.at<float>(cv::Point(currX,currY));


                //if new cost is less then update the cost
                if( newCost < oldCost )
                {
                    dijstrasCost.at<float>(cv::Point(currX+j,currY+i)) = newCost;

                    //parent of ( Point(currX+j,currY+i) ) is Point( currX,currY )
                    //printf( "Parent of (%d,%d) is (%d,%d)\n",currX+j,currY+i,  currX,currY );
                    //parentX.at<int>(cv::Point(currX+j,currY+i)) = (2-i)*3 + (2-j);//currY;
                    //parentY.at<int>(cv::Point(currX+j,currY+i)) = i;//currX;

                    previous.at<uchar>(cv::Point(currX+j,currY+i)) = (1-i)*3 + (1-j);




                    //push this value into min-priority-queue.
                    //   note that due to huge number of nodes, finding the minimum simplying by comparison is not feasible.
                    if( ( isVisited.at<uchar>(cv::Point(currX+j,currY+i)) == 0 )  && 
                        ( (currX+j) > 0 ) && ( (currX+j) < im.cols-1 )  && ( currY+i > 0 )  && 
                        ( currY+i < im.rows-1)  )
                        pqueue.push(SpecialType(newCost,currX+j,currY+i));
                }


            }
        }
        //end of update neighbours loop


        //empty queue implies all the nodes have been visited
        if( pqueue.empty() == true )
        {
            std::cout<<"Everyone Visited........!\n";
            break;
        }


        //go through un-visited nodes and find the node which has minimim cost along with itz index

        //get a minimum which is not visited

        //get a minimum
        SpecialType t = pqueue.top();
        pqueue.pop();
        if( pqueue.empty() == true )
        {
            std::cout<<"Everyone Visited........!\n";
            break;
        }

        //check if it is visited
        while( isVisited.at<uchar>(cv::Point(t.theX,t.theY))==1 ) //if it is visited then ignore this and get next element in the min-queue
        {
            t=pqueue.top();
            pqueue.pop();
            //empty queue implies all the nodes have been visited
            if( pqueue.empty() == true )
            {
                std::cout<<"Everyone Visited........!\n";
                break;
            }
        }



        //update currX, currY to be minimum in the queue
        currX = t.theX;
        currY = t.theY;

        //std::cout<<"A minimum cost of "<<t.num<<" found at ("<<t.theX<<", "<<t.theY<<")\n";

    }



    //std::ofstream outfile("dijstrasCost.txt");
    //outfile<<format(dijstrasCost,"csv");


    //std::ofstream outfile1("isVisited.txt");
    //outfile1<<format(isVisited,"csv");

    /*
    std::ofstream outfile2("parentX.txt");
    outfile2<<format(parentX,"csv");
    std::ofstream outfile3("parentY.txt");
    outfile3<<format(parentY,"csv");
    */

    //std::ofstream outfile4("previous.txt");
    //outfile4<<format(previous,"csv");


}




bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{

    if( event->type() == QEvent::KeyPress )
    {
        QKeyEvent *key = static_cast<QKeyEvent *>(event);
        if((key->key() == Qt::Key_Enter) || (key->key() == Qt::Key_Return))
        {
            //Detect press of "Enter" key.
            //Enter key is to end.

            //When this happens, connect the lastClickedPoint to the 1st clicked point with a straight line
            //this is done to close the contour.


            //QMessageBox::information(this,"key press", "key press");



            cv::line(path,cv::Point( allClicksX[0], allClicksY[0] ), cv::Point( allClicksX[currentSize-1], allClicksY[currentSize-1] ), 255 );


            //then deactivate the scissor
            //scissorActive = false;



        }
    }


    //MouseClick - The 2nd condition is to ensure the event from QLabel.
    if( event->type() == QEvent::MouseButtonPress && strcmp(obj->metaObject()->className(),"MainWindow")==0 )
    {
        std::cout<<obj->metaObject()->className()<<"\n";
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QPoint P = ui->label->mapFrom(this,mouseEvent->pos());
        P /= scaleFactor;

        std::cout<<"Clicked on ("<<P.x()<<", "<<P.y()<<")\n";



        if( scissorActive == false )
        {
            std::cout<<"Scissor tool not active\n";
            return false;
        }



        if( currentSize == 0 )
        {
            path = cv::Mat::zeros( im.rows, im.cols, CV_8UC1 );
        }
        else
            registerPath(P.x(), P.y());







        double t = (double)cv::getTickCount();

        //all dijstras as soon as mouse is clicked
        doDijstras( P.x(), P.y() );

        t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
        std::cout << "Exe time for Dijstra's Algorithm: " << t << std::endl;


        allClicksX[currentSize] = P.x();
        allClicksY[currentSize] = P.y();


        currentSize++;
        //std::cout<<currentSize;

    }


    //OnMouseMove && under the image-display area
  if (event->type() == QEvent::MouseMove && ui->label->underMouse() )
  {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

    //Get current co-ordinates
    QPoint P = mouseEvent->pos();
    P /= scaleFactor;

    QString statusText;
    if( scissorActive )
        statusText = QString("Intelligent Scissor Active");

    statusBar()->showMessage(QString("(%1,%2) ").arg(P.x()).arg(P.y())  + statusText   );

    if(!im.empty() && currentSize>0 && scissorActive)
    {
        out = im.clone();
        //cv::line(out,cv::Point(allClicksX[currentSize-1],allClicksY[currentSize-1]),cv::Point(P.x(),P.y()),255);


        backTrackFrom( P.x(), P.y() );

        //cv::line(out,cv::Point(5,5),cv::Point(P.x(),P.y()),255);
        paintImage(out);
    }
  }




  return false;
}



//Whenever mouseClickEvent, register the path as a binary image
void MainWindow::registerPath(int startX, int startY)
{
    int tmp;
    while( (tmp=previous.at<uchar>(cv::Point(startX,startY))) != 255 )
    {
        //plot
        path.at<uchar>( cv::Point(startX,startY) ) = 255;

        switch(tmp)
        {
            case 0:
                startX--;
                startY--;
                continue;
            case 1:
                startY--;
                continue;
            case 2:
                startY--;
                startX++;
                continue;
            case 3:
                startX--;
                continue;
            case 4:
                std::cerr<<"Should not be occuring\n";
                exit(1);
                continue;
            case 5:
                startX++;
                continue;
            case 6:
                startX--;
                startY++;
                continue;
            case 7:
                startY++;
                continue;
            case 8:
                startX++;
                startY++;
                continue;
        }
    }
}


void MainWindow::backTrackFrom(int startX, int startY)
{
    int tmp;
    cv::Vec3b val(0,255,255);
    while( (tmp=previous.at<uchar>(cv::Point(startX,startY))) != 255 )
    {
        //plot
        out.at<cv::Vec3b>( cv::Point(startX,startY) ) = val;

        switch(tmp)
        {
            case 0:
                startX--;
                startY--;
                continue;
            case 1:
                startY--;
                continue;
            case 2:
                startY--;
                startX++;
                continue;
            case 3:
                startX--;
                continue;
            case 4:
                std::cerr<<"Should not be occuring\n";
                exit(1);
                continue;
            case 5:
                startX++;
                continue;
            case 6:
                startX--;
                startY++;
                continue;
            case 7:
                startY++;
                continue;
            case 8:
                startX++;
                startY++;
                continue;
        }
    }

    //imshow( "ddd", scrat );
    //paintImage(scrat);

}





void MainWindow::on_actionWrite_Path_triggered()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm", "Are you sure you want to write path.png?",
                                    QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
        cv::imwrite("path.png",path);
}

void MainWindow::on_actionWrite_Mask_triggered()
{
    //floodfill the path. We assume that the path is closed.
    //This function is not responsible for color-spill

    mask = path.clone();
    cv::threshold(path, mask, 1, 128, CV_THRESH_BINARY);
    cv::floodFill(mask, cv::Point(2, 2), 255, 0, 10, 10);

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm", "Are you sure you want to write mask.png?",
                                    QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
        cv::imwrite("mask.png",mask);
}


void MainWindow::on_actionScissor_triggered(bool checked)
{
    this->scissorActive = checked;
    if( this->scissorActive )
    {
        std::cout<<"Scissor Tool Activated\n";
        computeGraphWeights();
    }
    else
    {
        std::cout<<"Scissor tools disabled\n";
        out = im.clone();
        overlayPath(im,path,cv::Vec3b(255,0,0),out);
        paintImage(out);

    }
}

void MainWindow::on_actionReset_Contours_triggered()
{
    resetStateVariables();
    paintImage(im);
}



void MainWindow::resetStateVariables()
{
    std::cout<<"Reset Contours\n";

    //reset clicked points
    currentSize=0;

    //reset path, mask
    path = cv::Mat::zeros( path.size(),CV_8UC1 );
    mask = path.clone();

}



void MainWindow::on_actionQuit_triggered()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Quit", "Quit?",
                                    QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
        QCoreApplication::quit();

}

void MainWindow::on_actionAuthors_triggered()
{
    QString content = QString( "<h3>Intelligent Scissor</h3><br/> Project - 1<br/> Computer Vision Course (COMP 5421)<br/><p> <i>Manohar Kuse (mpkuse@ust.hk) <br/>Sunil Jaiswal (spjaiswal@ust.hk)" );
    QMessageBox::about(this,"About - Intelligent Scissor", content );
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::on_actionOverlay_Active_triggered(bool checked)
{
    overlayPathActive = checked;
    paintImage(out);
}

void MainWindow::on_actionZoom_In_triggered()
{
    scaleFactor += 0.1;
    paintImage(im);
}

void MainWindow::on_actionZoom_Out_triggered()
{
    scaleFactor -= 0.1;
    paintImage(im);
}

void MainWindow::on_actionHelp_triggered()
{
    QString content = QString( " \
                               <ul>\
                                   <li><strong>File --&gt; Open</strong> : Browse files and open image to mark</li>\
                                   <li><strong>File --&gt; Write Contour</strong> : Write the delineated path as a PNG image</li>\
                                   <li><strong>File --&gt; Write Mask </strong>: Write the marked mask as a PNG image</li>\
                                   <li><strong>File --&gt; Quit : Quit the program</strong></li>\
                                                                    \
                                   <li><strong>Scissor --&gt; Activate Scissor (check-able) : </strong>Activate/De-activate the intelligent scissor tool</li>\
                                   <li><strong>Scissor --&gt; Reset Contour</strong> : Reset the delineated contour.</li>\
                                   <li><strong>Scissor --&gt; Overlay Contour (check-able) </strong>: Activate/De-activate the display of delineated contours</li>\
                                                                     \
                                   <li><strong>Image --&gt; Zoom In</strong> : Zoom into the image</li>\
                                   <li><strong>Image --&gt; Zoom Out</strong> : Zoom out of the image</li>\
                                   <li><strong>Image --&gt; Blur</strong> : Blur image with 3x3 kernel</li>\
                                   <li><strong>Image --&gt; Blur</strong> : Blur image with 5x5 kernel</li>\
                                   <li><strong>Image --&gt; Gaussian Blur</strong> : Gaussian blur image</li>\
                                                                       \
                                   <li><strong>About --&gt; Help</strong> : Display usage</li>\
                                   <li><strong>About --&gt; Authors</strong> : Gives the names of the software creators</li>\
                                   <li><strong>About --&gt; Qt</strong> : Gives the version of Qt</li>\
                               </ul>\
                                " );
    QMessageBox::about(this,"Help -- Usage", content );
}

void MainWindow::on_actionBlur_3x3_triggered()
{
    if( !im.empty() )
        cv::blur( im, im, cv::Size( 3, 3 ), cv::Point(-1,-1) );
    resetStateVariables();
    paintImage(im);

}

void MainWindow::on_actionBlur_5x5_triggered()
{
    if( !im.empty() )
        cv::blur( im, im, cv::Size( 5, 5 ), cv::Point(-1,-1) );
    resetStateVariables();
    paintImage(im);
}

void MainWindow::on_actionGaussian_Blur_triggered()
{
    if( !im.empty() )
        cv::GaussianBlur( im, im, cv::Size( 3, 3 ), 4 );
    resetStateVariables();
    paintImage(im);
}

void MainWindow::on_actionGaussian_Blur_5x5_triggered()
{
    if( !im.empty() )
        cv::GaussianBlur( im, im, cv::Size( 5, 5 ), 4 );
    resetStateVariables();
    paintImage(im);
}

void MainWindow::on_actionFinish_Contour_triggered()
{
    cv::line(path,cv::Point( allClicksX[0], allClicksY[0] ), cv::Point( allClicksX[currentSize-1], allClicksY[currentSize-1] ), 255 );
}
