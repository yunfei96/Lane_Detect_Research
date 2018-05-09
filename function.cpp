#include "detect.hpp"

bool new_filter_valid_check(Mat _new,Mat _old){
    Mat mask=_new&_old;
    vector<vector<Point>> contours;
    findContours(mask, contours, RETR_LIST, CHAIN_APPROX_NONE);
    for(int i=0;i<contours.size();i++){
        if(contours[i].size()<600){
            contours.erase(contours.begin()+i);
        }
    }
    return contours.size()>=1;
}



vector<vector<double>> find_white_point(Mat result, bool isFirst)
{
    //----------------------get white point------------------------------
    vector<double> x_left;
    vector<double> y_left;
    int left_max = 0;
    int left_min = 1280;
    //cols is x, rows is y
    for(int i=0; i<result.rows; i++)
    {
        int white_number = 0;
        vector<double> tempx;
        vector<double> tempy;
        int x_dir_lim=result.cols/2;
        if(!isFirst){
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
        if(white_number/2 < tempy.size())
        {
            //out << tempx[white_number/2] << " ";
            //out << tempy[white_number/2] << endl;
            x_left.push_back(tempx[white_number/2]);
            y_left.push_back(tempy[white_number/2]);
        }
    }

    //----------------------right part of the image------------------
    vector<double> x_right;
    vector<double> y_right;
    int right_max = 0;
    int right_min = 1280;
    //cols is x, rows is y
    for(int i=0; i<result.rows; i++)
    {
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
        if(white_number/2 < tempy.size())
        {
            //out << tempx[white_number/2] << " ";
            //out << tempy[white_number/2] << endl;
            x_right.push_back(tempx[white_number/2]);
            y_right.push_back(tempy[white_number/2]);
        }
    }
    //lr = polyfit(xr,yr,2);

    vector<vector<double>> return_vector;
    return_vector.push_back(x_left);
    return_vector.push_back(y_left);
    return_vector.push_back(x_right);
    return_vector.push_back(y_right);
    return return_vector;
}

void draw_line_and_spread_function(Mat image, vector<double> x, vector<double> y)
{
    //road
    Point p0(x[x.size()-1], y[y.size()-1]);
    Point p1(x[x.size()-x.size()*1/16], y[y.size()-x.size()*1/16]);
    Point p2(x[x.size()-x.size()*2/16], y[y.size()-x.size()*2/16]);
    Point p3(x[x.size()-x.size()*4/16], y[y.size()-x.size()*4/16]);
    Point p4(x[x.size()-x.size()*8/16], y[y.size()-x.size()*8/16]);
    Point p5(x[x.size()-x.size()*14/16], y[y.size() -x.size()*14/16]);
    Point p6(x[0],y[0]);
    Point p7(0,0);
    if(p6.y != 0)
    {
        //find dy/dx
        double dy = p6.y-p5.y;
        double dx = p6.x-p5.x;
        double dydx = dy/dx;
        double b = p6.y -dydx*p6.x;
        double hit = -b/dydx;
        p7.x = hit;
    }
    //left line
    Point left_sp0 = p0;
    Point left_sp1(p1.x - 5, p1.y);
    Point left_sp2(p2.x - 10,p2.y);
    Point left_sp3(p3.x - 10,p3.y);
    Point left_sp4(p4.x - 15,p4.y);
    Point left_sp5(p5.x - 20,p5.y);
    Point left_sp6(p6.x - 20,p6.y);
    Point left_sp7(p7.x - 25,p7.y);
    //right line
    Point right_sp0 = p0;
    Point right_sp1(p1.x + 5, p1.y);
    Point right_sp2(p2.x + 10,p2.y);
    Point right_sp3(p3.x + 10,p3.y);
    Point right_sp4(p4.x + 15,p4.y);
    Point right_sp5(p5.x + 20,p5.y);
    Point right_sp6(p6.x + 20,p6.y);
    Point right_sp7(p7.x + 25,p7.y);
    //draw the line
    /*
    line(image, p0, p1, cv::Scalar(255,0,0), 2);
    line(image, p1, p2, cv::Scalar(255,0,0), 2);
    line(image, p2, p3, cv::Scalar(255,0,0), 2);
    line(image, p3, p4, cv::Scalar(255,0,0), 2);
    line(image, p4, p5, cv::Scalar(255,0,0), 2);
    line(image, p5, p6, cv::Scalar(255,0,0), 2);
    if(p6.y != 0)
    {
        line(image, p6, p7, cv::Scalar(255,0,0), 2);
    }*/
    //draw left line
    line(image, left_sp0, left_sp1, cv::Scalar(255,255,0), 2);
    line(image, left_sp1, left_sp2, cv::Scalar(255,255,0), 2);
    line(image, left_sp2, left_sp3, cv::Scalar(255,255,0), 2);
    line(image, left_sp3, left_sp4, cv::Scalar(255,255,0), 2);
    line(image, left_sp4, left_sp5, cv::Scalar(255,255,0), 2);
    line(image, left_sp5, left_sp6, cv::Scalar(255,255,0), 2);
    if(p6.y != 0)
    {
        line(image, left_sp6, left_sp7, cv::Scalar(255,255,0), 2);
    }
    //draw right line
    line(image, right_sp0, right_sp1, cv::Scalar(255,255,0), 2);
    line(image, right_sp1, right_sp2, cv::Scalar(255,255,0), 2);
    line(image, right_sp2, right_sp3, cv::Scalar(255,255,0), 2);
    line(image, right_sp3, right_sp4, cv::Scalar(255,255,0), 2);
    line(image, right_sp4, right_sp5, cv::Scalar(255,255,0), 2);
    line(image, right_sp5, right_sp6, cv::Scalar(255,255,0), 2);
    if(p6.y != 0)
    {
        line(image, right_sp6, right_sp7, cv::Scalar(255,255,0), 2);
    }
}
