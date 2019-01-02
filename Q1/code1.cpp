
#include<iostream>
#include<string>
#include<opencv2/opencv.hpp>
#include<sys/stat.h>
#include<unistd.h>

using namespace std;
using namespace cv;

int main(int s,char *argv[]) {

	if(s != 3) printf("Format is splitvideo [SOURCE] [DEST FOLDER] \n");
	
	string filename = string(argv[1]);
	VideoCapture cap(filename);
       		
	if(!cap.isOpened()) {
		printf("Error in opening file");
		return -1;
	}
	if(mkdir(argv[2],0777) == -1) {
		printf("Failed to create Directory");
		return -1;
	}

	Mat frame;
	string frameprefix = string(filename.c_str());
	frameprefix.replace(frameprefix.begin() + filename.rfind("."),frameprefix.end(),"");
	string dest = string(argv[2]) + "/" + frameprefix + "_";

	printf("Writing frames to %s as jpg images \n",argv[2]);

	while(cap.read(frame)) {
		int fno = cap.get(CV_CAP_PROP_POS_FRAMES)-1;
		string saveframe = dest + to_string(fno) + ".jpg";
		imwrite(saveframe.c_str(),frame);
		printf("Saved %s \n",saveframe.c_str());
	}
	
}



