#include "detect.hpp"
int main(int argc, char *argv[])
{
    Mat input;
    //read-in image
    if( argc != 2)
    {
        cout <<" Error: input argument not correct. \n Usage: detect ImageToLoadAndDisplay" << endl;
        return -1;
    }
    //load color image
    input = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    
    // Check for invalid input
    if(! input.data )
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    //-----------------------test input----------------------------------
    //namedWindow("input");
    //imshow("input", input);
    //waitKey(0);
    //----------------------------------------------------------------
    preprocess step1(input);
    
    return 0;
}
