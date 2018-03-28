#include "detect.hpp"
#include <fstream>
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
    //----------------------get white point------------------------------
    // left part of the image
    vector<double> xl;
    vector<double> yl;
    vector<double> ll;
    long kl=0;
    int lmax = 0;
    int lmin = 1280;
    //cols is x
    //rows is y
    for(int i=0; i<result.cols/2; i++)
    {
        for (int j=0; j<result.rows; j++)
        {
            if(result.at<bool>(j, i) == true)
            {
                if(i>lmax)
                {
                    lmax = i;
                }
                if(i<lmin)
                {
                    lmin = i;
                }
                xl.push_back(i);
                yl.push_back(j);
                //cout << i << " "<< j << endl;
                kl++;
            }
        }
    }
    ll = polyfit(xl,yl);
    //----------------------right part of the image------------------
    vector<double> xr;
    vector<double> yr;
    vector<double> lr;
    long kr =0;
    int rmax = 0;
    int rmin = 1280;
    //cols is x
    //rows is y
    ofstream out;
    out.open("/Users/YunfeiGuo/Desktop/data.txt");
    for(int i=result.cols/2; i<result.cols; i++)
    {
        for (int j=0; j<result.rows; j++)
        {
            if(result.at<bool>(j, i) == true)
            {
                if(i>rmax)
                {
                    rmax = i;
                }
                if(i<rmin)
                {
                    rmin = i;
                }
                xr.push_back(i);
                yr.push_back(j);
                out << i << " "<< j << "\n";
                kr++;
            }
        }
    }
    lr = polyfit(xr,yr,1);
    //-------------------cut into 2 half and find points-----------------------
    vector<Point2f> list_point(lmax);
    vector<Point2f> list_right_point(lmax);
    vector<Point2f> list_left_point(lmax);
    int a = 0;
    for(int i= lmin; i< lmax; i++)
    {
        list_point[i].x = i;
        list_right_point[i].x = i+a;
        list_left_point[i].x = i-a;
        double temp_y = ll[2]*i*i+ll[1]*i+ll[0];
        list_point[i].y = temp_y;
        list_right_point[i].y = temp_y;
        list_left_point[i].y =  temp_y;
        a++;
    } //draw curve
    for(int i= lmin; i< lmax-1; i++)
    {
        line(result,list_point[i],list_point[i+1],Scalar(255,0,0),2);
        line(result,list_right_point[i],list_right_point[i+1],Scalar(255,255,0),2);
        line(result,list_left_point[i],list_left_point[i+1],Scalar(255,255,0),2);
    }
    //------------------------------right---------------------
    vector<Point2f> list_pointr(1280);
    vector<Point2f> list_right_pointr(1280);
    vector<Point2f> list_left_pointr(1280);
    int ar = 0;
    for(int i= rmin; i< 1280; i++)
    {
        list_pointr[i].x = i;
        list_right_pointr[i].x = i+ar;
        list_left_pointr[i].x = i-ar;
        double temp_y = lr[1]*i+lr[0];
        list_pointr[i].y = temp_y;
        list_right_pointr[i].y = temp_y;
        list_left_pointr[i].y = temp_y;
        ar++;
    } //draw curve
    for(int i= rmin; i< 1280-1
        ; i++)
    {
        line(result,list_pointr[i],list_pointr[i+1],Scalar(255,0,0),2);
        //line(result,list_right_pointr[i],list_right_pointr[i+1],Scalar(255,255,0),2);
        //line(result,list_left_pointr[i],list_left_pointr[i+1],Scalar(255,255,0),2);
    }
    imshow("image",result);
    waitKey();
    return 0;
}
