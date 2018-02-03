#-------------------------------------------------
#
# Project created by QtCreator 2018-02-02T16:42:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Scissor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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


# User Setting for Jixin
# INCLUDEPATH += /usr/include/opencv2

# LIBS += /usr/lib/x86_64-linux-gnu/libopencv_highgui.so
# LIBS += /usr/lib/x86_64-linux-gnu/libopencv_core.so
# LIBS += /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so


SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui
