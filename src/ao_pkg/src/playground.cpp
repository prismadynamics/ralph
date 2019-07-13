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

void LOG(std::string text){
	std::cout << text << std::endl;
}
int main(int argc, char** argv) {
	std::shared_ptr<ImageUtility> ImgUtil(new ImageUtility());
	undistort_playground();
	return 0;
}
