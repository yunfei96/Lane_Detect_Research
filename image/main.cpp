#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <stack>

using namespace cv;
using namespace std;

int main(){
	ifstream imgList("list.txt");
	string fname;
	imgList>>fname;
	VideoWriter output;
	const string NAME="Output.avi";
	Mat testImg = imread(fname, CV_LOAD_IMAGE_COLOR); 
	Size S = Size(testImg.cols,testImg.rows);
	output.open(NAME, -1, 25, S, true);
	while(!imgList.eof()){
		if(fname==""){
			imgList>>fname;
			continue;
		}
		Mat image;
    	image = imread(fname, CV_LOAD_IMAGE_COLOR); 
    	if(!image.empty()){
    		output<<image;
    	}


		imgList>>fname;
	}
	imgList.close();
	return 0;
}