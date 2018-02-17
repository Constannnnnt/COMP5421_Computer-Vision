#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);

    MainWindow w;
    w.show();
    
    return a.exec();
}
