#include<opencv2/opencv.hpp>
#include<iostream>
#include<sys/stat.h>

using namespace std;
using namespace cv;

int main(int s,char *argv[]) {

	if(s != 2) {
		printf("Format is webcamframes [FOLDERNAME]\n");
		return -1;
	}
	if(mkdir(argv[1],0777) == -1) {
		printf("Error in creating Directory");
		return -1;
	}

	printf("Press e to exit\n");

	VideoCapture cap(0);
	namedWindow("Webcam",1);
	Mat frame;
	
	string saveframe = string(argv[1]) + string("/frame_");
	int fno = 0,key;
	while((key = waitKey(1)) != 'c' && cap.read(frame)) {
		imwrite(saveframe + to_string(fno) + string(".jpg"),frame);	
		imshow("Webcam",frame);
		cout << key << endl; 
		fno++;
	}
	return 0;
}



