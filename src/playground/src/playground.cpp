/** @file playground.h
 *  @brief main file to test out different ideas.
 * 
 *  @author Ao Y. Yu 
 *  @bug No known bugs.
 */

#include <memory>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <sstream>


#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/ximgproc.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/calib3d.hpp"

#include "DepthEstimation/DepthEstimatorStrategy.h"
#include "DepthEstimation/StereoSGBMConcrete.h"
#include "DepthEstimation/StereoBMConcrete.h"

#include "util/ImageUtility.h"
#include "util/DataLoader.hpp"

void playground_featurepoint_detect(){
	DataLoader data_loader;
	std::string dir1 = std::string("/media/prismadynamics/Elements/KITTI/raw/2011_09_29/2011_09_29_drive_0071_sync/image_00/data/");
	std::vector<std::string> files = std::vector<std::string>();
	
	data_loader.getkittidir(dir1, files);
	std::vector<cv::Mat> vector_src = data_loader.loadimages(dir1, files);
	int minHessian = 400;
	cv::Ptr<cv::xfeatures2d::SURF> detector = cv::xfeatures2d::SURF::create( minHessian );
	std::vector<cv::KeyPoint> keypoints;
	cv::Mat img_keypoints;
	cv::namedWindow("Display window", cv::WINDOW_NORMAL);// Create a window for display.
	for(int i = 0; i < vector_src.size(); i++){
		detector->detect( vector_src[i], keypoints );
        cv::drawKeypoints(vector_src[i], keypoints, img_keypoints, cv::Scalar(254) );
		cv::imshow("Display window",img_keypoints);
    	cv::waitKey(20);     
	}
    cv::waitKey(0); 
}


void stereo_playground(){
	DataLoader data_loader;
	std::shared_ptr<DepthEstimatorStrategy> depthestimate_sgbm(new StereoSGBMConcrete());
	depthestimate_sgbm->create();

	std::string dir_left = std::string("/media/prismadynamics/Elements/KITTI/raw/2011_09_29/2011_09_26_drive_0051_sync/image_00/data/");
	std::vector<std::string> files_left = std::vector<std::string>();
	data_loader.getkittidir(dir_left, files_left);
	std::vector<cv::Mat> vector_src_left = data_loader.loadimages(dir_left, files_left);

	std::string dir_right = std::string("/media/prismadynamics/Elements/KITTI/raw/2011_09_29/2011_09_26_drive_0051_sync/image_01/data/");
	std::vector<std::string> files_right = std::vector<std::string>();
	data_loader.getkittidir(dir_right, files_right);
	std::vector<cv::Mat> vector_src_right = data_loader.loadimages(dir_right, files_right);

	cv::Mat dst;
	cv::namedWindow("Disparity", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("Original Left", cv::WINDOW_NORMAL);
	cv::namedWindow("Original Right", cv::WINDOW_NORMAL);
	for(int i = 0; i < files_left.size(); i++){
		cv::GaussianBlur(vector_src_left[i],vector_src_left[i], cv::Size(5,5), 0, 0);
		cv::GaussianBlur(vector_src_right[i],vector_src_right[i], cv::Size(5,5), 0, 0);
		depthestimate_sgbm->get_disparity(vector_src_left[i],vector_src_right[i], dst);
		depthestimate_sgbm->get_disparity_viz(dst,dst);
		cv::imshow("Disparity",dst);
		cv::imshow("Original Left",vector_src_left[i]);
		cv::imshow("Original Right",vector_src_right[i]);
		cv::waitKey(10);
	}
}

#include "util/UndistortStrategy.h"
#include "util/OpenCVUndistortConcrete.h"
#include "util/CameraCalibration.hpp"

void undistort_playground(){
	std::string image_dir = std::string("/media/prismadynamics/Elements/camera_calibration/mono_calibration/1280x720/left-0008.png");
	std::string calib_dir = std::string("/media/prismadynamics/Elements/camera_calibration/mono_calibration/1280x720/ost.yaml");
	CameraCalibration camerCal;
	camerCal.from_file(calib_dir);
	std::shared_ptr<UndistortStrategy> undistort_util(new OpenCVUndistortConcrete(camerCal));
	std::shared_ptr<ImageUtility> ImgUtil(new ImageUtility());

	cv::Mat original_img = cv::imread(image_dir);
	cv::Mat undistort_img;
	undistort_util->undistort(original_img, undistort_img);
	cv::namedWindow("Original", cv::WINDOW_NORMAL);
	cv::namedWindow("Undistorted", cv::WINDOW_NORMAL);
	cv::imshow("Original",original_img);
	cv::imshow("Undistorted",undistort_img);
	cv::waitKey(0);
}
void stereo_rectify_playground(){
	std::string master_dir = "/media/prismadynamics/Elements/camera_calibration/stereo_calibration/2560x960/";
	std::string left_image_dir = std::string(master_dir + "left-0023.png");
	std::string right_image_dir = std::string(master_dir + "right-0023.png");

	std::string left_calib_dir = std::string(master_dir + "left.yaml");
	std::string right_calib_dir = std::string(master_dir + "right.yaml");

	CameraCalibration left_cameraCal, right_cameraCal;
	left_cameraCal.from_file(left_calib_dir);
	right_cameraCal.from_file(right_calib_dir);

	left_cameraCal.set_is_stereo(true);
	right_cameraCal.set_is_stereo(true);
	left_cameraCal.set_baseline(1.0);
	right_cameraCal.set_baseline(1.0);
	right_cameraCal.set_rectification_matrix(left_cameraCal.get_rectification_matrix());

	std::shared_ptr<UndistortStrategy> left_undistort(new OpenCVUndistortConcrete(left_cameraCal));
	std::shared_ptr<UndistortStrategy> right_undistort(new OpenCVUndistortConcrete(right_cameraCal));

	cv::Mat left_original = cv::imread(left_image_dir);
	cv::Mat right_original = cv::imread(right_image_dir);
	cv::namedWindow("left_original", cv::WINDOW_NORMAL);
	cv::namedWindow("right_original", cv::WINDOW_NORMAL);
	cv::imshow("left_original",left_original);
	cv::imshow("right_original",right_original);


	cv::Mat left_rect;
	cv::Mat right_rect;
	left_undistort->undistort(left_original, left_rect);
	right_undistort->undistort(right_original, right_rect);

	cv::namedWindow("left_rect", cv::WINDOW_NORMAL);
	cv::namedWindow("right_rect", cv::WINDOW_NORMAL);
	cv::imshow("left_rect",left_rect);
	cv::imshow("right_rect",right_rect);


	std::shared_ptr<DepthEstimatorStrategy> depthestimate_sgbm(new StereoSGBMConcrete());
	depthestimate_sgbm->create();
	cv::Mat dst;

	//cv::GaussianBlur(left_rect,left_rect, cv::Size(5,5), 0, 0);
	//cv::GaussianBlur(right_rect,right_rect, cv::Size(5,5), 0, 0);
	depthestimate_sgbm->get_disparity(left_rect,right_rect, dst);
	depthestimate_sgbm->get_disparity_viz(dst,dst);
	cv::namedWindow("Disparity", cv::WINDOW_NORMAL);
	cv::imshow("Disparity",dst);
	cv::waitKey(0);
}

void stereo_test(){
	std::string master_dir = "/home/prismadynamics/";
	cv::Mat src = cv::imread(master_dir + "my_photo-9.jpg");
	std::string left_calib_dir = std::string("/media/prismadynamics/Elements/camera_calibration/stereo_calibration/2560x960/left.yaml");
	std::string right_calib_dir = std::string("/media/prismadynamics/Elements/camera_calibration/stereo_calibration/2560x960/right.yaml");

	std::shared_ptr<ImageUtility> ImgUtil(new ImageUtility());
	cv::Mat left_original, right_original;
	ImgUtil->split_image(src, left_original, right_original);

	cv::namedWindow("left_original", cv::WINDOW_NORMAL);
	cv::namedWindow("right_original", cv::WINDOW_NORMAL);
	cv::imshow("left_original",left_original);
	cv::imshow("right_original",right_original);

	CameraCalibration left_cameraCal, right_cameraCal;
	left_cameraCal.from_file(left_calib_dir);
	right_cameraCal.from_file(right_calib_dir);

	left_cameraCal.set_is_stereo(true);
	right_cameraCal.set_is_stereo(true);
	left_cameraCal.set_baseline(1.0);
	right_cameraCal.set_baseline(1.0);
	//right_cameraCal.set_rectification_matrix(left_cameraCal.get_rectification_matrix());

	std::shared_ptr<UndistortStrategy> left_undistort(new OpenCVUndistortConcrete(left_cameraCal));
	std::shared_ptr<UndistortStrategy> right_undistort(new OpenCVUndistortConcrete(right_cameraCal));
	cv::Mat left_rect;
	cv::Mat right_rect;
	left_undistort->undistort(left_original, left_rect);
	right_undistort->undistort(right_original, right_rect);
	cv::namedWindow("left_rect", cv::WINDOW_NORMAL);
	cv::namedWindow("right_rect", cv::WINDOW_NORMAL);
	cv::imshow("left_rect",left_rect);
	cv::imshow("right_rect",right_rect);

	std::shared_ptr<DepthEstimatorStrategy> depthestimate_sgbm(new StereoSGBMConcrete());
	depthestimate_sgbm->create();
	cv::Mat dst;
	//cv::GaussianBlur(left_rect,left_rect, cv::Size(5,5), 0, 0);
	//cv::GaussianBlur(right_rect,right_rect, cv::Size(5,5), 0, 0);
	depthestimate_sgbm->get_disparity(left_rect,right_rect, dst);
	depthestimate_sgbm->get_disparity_viz(dst,dst);
	cv::namedWindow("Disparity", cv::WINDOW_NORMAL);
	cv::imshow("Disparity",dst);
	cv::waitKey(0);
}

///////////////////////////////// OpenCV ROS ////////////////////////
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

static const std::string OPENCV_WINDOW = "Image window";

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

public:
  ImageConverter() : it_(nh_)
  {
    image_sub_ = it_.subscribe("/kitti/camera_color_left/image_raw", 1,
      &ImageConverter::imageCb, this);
    cv::namedWindow(OPENCV_WINDOW);
  }
  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    // Update GUI Window
    cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::waitKey(3);
  }
};

void ros_opencv(int argc, char** argv){
	ros::init(argc, argv, "image_converter");
	ImageConverter ic;
	ros::spin();
}
///////////////////////////////// OpenCV ROS ////////////////////////

void LOG(std::string text){
	std::cout << text << std::endl;
}
int main(int argc, char** argv){
	ros_opencv(argc, argv);
	return 0;
}
