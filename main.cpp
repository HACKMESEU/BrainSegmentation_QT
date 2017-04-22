#include <iostream>
#include <iomanip>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

#include "dcmtk/config/osconfig.h"
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>


Mat Segmentation(Mat src);
void fillHole(const Mat srcBw, Mat &dstBw);
void findMaxConnection(const Mat srcBw, Mat &dstBw);

int main()
{
    DicomImage *image = new DicomImage("C:/Users/dell/Desktop/BrainSegmentation/S3010/I20");
//    if(image != NULL)
//    {
//        auto height=image->getHeight();
//        auto width=image->getWidth();
//        std::cout<<image->getHeight()<<" "<<image->getWidth()<<std::endl;
//        auto isChom=image->isMonochrome();
//        std::cout<<"is mono chrom:"<<image->isMonochrome()<<std::endl;
//    }
    uchar* pixelData = (uchar*)(image->getOutputData(16));
    Mat image2(int(image->getHeight()),int(image->getWidth()),CV_16U,pixelData);
//    double min,max;
//    cv::minMaxIdx(image2,&min,&max);
//    std::cout<<min<<" "<<max<<std::endl;
    imshow("Origin",image2);
    Mat ROIs = Segmentation(image2);

    Mat image8U;
    image2.convertTo(image8U,CV_8U,1./256,0);
    Mat result =  image8U & ROIs;
    imshow("Result",result);

    waitKey();
    return 0;
}




/* The function to segment the brian & veins & sick tissues.
 * Input: Mat src,(16 bits CV_16U Mat)
 * Return: Mat result, 0 means the non-intersted area, non-zero means the intersted area.
*/
Mat Segmentation(Mat src){
    Mat dst;
    /*The threshold value for different tissues.*/
    double m_dbthreshold1 = 74;
    double m_dbthreshold2 = 62;
    double m_dbthreshold3 = 82;

    /* The element for erosion and dilate. */
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));

    /* covert the dicom(16bits) to 8 bits. For the openCV operation. */
    src.convertTo(dst,CV_8U,1./256,0);
    Mat origin;
    dst.copyTo(origin);  //save the orignal image.

    // ******Step1: Find the vein (high signal) and skull outside the WM & GM.*****/
    // Using Medfilter to make image more smooth.
    cv::medianBlur(dst,dst,3);

    //Ignore the low levels (WM & GM & CSF & air)
    cv::threshold(dst,dst,m_dbthreshold1,m_dbthreshold1,THRESH_TOZERO);

    //Threshhold using Qtsu's method.
    cv::threshold(dst,dst,1,1,THRESH_OTSU);

    //Find the biggest connected area: the vein intersted.
    Mat highlevel;
    findMaxConnection(dst,highlevel);

    //Using imdilate and rode to connet the high levels veins.
    cv::dilate(highlevel,highlevel,element);
    cv::erode(highlevel,highlevel,element);

    /*********Step2: Find the WM & GM & CSF (inside the vein.)********/

    Mat D2;
    origin.copyTo(D2);

    //ignore low levels (CSF & air)
    cv::threshold(D2,D2,m_dbthreshold2,m_dbthreshold2,THRESH_TOZERO);

    //ignore high levels (skull & other hard tissues)
    cv::threshold(D2,D2,m_dbthreshold3,m_dbthreshold3,THRESH_TOZERO_INV);

    //erode away thick layer (dissolve thin surrounding tissues)
    element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    cv::erode(D2,D2,element);
    cv::erode(D2,D2,element);

    //isolate brain mass
    cv::threshold(D2,D2,1,1,THRESH_OTSU);
    Mat brain;
    findMaxConnection(D2,brain);

    //grow back main region (brian mass)
    cv::dilate(brain,brain,element);
    cv::dilate(brain,brain,element);

    /*******Connect the veins and brain inside.******/
    Mat result = brain + highlevel;
    fillHole(result,result);

    //Try to remove the skull outside.
    element = getStructuringElement(MORPH_ELLIPSE, Size(10, 10));
    cv::erode(result,result,element);
    cv::erode(result,result,element);
    cv::erode(result,result,element);
    cv::erode(result,result,element);

    //Connect the brain inside and veins.
    result = result + brain;
    return result;
}



/* The function to find the largest connected area. */
void findMaxConnection(const Mat src, Mat &dst)
{
    vector<vector<cv::Point>> contours ;
    cv::findContours(src,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

    // Find the largest connected area
    double maxArea = 0;
    int maxContourID = 0;
    vector<cv::Point> maxContour;
    for(size_t i = 0; i < contours.size(); i++)
    {
        double area = cv::contourArea(contours[i]);
        if (area > maxArea)
        {
            maxArea = area;
            maxContour = contours[i];
            maxContourID = int(i);
        }
    }
    dst =  Mat::zeros(src.size(),src.type());
    //fill the contours.
    drawContours(dst,contours,maxContourID,Scalar(255),-1,8,vector<Vec4i>(),0);
}

/* The function to fill the holes */
void fillHole(const Mat srcBw, Mat &dstBw)
{
    Size m_Size = srcBw.size();
    Mat Temp=Mat::zeros(m_Size.height+2,m_Size.width+2,srcBw.type());
    srcBw.copyTo(Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));
    cv::floodFill(Temp, Point(0, 0), Scalar(255));
    Mat cutImg;
    Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);
    dstBw = srcBw | (~cutImg);
}

