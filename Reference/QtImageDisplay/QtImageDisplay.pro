#-------------------------------------------------
#
# Project created by QtCreator 2014-02-12T22:08:49
#
#-------------------------------------------------

#QT       += core gui

#TARGET = QtImageDisplay
#TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtImageDisplay
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# The following lines tells Qmake to use pkg-config for opencv
INCLUDEPATH += /usr/local/Cellar/opencv/3.4.0_1/include
LIBS += -L/usr/local/Cellar/opencv/3.4.0_1/lib -lopencv_stitching \
        -lopencv_superres -lopencv_videostab -lopencv_aruco -lopencv_bgsegm \
        -lopencv_bioinspired -lopencv_ccalib -lopencv_dpm -lopencv_face -lopencv_photo \
        -lopencv_fuzzy -lopencv_img_hash -lopencv_line_descriptor -lopencv_optflow \
        -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_stereo -lopencv_structured_light \
        -lopencv_phase_unwrapping -lopencv_surface_matching -lopencv_tracking -lopencv_datasets \
        -lopencv_text -lopencv_dnn -lopencv_plot -lopencv_xfeatures2d -lopencv_shape \
        -lopencv_video -lopencv_ml -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d \
        -lopencv_highgui -lopencv_videoio -lopencv_flann -lopencv_xobjdetect -lopencv_imgcodecs \
        -lopencv_objdetect -lopencv_xphoto -lopencv_imgproc -lopencv_core


#QMAKE_CXXFLAGS+= -fopenmp
#QMAKE_LFLAGS +=  -fopenmp

#INCLUDEPATH += /usr/local/include/opencv
#LIBS += -L/usr/local/lib \
#-lopencv_core \
#-lopencv_imgproc \
#-lopencv_highgui \
##-lopencv_ml \
##-lopencv_video \
##-lopencv_features2d \
##-lopencv_calib3d \
##-lopencv_objdetect \
##-lopencv_contrib \
##-lopencv_legacy \
##-lopencv_flann

#QMAKE_LFLAGS += -static-libgcc
#CONFIG += static

