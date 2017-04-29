#ifndef SEGMENT_H
#define SEGMENT_H

#endif // SEGMENT_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;


Mat Segmentation(Mat src);
void fillHole(const Mat srcBw, Mat &dstBw);
void findMaxConnection(const Mat srcBw, Mat &dstBw);
