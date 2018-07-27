#include "detect.hpp"
/*
 This function check if the new "next frame filter" is valid. To be valid,
 new and old have to share a sufficient common area
 @param _new
    the new filter
 @param _old
    the old filter
 */
bool new_filter_valid_check(Mat _new,Mat _old){
    Mat mask=_new&_old;
    vector<vector<Point>> contours;
    findContours(mask, contours, RETR_LIST, CHAIN_APPROX_NONE);
    for(int i=0;i<contours.size();i++)
    {
        if(contours[i].size()<100)
        {
            contours.erase(contours.begin()+i);
        }
    }
    return contours.size()>=1;
}


/*
 @param: result
    it is the result of the filter
 @param: isFirst
    if this frame is first frame
 @return:
    a list of (x,y) coordinate of white point(the lane is white in binary image)
 */

/*
 in first frame, cut the whole image vertically into 2 parts
else use the whole image
 */
vector<vector<double>> find_white_point(Mat result, bool isFirst)
{
    //get white point
    vector<double> x_left;
    vector<double> y_left;
    //cols is x, rows is y
    //--------------------left part of the image-----------------
    for(int i=0; i<result.rows; i++)
    {
        int left_max = 0;
        int left_min = 1280;
        int white_number = 0;
        vector<double> tempx;
        vector<double> tempy;
        int x_dir_lim=result.cols/2;
        if(!isFirst)
        {
            x_dir_lim=result.cols;
        }
        for (int j=0; j<x_dir_lim; j++)
        {
            if(result.at<bool>(i, j) == true)
            {
                if(j>left_max)
                {
                    left_max = j;
                }
                if(j<left_min)
                {
                    left_min = j;
                }
                tempx.push_back(j);
                tempy.push_back(i);
                white_number++;
            }
        }
        //find the mean
        if(white_number/2 < tempy.size()&&(left_max-left_min <= 500))
        {
            x_left.push_back(tempx[white_number/2]);
            y_left.push_back(tempy[white_number/2]);
        }
    }

    //----------------------right part of the image------------------
    vector<double> x_right;
    vector<double> y_right;
    //cols is x, rows is y
    for(int i=0; i<result.rows; i++)
    {
        int right_max = 0;
        int right_min = 1280;
        int white_number = 0;
        vector<double> tempx;
        vector<double> tempy;
        int x_dir_lim=result.cols/2;
        if(!isFirst){
            x_dir_lim=0;
        }
        for (int j=x_dir_lim; j<result.cols; j++)
        {
            if(result.at<bool>(i, j) == true)
            {
                if(j>right_max)
                {
                    right_max = j;
                }
                if(j<right_min)
                {
                    right_min = j;
                }
                tempx.push_back(j);
                tempy.push_back(i);
                white_number++;
            }
        }
        //find the mean
        if(white_number/2 < tempy.size()&&(right_max-right_min <= 500))
        {
            x_right.push_back(tempx[white_number/2]);
            y_right.push_back(tempy[white_number/2]);
        }
    }
    vector<vector<double>> return_vector;
    return_vector.push_back(x_left);
    return_vector.push_back(y_left);
    return_vector.push_back(x_right);
    return_vector.push_back(y_right);
    return return_vector;
}
/*
 @param: image
 the original image
 @param: x
 x value of white points in xy coordinate
 @param: y
 y value of white points in xy coordinate
*/

//draw area of interest. Make the area an enclosed area
//The idea is find 8 critical points and directly connect a line between these points.

void draw_line_and_spread_function(Mat image, vector<double> x, vector<double> y)
{
    //find 8 points and link these points together
    Point p0(x[x.size()-1], y[y.size()-1]);
    Point p1(x[x.size()-x.size()*1/64-1], y[y.size()-x.size()*1/64-1]);
    Point p2(x[x.size()-x.size()*1/32-1], y[y.size()-x.size()*1/32-1]);
    Point p3(x[x.size()-x.size()*1/16-1], y[y.size()-x.size()*1/16-1]);
    Point p4(x[x.size()-x.size()*3/16-1], y[y.size()-x.size()*3/16-1]);
    Point p5(x[x.size()-x.size()*5/16-1], y[y.size()-x.size()*5/16-1]);
    Point p6(x[x.size()-x.size()*7/16-1], y[y.size()-x.size()*7/16-1]);
    Point p7(x[x.size()-x.size()*9/16-1], y[y.size()-x.size()*9/16-1]);
    Point p8(x[x.size()-x.size()*13/16-1], y[y.size() -x.size()*13/16-1]);
    Point p9(x[0],y[0]);
    Point p10(0,0);
    //extend to bottom of the image
    if(p0.y < image.rows-20)
    {
        //find dy/dx
        double dy = p0.y-p1.y;
        double dx = p0.x-p1.x;
        if(dx != 0)
        {
            double k = dy/dx;
            double b = p0.y -k*p0.x;
            double x_predict = (image.rows-b)/k;
            p0.x = x_predict;
            p0.y = image.rows;
        }
        else
        {
            p0.y = p0.y+7;
        }
    }
    //extend to top of the image
    //if highest point is not reach top
    if(p9.y != 0)
    {
        //if highest point is high enough and direct link the to the top
        if(p9.y < 80)
        {
            //find dy/dx
            double dy = p9.y-p8.y;
            double dx = p9.x-p8.x;
            if(dx != 0)
            {
                double k = dy/dx;
                double b = p9.y -k*p9.x;
                double x_predict = -b/k;
                p10.x = x_predict;
            }
            else
            {
                p10.x = p9.x;
            }
        }
        else
        {
            p10.y = p9.y -80;
            //find dy/dx
            double dy = p9.y-p8.y;
            double dx = p9.x-p8.x;
            if(dx != 0)
            {
                double k = dy/dx;
                double b = p9.y -k*p9.x;
                double x_predict = (p10.y-b)/k;
                p10.x = x_predict;
            }
            else
            {
                p10.x = p9.x;
            }
        }
    }
    //left line
    Point left_sp0(p0.x, p0.y);
    Point left_sp1(p1.x - 5, p1.y);
    Point left_sp2(p2.x - 6,p2.y);
    Point left_sp3(p3.x - 8,p3.y);
    Point left_sp4(p4.x - 10,p4.y);
    Point left_sp5(p5.x - 10,p5.y);
    Point left_sp6(p6.x - 10,p6.y);
    Point left_sp7(p7.x - 10,p7.y);
    Point left_sp8(p8.x - 10,p8.y);
    Point left_sp9(p9.x - 10,p9.y);
    Point left_sp10(p10.x - 10,p10.y);
    //right line
    Point right_sp0(p0.x, p0.y);
    Point right_sp1(p1.x + 5, p1.y);
    Point right_sp2(p2.x + 6,p2.y);
    Point right_sp3(p3.x + 8,p3.y);
    Point right_sp4(p4.x + 10,p4.y);
    Point right_sp5(p5.x + 10,p5.y);
    Point right_sp6(p6.x + 10,p6.y);
    Point right_sp7(p7.x + 10,p7.y);
    Point right_sp8(p8.x + 10,p8.y);
    Point right_sp9(p9.x + 10,p9.y);
    Point right_sp10(p10.x + 10,p10.y);
    //draw left line
    line(image, left_sp0, left_sp1, Scalar(255,255,0), 2);
    line(image, left_sp1, left_sp2, Scalar(255,255,0), 2);
    line(image, left_sp2, left_sp3, Scalar(255,255,0), 2);
    line(image, left_sp3, left_sp4, Scalar(255,255,0), 2);
    line(image, left_sp4, left_sp5, Scalar(255,255,0), 2);
    line(image, left_sp5, left_sp6, Scalar(255,255,0), 2);
    line(image, left_sp6, left_sp7, Scalar(255,255,0), 2);
    line(image, left_sp7, left_sp8, Scalar(255,255,0), 2);
    line(image, left_sp8, left_sp9, Scalar(255,255,0), 2);
    
    if(p9.y != 0)
    {
        line(image, left_sp9, left_sp10, Scalar(255,255,0), 2);
    }
    //draw right line
    line(image, right_sp0, right_sp1, Scalar(255,255,0), 2);
    line(image, right_sp1, right_sp2, Scalar(255,255,0), 2);
    line(image, right_sp2, right_sp3, Scalar(255,255,0), 2);
    line(image, right_sp3, right_sp4, Scalar(255,255,0), 2);
    line(image, right_sp4, right_sp5, Scalar(255,255,0), 2);
    line(image, right_sp5, right_sp6, Scalar(255,255,0), 2);
    line(image, right_sp6, right_sp7, Scalar(255,255,0), 2);
    line(image, right_sp7, right_sp8, Scalar(255,255,0), 2);
    line(image, right_sp8, right_sp9, Scalar(255,255,0), 2);
    if(p9.y != 0)
    {
        line(image, right_sp9, right_sp10, Scalar(255,255,0), 2);
    }
    if(p9.y != 0 && p9.y >= 80)
    {
         line(image, right_sp10, left_sp10, Scalar(255,255,0), 2);
    }
}
