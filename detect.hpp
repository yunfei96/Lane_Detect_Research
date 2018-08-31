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
#include <queue>
//This need to be enabled when real road data is used, because real road lane might be white/yellow, color filter is not enabled
//#define REAL_ROAD_MODE
//This need to be enabled when test using robot
#define ROBOT_TEST_MODE
//if you want to output any video, set record rst
//#define RECORD_RST

using namespace std;
using namespace cv;

//name of output file
const string NAME="/Users/YunfeiGuo/Desktop/output.mp4";
/*
 @param: result
 it is the result of the filter
 @param: isFirst
 if this frame is first frame
 @return:
 a list of (x,y) coordinate of white point(the lane is white in binary image)
 */
vector<vector<double>> find_white_point(Mat result, bool isFirst);
/*
 @param: image
 the original image
 @param: x
 x value of white points in xy coordinate
 @param: y
 y value of white points in xy coordinate
 */
void draw_spread_function(Mat image, vector<double> x, vector<double> y);
void draw_line(Mat image, vector<double> x, vector<double> y);
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
