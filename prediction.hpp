#ifndef PREDICTION_FILTER
#define PREDICTION_FILTER

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <stack>

using namespace std;
using namespace cv;

void prediction_filter(vector<Point> points, Mat&dst, int k){
    vector<Point> left,right;
    if(points.size()<=1){
        cout<<"not enough points!\n";
        return;
    }
    for(int i=0;i<points.size();i++){
        
        left.push_back(Point(points[i].x-(dst.rows-points[i].y)*k,points[i].y));
        right.push_back(Point(points[i].x+(dst.rows-points[i].y)*k,points[i].y));
    }
    Mat rtn(Size(dst.cols,dst.rows),CV_8UC1,Scalar(0));
    for(int i=1;i<points.size();i++){
        line(rtn,left[i-1],left[i],Scalar(255));
        line(rtn,right[i-1],right[i],Scalar(255));
    }
    floodFill(rtn,Point(left[left.size()/2].x+2,left[left.size()/2].y),Scalar(255));
    Mat imgThresholded;
    inRange(rtn,Scalar(250),Scalar(256),imgThresholded);
    dst=dst&imgThresholded;
        
    
}





#endif
