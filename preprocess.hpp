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
    int HighS = 25;
    //smaller V blacker it can go
    int LowV = 245;
    int HighV = 255;

public:
    //constructor
    void process(Mat &input, int LowH = 0, int HighH = 255, int LowS =0, int HighS=28, int LowV=243, int HighV=255);
    
    //to HSV, make it easier to set up threshold
    void toHSV();
    
    //Gaussian filter to reduce noise casued by light spot
    void filter();
    
    //change the image into Bird-eye view
    Mat IPM(Mat input);
    
    //use threshold to change into binary image
    void toBinary();
    
    //change the threshold based on output
    void refine();
    
    //return preprocessed result
    Mat prep_result();
    Mat origin_image;
};
#endif
