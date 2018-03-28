#ifndef DETECTH
#define DETECTH
#include "detect.hpp"
using namespace cv;
using namespace std;
//This class is used to do pre-process to the image to get enough point to perform machine learning
class preprocess
{
private:
    Mat image;
    //---------------------
    //H all range
    int LowH = 0;
    int HighH = 255;
    //smaller S lighter it can go
    int LowS = 0;
    int HighS = 20;
    //smaller V blacker it can go
    int LowV = 250;
    int HighV = 255;

public:
    //constructor
    preprocess(Mat &input);
    
    //to HSV, make it easier to set up threshold
    void toHSV();
    
    //Gaussian filter to reduce noise casued by light spot
    void filter();
    
    //change the image into Bird-eye view
    void IPM();
    
    //use threshold to change into binary image
    void toBinary();
    
    //change the threshold based on output
    void refine();
    
    //return preprocessed result
    Mat prep_result();
};
#endif
