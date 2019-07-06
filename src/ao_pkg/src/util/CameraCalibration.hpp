/** @file CameraCalibration.hpp
 *  @brief This structure parses camera calibration files in .yaml format typically outputted by 
 *         ROS's default camera calibration. The .yaml file is assumed to be standard format. Addtional
 *         camera parameters, extrinsicts or ID, are also impliemented. 
 *
 *  This contains definition for data structures and enumerations. 
 *
 *  @author Ao Y. Yu 
 *  @bug No known bugs.
 *  @example 
 *  	#define USE_EIGEN
 *	#include <iostream>
 *	#include <string>
 *	#include "CameraCalibration.hpp"
 *
 *	int main()
 *	{
 *	  std::string cal_file = "~/Desktop/camera_cal_parser/example.yaml";
 *	  CameraCalibration camcal;
 *	  camcal.from_file(cal_file);
 *	  std::cout << "OpenCV Datastructures: " << "\n";
 *	  std::cout << camcal.opencv_get_camera_matrix() << "\n\n";
 *	  std::cout << camcal.opencv_get_distortion_coefficients() << "\n\n";
 *	  std::cout << camcal.opencv_get_rectification_matrix() << "\n\n";
 *	  std::cout << camcal.opencv_get_projection_matrix() << "\n\n";
 *	  return 0;
 *	}
 */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <assert.h>

#include <boost/algorithm/string.hpp>

#ifdef USE_EIGEN
#include <Eigen/Core>
#endif
#include <opencv2/core/core.hpp>

struct CameraCalibration{
  CameraCalibration(){
  }
  ~CameraCalibration(){
  }

  int camera_id = 0;
  int image_width = 0;
  int image_height = 0;
  float stereo_baseline = 0.0; // in meters.
  std::string camera_name = "";
  std::string distortion_model = "";

  std::vector<std::string> CameraPramatersStrings = { "image_width", "image_height", "camera_name", "camera_matrix", "distortion_model", 
                                                  "distortion_coefficients", "rectification_matrix", "projection_matrix"};
  std::vector<std::string> CameraSubPramatersStrings = { "rows", "cols", "data"};

  cv::Mat camera_matrix = cv::Mat::zeros(cv::Size(3, 3), CV_32F);
  cv::Mat distortion_coefficients = cv::Mat::zeros(cv::Size(5, 1), CV_32F);
  cv::Mat rectification_matrix = cv::Mat::zeros(cv::Size(3, 3), CV_32F);
  cv::Mat projection_matrix = cv::Mat::zeros(cv::Size(4, 3), CV_32F);
  cv::Mat extrinsic_matrix = cv::Mat::eye(cv::Size(4, 4), CV_32F);

  // Intermediate variables. This is used to store values for different return types (Eigen or OpenCV).
  float array_camera_matrix[9];
  float array_distortion_coefficients[5];
  float array_rectification_matrix[9];
  float array_projection_matrix[12];

  int get_image_width(){
    return this->image_width;
  }
  int get_image_height(){
    return this->image_height;
  }
  std::string get_camera_name(){
    return this->camera_name;
  }
  std::string get_distortion_model(){
    return this->distortion_model;
  }
  float get_focal_length_x(){
    return this->camera_matrix.at<float>(0,0);
  }
  float get_focal_length_y(){
    return this->camera_matrix.at<float>(1,1);
  }
  float get_optical_center_x(){
    return this->camera_matrix.at<float>(0,2);
  }
  float get_optical_center_y(){
    return this->camera_matrix.at<float>(1,2);
  }
  cv::Mat opencv_get_extrinsics(){
    return this->extrinsic_matrix;
  }
  cv::Mat get_camera_matrix(){
    cv::Mat camera_matrix_temp = cv::Mat::zeros(cv::Size(3, 3), CV_32F);
    for(int i = 0; i < 3; i++){
      for(int j = 0; j < 3; j++){
        camera_matrix_temp.at<float>(i, j) = array_camera_matrix[i*3+j];
      }
    }
    assert((camera_matrix_temp.rows == 3 && camera_matrix_temp.cols == 3));
    return camera_matrix_temp;
  }
  cv::Mat get_distortion_coefficients(){
    cv::Mat distortion_coefficients_temp = cv::Mat::zeros(cv::Size(5, 1), CV_32F);
    for(int i = 0; i < (sizeof(array_distortion_coefficients)/sizeof(*array_distortion_coefficients)); i++){
        distortion_coefficients_temp.at<float>(0, i) = array_distortion_coefficients[i];
    }
    assert((distortion_coefficients_temp.rows == 1 && distortion_coefficients_temp.cols == 5));
    return distortion_coefficients_temp;
  }
  cv::Mat get_rectification_matrix(){
    cv::Mat rectification_matrix_temp = cv::Mat::zeros(cv::Size(3, 3), CV_32F);
    for(int i = 0; i < 3; i++){
      for(int j = 0; j < 3; j++){
        rectification_matrix_temp.at<float>(i, j) = array_rectification_matrix[i*3+j];
      }
    }
    assert((rectification_matrix_temp.rows == 3 && rectification_matrix_temp.cols == 3));
    return rectification_matrix_temp;
  }
  cv::Mat get_projection_matrix(){
    cv::Mat projection_matrix_temp = cv::Mat::zeros(cv::Size(4, 3), CV_32F);
    for(int i = 0; i < 3; i++){
      for(int j = 0; j < 4; j++){
        projection_matrix_temp.at<float>(i, j) = array_projection_matrix[i*3+j];
      }
    }
    assert((projection_matrix_temp.rows == 3 && projection_matrix_temp.cols == 4));
    return projection_matrix_temp;
  }

#ifdef USE_EIGEN
  Eigen::Matrix3f eigen_camera_matrix;
  Eigen::Matrix<float, 1, 5> eigen_distortion_coefficients;
  Eigen::Matrix3f eigen_rectification_matrix;
  Eigen::Matrix<float, 3, 4> eigen_projection_matrix;

  Eigen::Matrix3f eigen_get_camera_matrix(){
    for(int i = 0; i < 3; i++){
      for(int j = 0; j < 3; j++){
        eigen_camera_matrix(i, j) = array_camera_matrix[i*3+j];
      }
    }
    return eigen_camera_matrix;
  }

  Eigen::Matrix<float, 1, 5> eigen_get_distortion_coefficients(){
    for(int i = 0; i < (sizeof(array_distortion_coefficients)/sizeof(*array_distortion_coefficients)); i++){
        eigen_distortion_coefficients(0, i) = array_distortion_coefficients[i];
    }
    return eigen_distortion_coefficients;
  }

  Eigen::Matrix3f eigen_get_rectification_matrix(){
    for(int i = 0; i < 3; i++){
      for(int j = 0; j < 3; j++){
        eigen_rectification_matrix(i, j) = array_rectification_matrix[i*3+j];
      }
    }
    return eigen_rectification_matrix;
  }

  Eigen::Matrix<float, 3, 4> eigen_get_projection_matrix(){
    for(int i = 0; i < 3; i++){
      for(int j = 0; j < 4; j++){
        eigen_projection_matrix(i, j) = array_projection_matrix[i*3+j];
      }
    }
    return eigen_projection_matrix;
  }
#endif

  int from_file(const std::string cal_file){
      std::string line;
      std::ifstream myfile (cal_file);
      std::string CameraParamter;
      if (myfile.is_open())
      {
        while ( std::getline (myfile,line) )
        {
          std::string CameraParameter, Value;
          boost::algorithm::erase_all(line, " ");
          std::vector<std::string> SplitVec;
          boost::algorithm::split(SplitVec, line, boost::algorithm::is_any_of(":"), boost::algorithm::token_compress_on);
          boost::algorithm::trim_left_if(SplitVec[1], boost::algorithm::is_any_of("["));
          boost::algorithm::trim_right_if(SplitVec[1], boost::algorithm::is_any_of("]"));
          if(in_array(SplitVec[0], CameraSubPramatersStrings)){
            // This branch is for if current line is a camera sub parameter of CameraParameter
            if(SplitVec[0] == "data"){
              this->parse_calibration(CameraParamter, SplitVec[1]);
            }
          }else if(in_array(SplitVec[0], CameraPramatersStrings)){
            CameraParamter = SplitVec[0];
          }
        }
        myfile.close();
      }
      camera_matrix = this->get_camera_matrix();
      distortion_coefficients = this->get_distortion_coefficients();
      rectification_matrix = this->get_rectification_matrix();
      projection_matrix = this->get_projection_matrix();
      return 0;
  }

  void parse_calibration(std::string CameraParameter, std::string Value){
    std::vector<std::string> matrix_elements;
    if(CameraParameter == "image_width"){
      image_width = std::atoi(Value.c_str());
    }else if(CameraParameter == "image_height"){
      image_height = std::atoi(Value.c_str());
    }else if(CameraParameter == "camera_name"){
      camera_name = Value;
    }else if(CameraParameter == "camera_matrix"){
      matrix_elements = get_matrix_elements(Value);
      for(int i = 0; i < (sizeof(array_camera_matrix)/sizeof(*array_camera_matrix)); i++){
        array_camera_matrix[i] = std::stof(matrix_elements[i]);
      }
    }else if(CameraParameter == "distortion_model"){
      distortion_model = Value;
    }else if(CameraParameter == "distortion_coefficients"){
      matrix_elements = get_matrix_elements(Value);
      for(int i = 0; i < (sizeof(array_distortion_coefficients)/sizeof(*array_distortion_coefficients)); i++){
        array_distortion_coefficients[i] = std::stof(matrix_elements[i]);
      }
    }else if(CameraParameter == "rectification_matrix"){
      matrix_elements = get_matrix_elements(Value);
      for(int i = 0; i < (sizeof(array_rectification_matrix)/sizeof(*array_rectification_matrix)); i++){
        array_rectification_matrix[i] = std::stof(matrix_elements[i]);
      }
    }else if(CameraParameter == "projection_matrix"){
      matrix_elements = get_matrix_elements(Value);
      for(int i = 0; i < (sizeof(array_projection_matrix)/sizeof(*array_projection_matrix)); i++){
        array_projection_matrix[i] = std::stof(matrix_elements[i]);
      }
    }else{
      std::cout << "Unsupported Calibration Parameter." << std::endl;
    }
  }
  std::vector<std::string> get_matrix_elements(std::string Value){
    std::vector<std::string> matrix_elements;
    matrix_elements = boost::algorithm::split(matrix_elements, Value, boost::algorithm::is_any_of(","), boost::algorithm::token_compress_on);
    return matrix_elements;
  }

  bool in_array(const std::string &value, const std::vector<std::string> &array)
  {
      return std::find(array.begin(), array.end(), value) != array.end();
  }

  void pretty_print(){
    // TO_DO: Impliement pretty printing.
  }
};
