
#include<iostream>
#include<string>
#include<opencv2/opencv.hpp>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<vector>
#include<cstdio>


using namespace std;
using namespace cv;

int main(int s,char *argv[]) {

	if(s != 4) printf("Format is combinevideo [OUTPUT] [SOURCE FOLDER] [FRAMERATE] \n"
		"Images in folder should be readable in scanf format [name]_[frame_no].[type]"
		"\n It is assumed that frame nos start from 0 and there are no missing frames\n");
	//get the list of files in the folder
	vector<string> file_list;
	DIR *dir;
	struct dirent *dir_entry; 

	if((dir = opendir(argv[2])) != NULL) {
		while((dir_entry = readdir(dir)) != NULL) {
			if(dir_entry->d_name[0] != '.')
				file_list.push_back(string(dir_entry->d_name));
		}
		closedir(dir);
	}
	else {
		printf("Error opening folder ");
		return -1;
	}
	
	//sort the files according to frame number in another vector
	vector<string> sorted_frames(file_list.size());
	for(auto i = file_list.begin(); i != file_list.end(); i++) {
		int a = (*i).find("_"), b = (*i).find(".");
		int fno = atoi(((*i).substr(a + 1, b - a - 1)).c_str());
		sorted_frames[fno] = *i;
	}

	//combine the frames into a video
	char c_dir[512];
	getcwd(c_dir,512);
	chdir(argv[2]);
	auto i = sorted_frames.begin();
	Mat frame = imread((*i).c_str(),1);
	VideoWriter output_video(string(c_dir) + string("/")  + string(argv[1]),
		VideoWriter::fourcc('M','P','4','2'),(double)atoi(argv[3]),frame.size(),true);
	if(!output_video.isOpened()) {
		printf("Error creating video output file \n");
		return -1;
	}
	output_video.write(frame);
	while( ++i != sorted_frames.end()) {
		frame = imread((*i).c_str(),1);
		output_video.write(frame);
	}
	return 0;

}



