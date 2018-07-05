//  source file name: detect_video.cpp function.cpp
//  headers: detect.hpp
//  build: detect
//  Created by Jason Lu on 4/19/18.
//  Modified by Yunfei Guo on 6/30/18

#include "detect.hpp"
#ifdef VIDEO_MODE
VideoWriter output;
const string NAME="Output.avi";
const int min_first_frame_point_thresh = 3;
/*
 the main function of the program
 @param argc
 @param argv
    see argument rule below
 */
int main(int argc, char* argv[])
{
    VideoCapture cap;
    setUseOptimized(true);
    /**
     * argument rules:
     *  if there is no aregument, open device zero as default.
     *  if there is one arguement, assume it is a single digit
     *      that represent the device number.
     *  if there is one areguement and it is not a single digit,
     *      treat it as a local address of a video file.
     */
    if(argc==1)
    {
        cap.open(0);
    }
    else if(argc>=2)
    {
        if(strlen(argv[1])==1 && isdigit(argv[1][0]))
        {
            cap.open(argv[1][0]-'0');
        }
        else
        {
            cap.open(argv[1]);
        }
    }
    if(!cap.isOpened())
    {
        cout<<"video stream cannot be correctly opened.\n";
        return EXIT_FAILURE;   // if the video stream is failed to open.
    }
    
    Mat frame;
    cap>>frame;
#ifdef RECORD_RST
    Size S = Size(1280,288);
    output.open(NAME, -1, 25, S, true);
#endif
    int emptyFrameCount=0;
    bool isFirstFrame=true;
    Mat filter_frame_L,filter_frame_R; // init the left and right filter mat
    int follow_up_fail_count=0;
    
    while(1)// cycle start
    {
        if(frame.empty())// check if read frame is failed
        {
            emptyFrameCount++;
            if(emptyFrameCount>=9)
            {
                cout<<"fail to read frame from stream, or video ended.\n";
                return EXIT_FAILURE;
            }
            waitKey(27);
            cap>>frame;
            continue;
        }
        emptyFrameCount=0;
        
        if(isFirstFrame) // first frame identification
        {
            bool success=img_proc(frame,filter_frame_L,filter_frame_R,true);
            if(success)
            {
                isFirstFrame=false;
            }
        }
        else // following frames
        {
            bool success=img_proc(frame,filter_frame_L,filter_frame_R,false);
            //imshow("filter frame l", filter_frame_L); // identified left reigon
            //imshow("filter frame r", filter_frame_R); // identified right reigon
            if(!success) // Lane is not detected
            {
                follow_up_fail_count++;
                if(follow_up_fail_count==200) //if lane cannot be detected in 200 frame, back to first frame identification
                {
                    follow_up_fail_count=0;
                    isFirstFrame=true;
                }
            }
            else
            {
                follow_up_fail_count=0;
            }
        }
        waitKey(1);//cycle end
        cap>>frame;
    }
#ifdef RECORD_RST
    output.release(); //output video
#endif
    
    return 0;
}
/*
 lane recognition and output result
 @param src
    the source image in Mat
 @param filter_frame_L
    "next frame filter" of the left lane, which marks the possible region where lane
    mark could occur
 @param filter_frame_R
    "next frame filter" of the right lane
 @param isFirst
    indicate if it is in process of finding the lane marks in the first frame passed in
 @return
    if the process is successful
 */
bool img_proc(Mat src, Mat&filter_frame_L, Mat&filter_frame_R,bool isFirst)
{
    preprocess step1;
    if(isFirst) //if is first frame
    {
        step1.process(isFirst,src, 0,255,0,15,250,255); // use both color and edge filter
    }
    else
    {
        step1.process(isFirst, src,isFirst); // only use edge filter
    }
    Mat result = step1.prep_result();
    vector<vector<double>> white_points;
    vector<double> left_x ;
    vector<double> left_y ;
    vector<double> right_x ;
    vector<double> right_y ;
    if(isFirst)
    {
        
        white_points = find_white_point(result, true);
        left_x = white_points[0];
        left_y = white_points[1];
        right_x = white_points[2];
        right_y = white_points[3];
    }
    else
    {
        Mat rstl=result&filter_frame_L;
        Mat rstr=result&filter_frame_R;
        white_points=find_white_point(rstl, false);
        left_x = white_points[0];
        left_y = white_points[1];
        white_points=find_white_point(rstr, false);
        right_x = white_points[2];
        right_y = white_points[3];
    }
    
    //eliminating extreme condition
    if(left_x.size()<=min_first_frame_point_thresh||left_y.size()<=min_first_frame_point_thresh||right_x.size()<=min_first_frame_point_thresh||right_y.size()<=min_first_frame_point_thresh)
    {
        return false;
    }
    draw_line_and_spread_function(step1.origin_image, left_x, left_y);
    draw_line_and_spread_function(step1.origin_image, right_x, right_y);
    //before passing the filter frames, initialize it.
    Mat fl(Size(step1.origin_image.cols,step1.origin_image.rows),CV_8UC1,Scalar(0));
    draw_line_and_spread_function(fl, left_x, left_y);
    floodFill(fl,Point(left_x[0],left_y[0]),Scalar(255));
    inRange(fl,Scalar(250),Scalar(256),fl);
    
    Mat fr(Size(step1.origin_image.cols,step1.origin_image.rows),CV_8UC1,Scalar(0));
    draw_line_and_spread_function(fr, right_x, right_y);
    floodFill(fr,Point(right_x[0],right_y[0]),Scalar(255));
    inRange(fr,Scalar(250),Scalar(256),fr);
    if(isFirst)
    {
        filter_frame_L=fl;
        filter_frame_R=fr;
    }
    else
    {
        if(new_filter_valid_check(fl, filter_frame_L) && new_filter_valid_check(fr, filter_frame_R))
        {
            filter_frame_L=fl;
            filter_frame_R=fr;
        }
        else
        {
            return false;
        }
    }
    imshow("image",step1.origin_image);
    
#ifdef RECORD_RST
    Mat out;
    resize(step1.origin_image, out, Size(1280,288));
    output<<out;
#endif
    return true;
}
#endif
