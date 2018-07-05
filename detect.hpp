#ifndef CV_HEADER
#define CV_HEADER

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include "preprocess.hpp"
#include <cmath>

//if the input is a video and there is no need to output any video, set video mode 
#define VIDEO_MODE
//if you want to output any video, set record rst
//#define RECORD_RST

using namespace std;
using namespace cv;

//extract white point in the image
vector<vector<double>> find_white_point(Mat result, bool isFirst);
//draw lane and spread function
void draw_line_and_spread_function(Mat image, vector<double> x, vector<double> y);
/*
This function check if the new "next frame filter" is valid. To be valid,
new and old have to share a sufficient common area
@param _new
    the new filter
@param _old
    the old filter
*/
bool new_filter_valid_check(Mat _new,Mat _old);
/*
 lane recognition and output result
 @param src
    the source image in Mat
 @param filter_frame_L
    "next frame filter" of the left lane, which marks the possible region where lane
    mark could occur
 @param filter_frame_R
    "next frame filter" of the right lane
 @param isFirst
    indicate if it is in process of finding the lane marks in the first frame passed in
 @return
    if the process is successful
 */
bool img_proc(Mat src, Mat&filter_frame_L, Mat&filter_frame_R,bool isFirst);
#endif
