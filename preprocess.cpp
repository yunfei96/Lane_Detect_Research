#include "preprocess.hpp"

const int CONTOURS_POINT_COUNT_THRESHOLD=100;
/*
 Draw out the edge detection result, enlarge the area
 @param src
    address of the source image mat
 @param dst
    the address to write the result
 */
void findDrawContours(Mat&src,Mat&dst)
{
    src.copyTo(dst);
    dst.setTo(0);
    vector<vector<Point> > contours;
    Mat cannyed;
    Canny(src,cannyed,240,255);
    findContours(cannyed, contours, RETR_LIST, CHAIN_APPROX_NONE);
    for(int i=0;i<contours.size();i++)
    {
        if(contours[i].size()<CONTOURS_POINT_COUNT_THRESHOLD)
        {
            continue;
        }
        drawContours(dst,contours,i,255,10);
        
    }
    
}
/*
 To fit line over the contours that is passed in
 @param contours
    the contours found in 2D array form
 @param dst
    the address to write result
 */
void drawFit(vector<vector<Point>> contours,Mat&dst)
{
    for(int i=0;i<contours.size();i++)
    {
        if(contours[i].size()<CONTOURS_POINT_COUNT_THRESHOLD)
        {
            continue;
        }
        Vec4f rst;
        fitLine(contours[i], rst, CV_DIST_L2, 0, 0.01,0.01);
        int c=rst[3]-(rst[1]/rst[0])*rst[2];
        int y1=dst.rows;
        int y2=0;
        
        if(contours[i][0].y<dst.rows/3)
        {
            y1=dst.rows/3;
        }
        else if(contours[i][0].y<dst.rows/3*2)
        {
            y1=dst.rows/3*2;
            y2=dst.rows/3;
        }
        else
        {
            y2=dst.rows/3*2;
        }
        
        int x1=(y1-c)/(rst[1]/rst[0]);
        int x2=(y2-c)/(rst[1]/rst[0]);
        line(dst,Point(x1,y1),Point(x2,y2),Scalar(255,0,255),5);
    }
}

/*
 To produce the second binary image for confirming the result
 @param src
    the address of source image
 @param dst
    the address to write the result
 */

void confirmation_filter_producer(Mat src,Mat&dst){
    
    Mat contoursImg;
    Mat contoursImg2;
    Mat newFilter(Size(src.cols,src.rows),CV_8UC1,Scalar(0));
    
    cvtColor(src,contoursImg,CV_BGR2HSV);
    vector<Mat> hsvSpl;
    split(contoursImg, hsvSpl);
    Mat s,v;
    findDrawContours(hsvSpl[0],contoursImg2);
    findDrawContours(hsvSpl[1],s);
    findDrawContours(hsvSpl[2],v);
    dst=contoursImg2|s|v;
}

//constructor
void preprocess::process(bool isFirst, Mat &input, int LowH, int HighH, int LowS, int HighS, int LowV, int HighV )
{
    this->LowH = LowH;
    this->HighH = HighH;
    //smaller S lighter it can go
    this->LowS = LowS;
    this->HighS = HighS;
    //smaller V blacker it can go
    this->LowV = LowV;
    this->HighV = HighV;
    //resize the image
    image = input(Rect(0, input.rows / 2, input.cols, input.rows/2));
    //image = image(Rect(0, image.rows / 4, input.cols, image.rows/4*2));
    //
    int MAX_KERNEL_LENGTH = 4;
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    {
        GaussianBlur( image, image, Size( i, i ), 0, 0 );
    }
    //
    image.copyTo(origin_image);
    //
    toHSV();
    
    //
    toBinary(isFirst);
    //
    image = IPM(image);
    //
    origin_image = IPM(origin_image);
}

//to HSV, make it easier to set up threshold
void preprocess::toHSV()
{
    Mat imgHSV;
    vector<Mat> hsvSplit;
    //Convert the captured frame from BGR to HSV
    cvtColor(image, imgHSV, COLOR_BGR2HSV);
    split(imgHSV, hsvSplit);
    //equalizeHist(hsvSplit[2],hsvSplit[2]);
    merge(hsvSplit,imgHSV);
    image = imgHSV;
}

//change the image into Bird-eye view
Mat preprocess::IPM(Mat input)
{
    Point2f inputQuad[4];
    // Output Quadilateral or World plane coordinates
    Point2f outputQuad[4];
    
    // Lambda Matrix
    Mat lambda( 2, 4, CV_32FC1 );
    //Input and Output Image;
    Mat output;
    int coValue=0;
    int value = 270*2;
    if(value<0)
    {
        coValue=-value;
    }
    // Set the lambda matrix the same type and size as input
    lambda = Mat::zeros( input.rows, input.cols, input.type() );
    
    // The 4 points that select quadilateral on the input , from top-left in clockwise order
    // These four pts are the sides of the rect box used as input
    inputQuad[0] = Point2f( 0,0 );
    inputQuad[1] = Point2f( input.cols,0);
    inputQuad[2] = Point2f( input.cols-coValue,input.rows+1);
    inputQuad[3] = Point2f( +coValue,input.rows+1  );
    // The 4 points where the mapping is to be done , from top-left in clockwise order
    outputQuad[0] = Point2f( 0,0 );
    outputQuad[1] = Point2f( input.cols-1,0);
    outputQuad[2] = Point2f( input.cols-value,input.rows-1);
    outputQuad[3] = Point2f( value,input.rows-1  );
    
    // Get the Perspective Transform Matrix i.e. lambda
    lambda = getPerspectiveTransform( inputQuad, outputQuad );
    // Apply the Perspective Transform just found to the src image
    warpPerspective(input,output,lambda,output.size() );
    return output;
}

//use threshold to change into binary image
void preprocess::toBinary(bool isfirst)
{
    Mat imgThresholded;
    //Threshold the image
    inRange(image, Scalar(LowH, LowS, LowV), Scalar(HighH, HighS, HighV), imgThresholded);
    Mat element = getStructuringElement(MORPH_RECT, Size(2, 2));
    //open morph
    morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);
        
    //close morph
    morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);
    Mat cfilter;
    
    confirmation_filter_producer(origin_image, cfilter);
    //imshow("confirmation filter",cfilter);
    //image=isfirst?imgThresholded&cfilter:cfilter;
    
    image = cfilter&imgThresholded;
    
    imshow("confirmation filter",image);
}

//return preprocessed result
Mat preprocess::prep_result()
{
    return image;
}
