#ifndef CV_HEADER
#define CV_HEADER

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/plot.hpp"
#include "preprocess.hpp"
#include<cmath>
using namespace std;
using namespace cv;

vector<double> polyfit(vector<double> x, vector<double> y, int n=2);



#endif
