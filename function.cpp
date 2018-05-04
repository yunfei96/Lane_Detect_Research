#include "detect.hpp"

vector<vector<double>> find_white_point(Mat result)
{
    //----------------------get white point------------------------------
    // left part of the image
    vector<double> x_left;
    vector<double> y_left;
    //ofstream out;
    //out.open("/Users/YunfeiGuo/Desktop/data.txt");
    int left_max = 0;
    int left_min = 1280;
    //cols is x, rows is y
    for(int i=0; i<result.rows; i++)
    {
        int white_number = 0;
        vector<double> tempx;
        vector<double> tempy;
        for (int j=0; j<result.cols/2; j++)
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
        for (int j=result.cols/2; j<result.cols; j++)
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
    Point p1(x[x.size()/2], y[y.size()/2]);
    Point p2(x[0],y[0]);
    //left line
    Point left_sp0 = p0;
    Point left_sp1 = p1;
    left_sp1.x = p1.x -25;
    Point left_sp2 = p2;
    left_sp2.x = p2.x - 50;
    //right line
    Point right_sp0 = p0;
    Point right_sp1 = p1;
    right_sp1.x = p1.x + 25;
    Point right_sp2 = p2;
    right_sp2.x = p2.x + 50;
    //draw the line
    line(image, p0, p1, cv::Scalar(255,0,0), 2);
    line(image, p1, p2, cv::Scalar(255,0,0), 2);
    //draw left line
    line(image, left_sp0, left_sp1, cv::Scalar(0,255,0), 2);
    line(image, left_sp1, left_sp2, cv::Scalar(0,255,0), 2);
    //draw right line
    line(image, right_sp0, right_sp1, cv::Scalar(0,255,0), 2);
    line(image, right_sp1, right_sp2, cv::Scalar(0,255,0), 2);
}
