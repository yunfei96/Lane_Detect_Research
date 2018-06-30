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

//if the input is a video and there is no need to output any video, set video mode to 1
#define VIDEO_MODE 1
//if you want to output any video, set record rst to 1
#define RECORD_RST 0

using namespace std;
using namespace cv;

//extract white point in the image
vector<vector<double>> find_white_point(Mat result, bool isFirst);
//draw lane and spread function
void draw_line_and_spread_function(Mat image, vector<double> x, vector<double> y);
bool new_filter_valid_check(Mat _new,Mat _old);
bool img_proc(Mat src, Mat&filter_frame_L, Mat&filter_frame_R,bool isFirst);
#endif
