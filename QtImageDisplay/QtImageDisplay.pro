#-------------------------------------------------
#
# Project created by QtCreator 2014-02-12T22:08:49
#
#-------------------------------------------------

QT       += core gui

TARGET = QtImageDisplay
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib \
-lopencv_core \
-lopencv_imgproc \
-lopencv_highgui \
#-lopencv_ml \
#-lopencv_video \
#-lopencv_features2d \
#-lopencv_calib3d \
#-lopencv_objdetect \
#-lopencv_contrib \
#-lopencv_legacy \
#-lopencv_flann

#QMAKE_LFLAGS += -static-libgcc
#CONFIG += static

