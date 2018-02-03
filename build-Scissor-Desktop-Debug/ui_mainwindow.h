/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QScrollArea>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionSave_Contour;
    QAction *actionSave_Mask;
    QAction *actionExit;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QAction *actionHelp;
    QAction *actionScissor;
    QAction *actionDisplay_Contour;
    QAction *actionReset_Contour;
    QAction *actionPixel_Node;
    QAction *actionCost_Graph;
    QAction *actionPath_Tree;
    QAction *actionMin_Path;
    QAction *actionFinish_Contour;
    QWidget *centralWidget;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QLabel *label;
    QMenuBar *menuBar;
    QMenu *menufile;
    QMenu *menuScissor;
    QMenu *menuTool;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(892, 612);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave_Contour = new QAction(MainWindow);
        actionSave_Contour->setObjectName(QString::fromUtf8("actionSave_Contour"));
        actionSave_Mask = new QAction(MainWindow);
        actionSave_Mask->setObjectName(QString::fromUtf8("actionSave_Mask"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionZoom_In = new QAction(MainWindow);
        actionZoom_In->setObjectName(QString::fromUtf8("actionZoom_In"));
        actionZoom_Out = new QAction(MainWindow);
        actionZoom_Out->setObjectName(QString::fromUtf8("actionZoom_Out"));
        actionHelp = new QAction(MainWindow);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionScissor = new QAction(MainWindow);
        actionScissor->setObjectName(QString::fromUtf8("actionScissor"));
        actionScissor->setCheckable(true);
        actionDisplay_Contour = new QAction(MainWindow);
        actionDisplay_Contour->setObjectName(QString::fromUtf8("actionDisplay_Contour"));
        actionDisplay_Contour->setCheckable(true);
        actionReset_Contour = new QAction(MainWindow);
        actionReset_Contour->setObjectName(QString::fromUtf8("actionReset_Contour"));
        actionPixel_Node = new QAction(MainWindow);
        actionPixel_Node->setObjectName(QString::fromUtf8("actionPixel_Node"));
        actionCost_Graph = new QAction(MainWindow);
        actionCost_Graph->setObjectName(QString::fromUtf8("actionCost_Graph"));
        actionPath_Tree = new QAction(MainWindow);
        actionPath_Tree->setObjectName(QString::fromUtf8("actionPath_Tree"));
        actionMin_Path = new QAction(MainWindow);
        actionMin_Path->setObjectName(QString::fromUtf8("actionMin_Path"));
        actionFinish_Contour = new QAction(MainWindow);
        actionFinish_Contour->setObjectName(QString::fromUtf8("actionFinish_Contour"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(0, 0, 881, 551));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 879, 549));
        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 0, 461, 281));
        scrollArea->setWidget(scrollAreaWidgetContents);
        MainWindow->setCentralWidget(centralWidget);
        scrollArea->raise();
        label->raise();
        label->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 892, 25));
        menufile = new QMenu(menuBar);
        menufile->setObjectName(QString::fromUtf8("menufile"));
        menuScissor = new QMenu(menuBar);
        menuScissor->setObjectName(QString::fromUtf8("menuScissor"));
        menuTool = new QMenu(menuBar);
        menuTool->setObjectName(QString::fromUtf8("menuTool"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        MainWindow->insertToolBarBreak(mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menufile->menuAction());
        menuBar->addAction(menuScissor->menuAction());
        menuBar->addAction(menuTool->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menufile->addAction(actionOpen);
        menufile->addAction(actionSave_Contour);
        menufile->addAction(actionSave_Mask);
        menufile->addAction(actionExit);
        menuScissor->addAction(actionScissor);
        menuScissor->addAction(actionDisplay_Contour);
        menuScissor->addAction(actionReset_Contour);
        menuScissor->addAction(actionFinish_Contour);
        menuScissor->addSeparator();
        menuScissor->addAction(actionPixel_Node);
        menuScissor->addAction(actionCost_Graph);
        menuScissor->addAction(actionPath_Tree);
        menuScissor->addAction(actionMin_Path);
        menuTool->addAction(actionZoom_In);
        menuTool->addAction(actionZoom_Out);
        menuHelp->addAction(actionHelp);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0, QApplication::UnicodeUTF8));
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionSave_Contour->setText(QApplication::translate("MainWindow", "Save Contour", 0, QApplication::UnicodeUTF8));
        actionSave_Mask->setText(QApplication::translate("MainWindow", "Save Mask", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionZoom_In->setText(QApplication::translate("MainWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
        actionZoom_In->setShortcut(QApplication::translate("MainWindow", "Ctrl+=", 0, QApplication::UnicodeUTF8));
        actionZoom_Out->setText(QApplication::translate("MainWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
        actionZoom_Out->setShortcut(QApplication::translate("MainWindow", "Ctrl+-", 0, QApplication::UnicodeUTF8));
        actionHelp->setText(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
        actionScissor->setText(QApplication::translate("MainWindow", "Scissor", 0, QApplication::UnicodeUTF8));
        actionDisplay_Contour->setText(QApplication::translate("MainWindow", "Display Contour", 0, QApplication::UnicodeUTF8));
        actionReset_Contour->setText(QApplication::translate("MainWindow", "Reset Contour", 0, QApplication::UnicodeUTF8));
        actionPixel_Node->setText(QApplication::translate("MainWindow", "Pixel Node", 0, QApplication::UnicodeUTF8));
        actionCost_Graph->setText(QApplication::translate("MainWindow", "Cost Graph", 0, QApplication::UnicodeUTF8));
        actionPath_Tree->setText(QApplication::translate("MainWindow", "Path Tree", 0, QApplication::UnicodeUTF8));
        actionMin_Path->setText(QApplication::translate("MainWindow", "Min Path", 0, QApplication::UnicodeUTF8));
        actionFinish_Contour->setText(QApplication::translate("MainWindow", "Finish Contour", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        menufile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuScissor->setTitle(QApplication::translate("MainWindow", "Scissor", 0, QApplication::UnicodeUTF8));
        menuTool->setTitle(QApplication::translate("MainWindow", "Tool", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
