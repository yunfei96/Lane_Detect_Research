//
//  detect_video.cpp
//  detect
//
//  Created by Jason Lu on 4/19/18.
//

#include "detect.hpp"
const int min_first_frame_point_thresh = 3;

#ifdef VIDEO_MODE

bool first_frame_img_proc(Mat src, Mat&filter_frame_L, Mat&filter_frame_R){
    preprocess step1(src);
    Mat result = step1.prep_result();
#ifdef DEBUG_MODE
    imshow("step 1 result",result);
    //waitKey(10);
#endif
    vector<vector<double>> white_points = find_white_point(result);
    vector<double> left_x = white_points[0];
    vector<double> left_y = white_points[1];
    vector<double> right_x = white_points[2];
    vector<double> right_y = white_points[3];
    //-----
    //eliminating extreme condition
    if(left_x.size()<=min_first_frame_point_thresh||left_y.size()<=min_first_frame_point_thresh||right_x.size()<=min_first_frame_point_thresh||right_y.size()<=min_first_frame_point_thresh){
        return false;
    }
    draw_line_and_spread_function(step1.origin_image, left_x, left_y);
    draw_line_and_spread_function(step1.origin_image, right_x, right_y);
    /*
     缺失的部分：
     1.产生左边线和右边线的filter用于下一贞处理
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
            bool success=first_frame_img_proc(frame,filter_frame_L,filter_frame_R);
            if(success){
                isFirstFrame=false;
#ifdef DEBUG_MODE
                waitKey(0);
#endif
            }
        }else{
            //following frames
            //imshow("simply show", frame);
            /*
             缺失部分
             建立一个follow up processing function ：用于在拿到左右线filter之后的
             处理，并为下一贞持续update左右线filter
             
             */
            
        }
        //-----------------cycle end--------------------
        waitKey(27);
        cap>>frame;
    }
    
}

#endif
