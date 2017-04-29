#include "segment.h"
#include <iostream>
#include <iomanip>

/* The function to segment the brian & veins & sick tissues.
 * Input: Mat src,(16 bits CV_16U Mat)
 * Return: Mat result, 0 means the non-intersted area, non-zero means the intersted area.
*/
Mat Segmentation(Mat src){
    Mat dst;
    /* Covert the source image. */
    src = src + 1024;
    src.convertTo(src,CV_16U);
    /*The threshold value for different tissues.*/
//    double m_dbthreshold1 = 74;
//    double m_dbthreshold2 = 62;
//    double m_dbthreshold3 = 82;
    double m_dbthreshold1 = 1200;
    double m_dbthreshold2 = 990;
    double m_dbthreshold3 = 1300;

    /* The element for erosion and dilate. */
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));

    /* covert the dicom(16bits) to 8 bits. For the openCV operation. */

    dst = src;
    double MaxValue, MinValue;
    cv::minMaxLoc(dst, &MinValue,&MaxValue);
//  //  std::cout<<MaxValue<<"   "<<MinValue;
    src.convertTo(dst,CV_8UC1,1./MaxValue*256,0);
    m_dbthreshold1 = 1200./MaxValue*256.;
    m_dbthreshold2 = 990./MaxValue*256.;
    m_dbthreshold3 = 1300./MaxValue*256.;


    Mat origin;
    dst.copyTo(origin);  //save the orignal image.

    // ******Step1: Find the vein (high signal) and skull outside the WM & GM.*****/
    // Using Medfilter to make image more smooth.
    cv::medianBlur(dst,dst,3);

    //Ignore the low levels (WM & GM & CSF & air)
    cv::threshold(dst,dst,m_dbthreshold1,m_dbthreshold1,THRESH_TOZERO);

    //Threshhold using Qtsu's method.
    //cv::threshold(dst,dst,1,1,THRESH_OTSU);
    cv::threshold(dst,dst,256,256,THRESH_OTSU);

    //Find the biggest connected area: the vein intersted.
    Mat highlevel;
    findMaxConnection(dst,highlevel);

    //Using imdilate and rode to connet the high levels veins.
    cv::dilate(highlevel,highlevel,element);
    cv::erode(highlevel,highlevel,element);
    //imshow("high",highlevel);

    /*********Step2: Find the WM & GM & CSF (inside the vein.)********/
    Mat D2;
    origin.copyTo(D2);

    //ignore low levels (CSF & air)
    cv::threshold(D2,D2,m_dbthreshold2,m_dbthreshold2,THRESH_TOZERO);

    //ignore high levels (skull & other hard tissues)
    cv::threshold(D2,D2,m_dbthreshold3,m_dbthreshold3,THRESH_TOZERO_INV);

    cv::threshold(D2,D2,256,256,THRESH_OTSU);

    //erode away thick layer (dissolve thin surrounding tissues)
    element = getStructuringElement(MORPH_RECT, Size(7, 7 ));
    //imshow("D21",D2);
    cv::erode(D2,D2,element);

    //isolate brain mass
    Mat brain;
    findMaxConnection(D2,brain);

    //grow back main region (brian mass)
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

    //imshow("result",result);
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

