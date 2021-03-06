#-------------------------------------------------
#
# Project created by QtCreator 2017-04-18T13:53:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = segmentation
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++1z
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui




#OpenCV include files.
INCLUDEPATH += "C:/Program Files/DCMTK/include"
INCLUDEPATH += "E:/OpenCV2/opencv/buildnonpy/install/include"

#OpenCV lib files.
LIBS +=  \
        "E:\OpenCV2\opencv\buildnonpy\install\x64\vc12\lib\opencv_calib3d2411d.lib " \
        "E:\OpenCV2\opencv\buildnonpy\install\x64\vc12\lib\opencv_contrib2411d.lib " \
        "E:\OpenCV2\opencv\buildnonpy\install\x64\vc12\lib\opencv_core2411d.lib " \
        "E:\OpenCV2\opencv\buildnonpy\install\x64\vc12\lib\opencv_features2d2411d.lib " \
        "E:\OpenCV2\opencv\buildnonpy\install\x64\vc12\lib\opencv_flann2411d.lib " \
        "E:\OpenCV2\opencv\buildnonpy\install\x64\vc12\lib\opencv_gpu2411d.lib " \
        "E:\OpenCV2\opencv\buildnonpy\install\x64\vc12\lib\opencv_highgui2411d.lib " \
        "E:\OpenCV2\opencv\buildnonpy\install\x64\vc12\lib\opencv_imgproc2411d.lib " \
        "E:\OpenCV2\opencv\buildnonpy\install\x64\vc12\lib\opencv_legacy2411d.lib " \
        "E:\OpenCV2\opencv\buildnonpy\install\x64\vc12\lib\opencv_ml2411d.lib " \
        "E:\OpenCV2\opencv\buildnonpy\install\x64\vc12\lib\opencv_nonfree2411d.lib " \
        "E:\OpenCV2\opencv\buildnonpy\install\x64\vc12\lib\opencv_objdetect2411d.lib " \
        "E:\OpenCV2\opencv\buildnonpy\install\x64\vc12\lib\opencv_ocl2411d.lib " \
        "E:\OpenCV2\opencv\buildnonpy\install\x64\vc12\lib\opencv_photo2411d.lib " \
        "E:\OpenCV2\opencv\buildnonpy\install\x64\vc12\lib\opencv_stitching2411d.lib " \
        "E:\OpenCV2\opencv\buildnonpy\install\x64\vc12\lib\opencv_superres2411d.lib " \
        "E:\OpenCV2\opencv\buildnonpy\install\x64\vc12\lib\opencv_ts2411d.lib " \
        "E:\OpenCV2\opencv\buildnonpy\install\x64\vc12\lib\opencv_video2411d.lib " \
        "E:\OpenCV2\opencv\buildnonpy\install\x64\vc12\lib\opencv_videostab2411d.lib " \


#OpenCV lib files.
LIBS += ws2_32.lib \
        netapi32.lib \
        wsock32.lib \
        advapi32.lib\


LIBS += -L"C:\Program Files\DCMTK\lib"  \
        "C:\Program Files\DCMTK\lib\charls.lib  " \
        "C:\Program Files\DCMTK\lib\cmr.lib     " \
        "C:\Program Files\DCMTK\lib\dcmdata.lib " \
        "C:\Program Files\DCMTK\lib\dcmdsig.lib " \
        "C:\Program Files\DCMTK\lib\dcmfg.lib   " \
        "C:\Program Files\DCMTK\lib\dcmimage.lib" \
        "C:\Program Files\DCMTK\lib\dcmimgle.lib" \
        "C:\Program Files\DCMTK\lib\dcmiod.lib  " \
        "C:\Program Files\DCMTK\lib\dcmjpeg.lib " \
        "C:\Program Files\DCMTK\lib\dcmjpls.lib " \
        "C:\Program Files\DCMTK\lib\dcmnet.lib  " \
        "C:\Program Files\DCMTK\lib\dcmpmap.lib " \
        "C:\Program Files\DCMTK\lib\dcmpstat.lib" \
        "C:\Program Files\DCMTK\lib\dcmqrdb.lib " \
        "C:\Program Files\DCMTK\lib\dcmrt.lib   " \
        "C:\Program Files\DCMTK\lib\dcmseg.lib  " \
        "C:\Program Files\DCMTK\lib\dcmsr.lib   " \
        "C:\Program Files\DCMTK\lib\dcmtls.lib  " \
        "C:\Program Files\DCMTK\lib\dcmtract.lib" \
        "C:\Program Files\DCMTK\lib\dcmwlm.lib  " \
        "C:\Program Files\DCMTK\lib\i2d.lib     " \
        "C:\Program Files\DCMTK\lib\ijg12.lib   " \
        "C:\Program Files\DCMTK\lib\ijg16.lib   " \
        "C:\Program Files\DCMTK\lib\ijg8.lib   " \
        "C:\Program Files\DCMTK\lib\oflog.lib   " \
        "C:\Program Files\DCMTK\lib\ofstd.lib   " \
