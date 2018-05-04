#ifndef CV_HEADER
#define CV_HEADER

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc.hpp"
//#include "opencv2/plot.hpp"
#include "preprocess.hpp"
#include<cmath>

//if the program is in video mode, define VIDEO_MODE
#define VIDEO_MODE 1
//if it is debug mode it shows the images. define DEBUG_MODE
#define DEBUG_MODE 1

using namespace std;
using namespace cv;
vector<vector<double>> find_white_point(Mat result);
void draw_line_and_spread_function(Mat image, vector<double> x, vector<double> y);

#endif
