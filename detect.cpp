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
    //-----------------------test input----------------------------------
    //namedWindow("input");
    //imshow("input", input);
    //waitKey(0);
    //----------------------------------------------------------------
    preprocess step1(input);
    Mat result = step1.prep_result();
    
    //----------------------get result------------------------------
    
    int offset_x = 0;
    int offset_y = 0;
    //roi1
    Rect roi;
    roi.x = offset_x;
    roi.y = offset_y;
    roi.width = result.size().width/2;
    roi.height = result.size().height/4;
    Mat result1 = result(roi);
    //roi2
    roi.y = result.size().height/4;
    Mat result2 = result(roi);
    //roi3
    roi.y = result.size().height/2;
    Mat result3 = result(roi);
    //roi4
    roi.y = result.size().height* 3/4;
    Mat result4 = result(roi);
    //
    namedWindow("result1");
    imshow("result1", result1);
    waitKey(0);
    namedWindow("result2");
    imshow("result2", result2);
    waitKey(0);
    namedWindow("result3");
    imshow("result3", result3);
    waitKey(0);
    namedWindow("result4");
    imshow("result4", result4);
    waitKey(0);
    //------------------------------------------------------------
    ofstream myfile;
    myfile.open("/Users/YunfeiGuo/Desktop/data1.txt");
    for(int i=0; i<result1.cols; i++)
    {
        for (int j=0; j<result1.rows; j++)
        {
            if(result1.at<bool>(j, i) == true)
            {
                myfile << j << " " << i << endl;
            }
        }
    }
    myfile.close();
    myfile.open("/Users/YunfeiGuo/Desktop/data2.txt");
    for(int i=0; i<result2.cols; i++)
    {
        for (int j=0; j<result2.rows; j++)
        {
            if(result2.at<bool>(j, i) == true)
            {
                myfile << j+ result.size().height* 1/4 << " " << i << endl;
            }
        }
    }
    myfile.close();
    //
    myfile.open("/Users/YunfeiGuo/Desktop/data3.txt");
    for(int i=0; i<result3.cols; i++)
    {
        for (int j=0; j<result3.rows; j++)
        {
            if(result3.at<bool>(j, i) == true)
            {
                myfile << j+ result.size().height* 2/4  << " " << i << endl;
            }
        }
    }
    myfile.close();
    //
    myfile.open("/Users/YunfeiGuo/Desktop/data4.txt");
    for(int i=0; i<result4.cols; i++)
    {
        for (int j=0; j<result4.rows; j++)
        {
            if(result4.at<bool>(j, i) == true)
            {
                myfile << j + result.size().height* 3/4 << " " << i << endl;
            }
        }
    }
    myfile.close();
    //cout << result1.at<int>(1, 1) << endl;;
    vector<Point2f> list_point(800);
    for(int i= 100; i< 800; i++)
    {
        list_point[i].x = i;
        list_point[i].y = 0.0899*i*i-127.4791*i+44089;
        
    } //draw curve
    for(int i= 101; i< 800; i++) line(result,list_point[i-1],list_point[i],Scalar(255,0,0),4);
    imshow("image",result);
    waitKey();
    return 0;
}
