#include <iostream>
#include <iomanip>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

#include "dcmtk/config/osconfig.h"
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>

#include "mainwindow.h"
#include <QApplication>


#include "segment.h"

int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    MainWindow w;
    w.setWindowTitle("Brain Segmentation");
    w.show();

    return a.exec();


}




