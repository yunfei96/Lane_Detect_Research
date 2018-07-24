#ifndef DETECTH
#define DETECTH
#include "detect.hpp"
using namespace cv;
using namespace std;
//This class is used to do pre-process
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
    int HighS = 40;
    //smaller V blacker it can go
    int LowV = 199;
    int HighV = 255;

public:
    /*constructor
     @param isFirst
        Indicate if this is the process in finding the first frame of the recognition process
     @param input
        The address of the input source
     @param <Low/High><H/S/V>
        HSV threshold range
     */
    void process(bool isFirst, Mat &input, int LowH = 0, int HighH = 255, int LowS =0, int HighS=28, int LowV=199
                 , int HighV=255 );
    
    /*to HSV, make it easier to set up threshold
     conver the image to hsv solor space.
     */
    void toHSV();
    
    /*change the image into Bird-eye view
     @param input
        source mat
     @return
        mat in bird eye view
     */
    Mat IPM(Mat input);
    
    /*use threshold to change into binary image
     @param isfirst
        Indicate if it is finding the available first frame
     */
    void toBinary(bool isfirst);
    
    /*return preprocessed result
     @return
        return the preprocess result
     */
    Mat prep_result();
    /*
     @return
        return the original image before the process
     */
    Mat origin_image;
};
#endif
