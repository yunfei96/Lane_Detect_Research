//
//  NextFrameFilters.h
//  Lane_Detect_Research
//
//  Created by Jason Lu on 5/23/18.
//

#ifndef NextFrameFilters_h
#define NextFrameFilters_h
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc.hpp"
using namespace std;
using namespace cv;
class NextFrameFilters{
private:
    deque<Mat> L;
    deque<Mat> R;
    int count=0;
    Mat result(deque<Mat> lst){
        Mat rst;
        if(lst.size()>=1){
            rst=lst.front();
        }
        for(int i=0;i<lst.size();i++){
            Mat front=lst.front();
            rst=rst&front;
            lst.pop_front();
            lst.push_back(front);
        }
        
        return rst;
    }
public:
    NextFrameFilters(int count){
        this->count=count;
    }
    ~NextFrameFilters(){
        
    }
    void addL(Mat item){
        L.push_back(item);
        if(L.size()>count){
            L.pop_front();
        }
    }
    void addR(Mat item){
        R.push_back(item);
        if(R.size()>count){
            R.pop_front();
        }
    }
    int Lsize(){
        return L.size();
    }
    int Rsize(){
        return R.size();
    }
    Mat Lresult(){
        return result(L);
    }
    Mat Rresult(){
        return result(R);
    }
};


#endif /* NextFrameFilters_h */
