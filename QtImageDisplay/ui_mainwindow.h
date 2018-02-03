/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Wed Feb 26 11:41:12 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QScrollArea>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionWrite_Path;
    QAction *actionWrite_Mask;
    QAction *actionScissor;
    QAction *actionReset_Contours;
    QAction *actionQuit;
    QAction *actionAuthors;
    QAction *actionAbout_Qt;
    QAction *actionOverlay_Active;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QAction *actionHelp;
    QAction *actionBlur_3x3;
    QAction *actionBlur_5x5;
    QAction *actionGaussian_Blur;
    QAction *actionGaussian_Blur_5x5;
    QAction *actionFinish_Contour;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuTools;
    QMenu *menuAbout;
    QMenu *menuImage;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(865, 699);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionWrite_Path = new QAction(MainWindow);
        actionWrite_Path->setObjectName(QString::fromUtf8("actionWrite_Path"));
        actionWrite_Mask = new QAction(MainWindow);
        actionWrite_Mask->setObjectName(QString::fromUtf8("actionWrite_Mask"));
        actionScissor = new QAction(MainWindow);
        actionScissor->setObjectName(QString::fromUtf8("actionScissor"));
        actionScissor->setCheckable(true);
        actionReset_Contours = new QAction(MainWindow);
        actionReset_Contours->setObjectName(QString::fromUtf8("actionReset_Contours"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionAuthors = new QAction(MainWindow);
        actionAuthors->setObjectName(QString::fromUtf8("actionAuthors"));
        actionAbout_Qt = new QAction(MainWindow);
        actionAbout_Qt->setObjectName(QString::fromUtf8("actionAbout_Qt"));
        actionOverlay_Active = new QAction(MainWindow);
        actionOverlay_Active->setObjectName(QString::fromUtf8("actionOverlay_Active"));
        actionOverlay_Active->setCheckable(true);
        actionZoom_In = new QAction(MainWindow);
        actionZoom_In->setObjectName(QString::fromUtf8("actionZoom_In"));
        actionZoom_Out = new QAction(MainWindow);
        actionZoom_Out->setObjectName(QString::fromUtf8("actionZoom_Out"));
        actionHelp = new QAction(MainWindow);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionBlur_3x3 = new QAction(MainWindow);
        actionBlur_3x3->setObjectName(QString::fromUtf8("actionBlur_3x3"));
        actionBlur_5x5 = new QAction(MainWindow);
        actionBlur_5x5->setObjectName(QString::fromUtf8("actionBlur_5x5"));
        actionGaussian_Blur = new QAction(MainWindow);
        actionGaussian_Blur->setObjectName(QString::fromUtf8("actionGaussian_Blur"));
        actionGaussian_Blur_5x5 = new QAction(MainWindow);
        actionGaussian_Blur_5x5->setObjectName(QString::fromUtf8("actionGaussian_Blur_5x5"));
        actionFinish_Contour = new QAction(MainWindow);
        actionFinish_Contour->setObjectName(QString::fromUtf8("actionFinish_Contour"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 845, 632));
        horizontalLayout = new QHBoxLayout(scrollAreaWidgetContents);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        horizontalLayout->addWidget(label);

        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout_2->addWidget(scrollArea);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 865, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        menuImage = new QMenu(menuBar);
        menuImage->setObjectName(QString::fromUtf8("menuImage"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuImage->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionWrite_Path);
        menuFile->addAction(actionWrite_Mask);
        menuFile->addAction(actionQuit);
        menuTools->addAction(actionScissor);
        menuTools->addAction(actionReset_Contours);
        menuTools->addAction(actionOverlay_Active);
        menuTools->addAction(actionFinish_Contour);
        menuAbout->addAction(actionHelp);
        menuAbout->addAction(actionAuthors);
        menuAbout->addAction(actionAbout_Qt);
        menuImage->addAction(actionZoom_In);
        menuImage->addAction(actionZoom_Out);
        menuImage->addSeparator();
        menuImage->addAction(actionBlur_3x3);
        menuImage->addAction(actionBlur_5x5);
        menuImage->addAction(actionGaussian_Blur);
        menuImage->addAction(actionGaussian_Blur_5x5);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Intelligent Scissor", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0, QApplication::UnicodeUTF8));
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionWrite_Path->setText(QApplication::translate("MainWindow", "Write Contour", 0, QApplication::UnicodeUTF8));
        actionWrite_Path->setShortcut(QApplication::translate("MainWindow", "Ctrl+P", 0, QApplication::UnicodeUTF8));
        actionWrite_Mask->setText(QApplication::translate("MainWindow", "Write Mask", 0, QApplication::UnicodeUTF8));
        actionWrite_Mask->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionScissor->setText(QApplication::translate("MainWindow", "Activate Scissor", 0, QApplication::UnicodeUTF8));
        actionScissor->setShortcut(QApplication::translate("MainWindow", "Ctrl+A", 0, QApplication::UnicodeUTF8));
        actionReset_Contours->setText(QApplication::translate("MainWindow", "Reset Contours", 0, QApplication::UnicodeUTF8));
        actionReset_Contours->setShortcut(QApplication::translate("MainWindow", "Ctrl+R", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionAuthors->setText(QApplication::translate("MainWindow", "Authors", 0, QApplication::UnicodeUTF8));
        actionAbout_Qt->setText(QApplication::translate("MainWindow", "About Qt", 0, QApplication::UnicodeUTF8));
        actionOverlay_Active->setText(QApplication::translate("MainWindow", "Overlay Contour", 0, QApplication::UnicodeUTF8));
        actionOverlay_Active->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        actionZoom_In->setText(QApplication::translate("MainWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
        actionZoom_In->setShortcut(QApplication::translate("MainWindow", "Ctrl++", 0, QApplication::UnicodeUTF8));
        actionZoom_Out->setText(QApplication::translate("MainWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
        actionZoom_Out->setShortcut(QApplication::translate("MainWindow", "Ctrl+-", 0, QApplication::UnicodeUTF8));
        actionHelp->setText(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
        actionBlur_3x3->setText(QApplication::translate("MainWindow", "Blur (3x3)", 0, QApplication::UnicodeUTF8));
        actionBlur_5x5->setText(QApplication::translate("MainWindow", "Blur (5x5)", 0, QApplication::UnicodeUTF8));
        actionGaussian_Blur->setText(QApplication::translate("MainWindow", "Gaussian Blur (3x3)", 0, QApplication::UnicodeUTF8));
        actionGaussian_Blur_5x5->setText(QApplication::translate("MainWindow", "Gaussian Blur (5x5)", 0, QApplication::UnicodeUTF8));
        actionFinish_Contour->setText(QApplication::translate("MainWindow", "Finish Contour", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("MainWindow", "Scissor", 0, QApplication::UnicodeUTF8));
        menuAbout->setTitle(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
        menuImage->setTitle(QApplication::translate("MainWindow", "Image", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
