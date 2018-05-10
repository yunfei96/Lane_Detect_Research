//
//  detect_video.cpp
//  detect
//
//  Created by Jason Lu on 4/19/18.
//

#include "detect.hpp"
const int min_first_frame_point_thresh = 3;

#ifdef VIDEO_MODE





bool img_proc(Mat src, Mat&filter_frame_L, Mat&filter_frame_R,bool isFirst)
{
    preprocess step1;
    if(isFirst)
    {
        step1.process(src,0,255,0,15,250,255);
    }
    else
    {
        step1.process(src);
    }
    Mat result = step1.prep_result();

#ifdef DEBUG_MODE
    imshow("step 1 result",result);
    //waitKey(10);
#endif

    vector<vector<double>> white_points = find_white_point(result, true);
    vector<double> left_x = white_points[0];
    vector<double> left_y = white_points[1];
    vector<double> right_x = white_points[2];
    vector<double> right_y = white_points[3];
    if(!isFirst){
        Mat rstl=result&filter_frame_L;
        Mat rstr=result&filter_frame_R;
        imshow("filtered L",rstl);
        imshow("filtered R",rstr);
        white_points=find_white_point(rstl, false);
        left_x = white_points[0];
        left_y = white_points[1];
        white_points=find_white_point(rstr, false);
        right_x = white_points[2];
        right_y = white_points[3];
    }

    

    //-----
    //eliminating extreme condition
    if(left_x.size()<=min_first_frame_point_thresh||left_y.size()<=min_first_frame_point_thresh||right_x.size()<=min_first_frame_point_thresh||right_y.size()<=min_first_frame_point_thresh){
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
    if(isFirst){
        filter_frame_L=fl;
        filter_frame_R=fr;
    }else{
        if(new_filter_valid_check(fl, filter_frame_L)){
            filter_frame_L=fl;
        }else{
            return false;
        }
        if(new_filter_valid_check(fr, filter_frame_R)){
            filter_frame_R=fr;
        }else{
            return false;
        }
    }
    
    
    /*
     缺失的部分：
    检测新的filter和老的是不是差太多
     
     2.增加第一贞不符合规则return false
     */
#ifdef DEBUG_MODE
    imshow("image",step1.origin_image);
    //waitKey(10);
#endif
    
    return true;
}




int main(int argc, char* argv[]){
    VideoCapture cap;
    /**
     * argument rules:
     *  if there is not aregument, open device zero as default.
     *  if there is one arguement, assume it is a single digit
     *      that represent the device number.
     *  if there is one areguement and it is not a single digit,
     *      treat it as a local address of a video file.
     */
    if(argc==1){
        cap.open(0);
    }else if(argc>=2){
        if(strlen(argv[1])==1 && isdigit(argv[1][0])){
            cap.open(argv[1][0]-'0');
        }else{
            cap.open(argv[1]);
        }
    }
    if(!cap.isOpened()){
        cout<<"video stream cannot be correctly opened.\n";
        return EXIT_FAILURE;   //if the video stream is failed to open.
    }
    
    Mat frame;
    cap>>frame;
    int emptyFrameCount=0;
    bool isFirstFrame=true;
    Mat filter_frame_L,filter_frame_R;
    int follow_up_fail_count=0;
    while(1){
        if(frame.empty()){ //check if read frame is failed
            emptyFrameCount++;
            if(emptyFrameCount>=9){
                cout<<"fail to read frame from stream, or video ended.\n";
                return EXIT_FAILURE;
            }
            waitKey(27);
            cap>>frame;
            continue;
        }
        emptyFrameCount=0;
        //-----------------cycle start------------------
        if(isFirstFrame){
            //first frame identification
            bool success=img_proc(frame,filter_frame_L,filter_frame_R,true);
            if(success){
                isFirstFrame=false;
#ifdef DEBUG_MODE
                //imshow("filter frame l", filter_frame_L);
                //imshow("filter frame r", filter_frame_R);
                waitKey(0);
#endif
            }
        }else{
            //following frames
            //imshow("simply show", frame);
            bool success=img_proc(frame,filter_frame_L,filter_frame_R,false);
            //imshow("filter frame l", filter_frame_L);
            //imshow("filter frame r", filter_frame_R);
            if(!success){
               // isFirstFrame=true;
                follow_up_fail_count++;
                if(follow_up_fail_count==200){
                    follow_up_fail_count=0;
                    isFirstFrame=true;
                }
            }else{
                follow_up_fail_count=0;
            }
            /*
             缺失部分
             建立一个follow up processing function ：用于在拿到左右线filter之后的
             处理，并为下一贞持续update左右线filter
             
             */
            
        }
        //-----------------cycle end--------------------
        waitKey(0);
        cap>>frame;
    }
    
}

#endif
