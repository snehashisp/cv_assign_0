#include<cstdio>
#include<iostream>
#include<opencv2/opencv.hpp>
#include<cmath>
#include<cstdlib>

#define HUE_W 1
#define SAT_W 0
#define VAL_W 0

using namespace std;
using namespace cv;

int start;
Vec3b Color;

void mousePoint(int event, int x, int y, int flags, void *data) {

	Mat *frame = (Mat *)data;
	if(event == EVENT_LBUTTONUP) {
		Mat3b c(frame->at<Vec3b>(y,x)), col;
		cout << c << endl;
		if(!start) {
			cvtColor(c,col,COLOR_BGR2HSV);
			Color = col.at<Vec3b>(0,0);
		}
		start = 1;
	}
	return;
}

void composeFrames(Mat &fore, Mat &back, int diff) {

	Mat resback,ret;
	resize(back,resback,fore.size());
	
	for(int i = 0; i < fore.rows; i++) {
		for(int j = 0; j < fore.cols; j++) {
			Mat3b c(fore.at<Vec3b>(i,j)), hsvc;
			Vec3b bc = resback.at<Vec3b>(i,j);
			cvtColor(c,hsvc,COLOR_BGR2HSV);
			Vec3b fc = hsvc.at<Vec3b>(0,0);
			double alpha = 1;

			alpha = 1 - (HUE_W * diff/(abs(fc[0] - Color[0]) + 1) +
				SAT_W * diff/(abs(fc[1] - Color[1]) + 1) + 
				VAL_W * diff/(abs(fc[2] - Color[1]) + 1));
			if(alpha > 1) alpha  = 1;
			else if(alpha < 0) alpha = 0;
			fore.at<Vec3b>(i,j)[0] = alpha * fore.at<Vec3b>(i,j)[0] + (1 - alpha) * 
				resback.at<Vec3b>(i,j)[0];
			fore.at<Vec3b>(i,j)[1] = alpha * fore.at<Vec3b>(i,j)[1] + (1 - alpha) * 
				resback.at<Vec3b>(i,j)[1];
			fore.at<Vec3b>(i,j)[2] = alpha * fore.at<Vec3b>(i,j)[2] + (1 - alpha) * 
				resback.at<Vec3b>(i,j)[2];
		}
	}

}

int main(int s,char *argv[]) {

	if(s < 5 || s == 6 || s > 7) {
		printf("Format chromacombine [FOREGROUND VIDEO] [BACKGROUND VIDEO] [COMBINED VIDEO] "
			"[ALLOWED COLOR VARIATION]. Color variation can range from 0-255.");
		return -1;
	}
	printf("Click on the color to perform chroma keying on on or press 'n' to go to next frame of video\n");
	
	VideoCapture fore(argv[1]);
	VideoCapture back(argv[2]);
	if(!fore.isOpened()) {
		printf("Error opening %s",argv[1]);
		return -1;
	}
	if(!back.isOpened()) {
		printf("Error opening %s",argv[2]);
		return -1;
	}
	
	Mat foreframe, backframe, combineframe;
	fore.read(foreframe); back.read(backframe);
	Mat lut(1,256,CV_8UC3);
	namedWindow("Combine",1);
	setMouseCallback("Combine",mousePoint,(void *)&foreframe);
	int diff = atoi(argv[4]);
	int hres,vres;
	if(s == 7) {
		hres = atoi(argv[5]);
		vres = atoi(argv[6]);
	}
	else {
		hres = foreframe.cols;
		vres = foreframe.rows;
	}
	
	resize(foreframe,foreframe,Size(hres,vres));
	
	VideoWriter outvid(argv[3],fore.get(CV_CAP_PROP_FOURCC),fore.get(CV_CAP_PROP_FPS),foreframe.size(),true);
	
	
	while(!start) {
		resize(foreframe,foreframe,Size(hres,vres));
		imshow("Combine",foreframe);
		char k = waitKey(1);
		if(k == 'n') fore.read(foreframe);
	}
	
	int fno = fore.get(CV_CAP_PROP_POS_FRAMES)-1;
	int total = fore.get(CV_CAP_PROP_FRAME_COUNT) - fno;
	while(fore.read(foreframe)) {
		resize(foreframe,foreframe,Size(hres,vres));
		composeFrames(foreframe,backframe,diff);
		GaussianBlur(foreframe,foreframe,Size(5,5),0,0);
		if(!back.read(backframe)) {
			back.set(CV_CAP_PROP_POS_MSEC,0);
			back.read(backframe);
		}
		imshow("Combine",foreframe);
		outvid.write(foreframe);
		waitKey(1);
		cout << ((float)fno/total) * 100  << endl;
		fno++;
	}
}



		
	


