/** @file OpenCVUndistoryConcrete.cpp
 *  @brief Concrete implimenetaion for undistorting an image using OpenCV's native algorithms.
 * 
 *  @author Ao Y. Yu 
 *  @bug No known bugs.
 */

#include "OpenCVUndistortConcrete.h"

OpenCVUndistortConcrete::OpenCVUndistortConcrete(const char* configFileName){
	valid = true;
	
	// read parameters
	std::ifstream infile(configFileName);
	assert(infile.good());

	std::string l1, l2, l3, l4;

	std::getline(infile,l1);
	std::getline(infile,l2);
	std::getline(infile,l3);
	std::getline(infile,l4);

	// l1 & l2
	if(std::sscanf(l1.c_str(), "%f %f %f %f %f %f %f %f",
		&inputCalibration[0], &inputCalibration[1], &inputCalibration[2], &inputCalibration[3], &inputCalibration[4],
		&inputCalibration[5], &inputCalibration[6], &inputCalibration[7]) == 8 && std::sscanf(l2.c_str(), "%d %d", &in_width, &in_height) == 2)
	{
		printf("Input resolution: %d %d\n",in_width, in_height);
		printf("In: %f %f %f %f %f %f %f %f\n",
				inputCalibration[0], inputCalibration[1], inputCalibration[2], inputCalibration[3], inputCalibration[4],
				inputCalibration[5], inputCalibration[6], inputCalibration[7]);
	}
	else{
		printf("Failed to read camera calibration (invalid format?)\nCalibration file: %s\n", configFileName);
		valid = false;
	}

	// l3
	if(l3 == "crop"){
		outputCalibration = -1;
		printf("Out: Crop\n");
	}
	else if(l3 == "full"){
		outputCalibration = -2;
		printf("Out: Full\n");
	}
	else if(l3 == "none"){
		printf("NO RECTIFICATION\n");
		valid = false;
	}
	else{
		printf("Out: Failed to Read Output pars... not rectifying.\n");
		valid = false;
	}

	// l4
	if(std::sscanf(l4.c_str(), "%d %d", &out_width, &out_height) == 2){
		printf("Output resolution: %d %d\n", out_width, out_height);
	}
	else{
		printf("Out: Failed to Read Output resolution... not rectifying.\n");
		valid = false;
	}
	cv::Mat distCoeffs = cv::Mat::zeros(4, 1, CV_32F);
	for(int i = 0; i < 4; ++i){
		distCoeffs.at<float>(i, 0) = inputCalibration[4 + i];
	}
	originalK_ = cv::Mat(3, 3, CV_64F, cv::Scalar(0));
	originalK_.at<double>(0, 0) = inputCalibration[0] * in_width;
	originalK_.at<double>(1, 1) = inputCalibration[1] * in_height;
	originalK_.at<double>(2, 2) = 1;
	originalK_.at<double>(0, 2) = inputCalibration[2] * in_width;
	originalK_.at<double>(1, 2) = inputCalibration[3] * in_height;

	if (valid){
		K_ = cv::getOptimalNewCameraMatrix(originalK_, distCoeffs, cv::Size(in_width, in_height), (outputCalibration == -2) ? 1 : 0, cv::Size(out_width, out_height), nullptr, false);
		cv::initUndistortRectifyMap(originalK_, distCoeffs, cv::Mat(), K_,
		cv::Size(out_width, out_height), CV_16SC2, map1, map2);
		
		originalK_.at<double>(0, 0) /= in_width;
		originalK_.at<double>(0, 2) /= in_width;
		originalK_.at<double>(1, 1) /= in_height;
		originalK_.at<double>(1, 2) /= in_height;
	}
	originalK_ = originalK_.t();
	K_ = K_.t();
	assert(map1.rows > 0 && map1.cols > 0 && map2.rows > 0 && map2.cols > 0);
}

OpenCVUndistortConcrete::OpenCVUndistortConcrete(CameraCalibration cameraCalibration){
	valid = true;
	cv::Mat distCoeffs = cameraCalibration.get_distortion_coefficients();
	originalK_ = cameraCalibration.get_camera_matrix();
    in_width = cameraCalibration.get_image_width();
    in_height = cameraCalibration.get_image_height();
    out_width = in_width;
    out_height = in_height;
    std::string crop_mode = cameraCalibration.get_crop();

	if(crop_mode == "crop"){
		outputCalibration = -1;
	}else if(crop_mode == "full"){
		outputCalibration = -2;
	}else if(crop_mode == "none"){
		valid = false;
	}else{
		printf("Out: Failed to Read Output pars... not rectifying.\n");
		valid = false;
	}

    if(!(in_width > 0 && in_height > 0)){
    	valid = false;
    }else if(!(distCoeffs.rows == 1 && distCoeffs.cols == 5)){
    	valid = false;
    }else if(!(originalK_.rows == 3 && originalK_.cols == 3)){
    	valid = false;
    }else if(cameraCalibration.get_is_stereo() && cameraCalibration.get_baseline() <= 0.0){
    	valid = false;
    }else{
    	valid = true;
    }

  	if(valid){
  		K_ = cv::getOptimalNewCameraMatrix(originalK_, distCoeffs, cv::Size(in_width, in_height), (outputCalibration == -2) ? 1 : 0, cv::Size(out_width, out_height), nullptr, false);
		if(cameraCalibration.get_is_stereo() && cameraCalibration.get_baseline() > 0.0){
			cv::initUndistortRectifyMap(originalK_, distCoeffs, cameraCalibration.get_rectification_matrix(), K_,cv::Size(out_width, out_height), CV_16SC2, map1, map2);
			//cv::initUndistortRectifyMap(originalK_, distCoeffs, cv::Mat(), K_,cv::Size(out_width, out_height), CV_16SC2, map1, map2);

		}else{
			cv::initUndistortRectifyMap(originalK_, distCoeffs, cv::Mat(), K_,cv::Size(out_width, out_height), CV_16SC2, map1, map2);
		}

  	}
  	originalK_ = originalK_.t();
	K_ = K_.t();
	assert(map1.rows > 0 && map1.cols > 0 && map2.rows > 0 && map2.cols > 0);
}

OpenCVUndistortConcrete::OpenCVUndistortConcrete(){

}

OpenCVUndistortConcrete::~OpenCVUndistortConcrete(){

}

void OpenCVUndistortConcrete::print_debug(){
    std::cout << "in_width: " << in_width << std::endl;
    std::cout << "in_height: " << in_height << std::endl;
    std::cout << "out_width: " << out_width << std::endl;
    std::cout << "out_height: " << out_height << std::endl;
    std::cout << "valid: " << valid << std::endl;
    std::cout << "K_: " << K_ << std::endl;
    std::cout << "originalK_: " << originalK_ << std::endl;
}

void OpenCVUndistortConcrete::undistort(const Image& image, cv::OutputArray& result){
	cv::remap(image, result, map1, map2, cv::INTER_LINEAR);
}

const cv::Mat& OpenCVUndistortConcrete::getK(){
	return K_;
}

const cv::Mat& OpenCVUndistortConcrete::getOriginalK(){
	return originalK_;
}
int OpenCVUndistortConcrete::getOutputWidth(){
	return out_width;
}
int OpenCVUndistortConcrete::getOutputHeight(){
	return out_height;
}
int OpenCVUndistortConcrete::getInputWidth(){
	return in_width;
}
int OpenCVUndistortConcrete::getInputHeight(){
	return in_height;
}
bool OpenCVUndistortConcrete::isValid(){
	return valid;
}
