#include "detect.hpp"
#include <fstream>
#ifndef VIDEO_MODE
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
    preprocess step1(input);
    Mat result = step1.prep_result();
    //-------------------find white point
    vector<vector<double>> white_points = find_white_point(result);
    vector<double> left_x = white_points[0];
    vector<double> left_y = white_points[1];
    vector<double> right_x = white_points[2];
    vector<double> right_y = white_points[3];
    //-----
    draw_line_and_spread_function(step1.origin_image, left_x, left_y);
    draw_line_and_spread_function(step1.origin_image, right_x, right_y);
    //-----
    imshow("image",step1.origin_image);
    waitKey();
    return 0;
}
#endif
