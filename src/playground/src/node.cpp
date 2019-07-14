//
// Created by root on 11/05/19.
//

#include <ros/ros.h>
#include <sstream>
#include "IOWrapper/ROS/ROSImageStreamThread.h"
#include <X11/Xlib.h>
#include "util/settings.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "opencv2/xfeatures2d.hpp"
using namespace ros;
using namespace std;
using namespace lsd_slam;
int main(int argc, char** argv) {
	XInitThreads();
	ros::init(argc, argv, "AO_SLAM");
	InputImageStream* inputStream_left = new ROSImageStreamThread("image_left");
	InputImageStream* inputStream_right = new ROSImageStreamThread("image");

	std::string calibFile = "/home/prismadynamics/test_ws/src/ao_pkg/calib/OpenCV_example_calib.cfg";
	inputStream_left->setCalibration(calibFile);
	inputStream_right->setCalibration(calibFile);

	inputStream_left->run();
	inputStream_right->run();

	int count = 0;

	cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SIFT::create();
  	std::vector<cv::KeyPoint> keypoints_1, keypoints_2;    

	while(ros::ok()){
		ros::Rate(.1);
		std::cout << "Loop Iteration: " << count << std::endl;
		if(inputStream_left->getBuffer()->size() != 0){
			int size_now = inputStream_left->getBuffer()->size();
			for(int i = 0; i < size_now; i++){
				cv::imshow( "window " + to_string(i), inputStream_left->getBuffer()->popFront().data ); // Show our image inside it.
			}
			cv::waitKey(1000);
			std::cout << "Buffer size: " << inputStream_left->getBuffer()->size() << std::endl;
		}else{
			std::cout << "Buffer size: " << inputStream_left->getBuffer()->size() << std::endl;
		}
		ros::Rate(10);
		count++;
	}
	if (inputStream_left != nullptr){
		delete inputStream_left;
	}
	//if (inputStream_right != nullptr){
		//delete inputStream_right;
	//}
    return 0;
}
