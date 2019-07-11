/** @file OpenCVUndistortConcrete.h
 *  @brief Concrete implimenetaion for undistorting an image using OpenCV's native algorithms.
 * 
 *  @author Ao Y. Yu 
 *  @bug No known bugs.
 */

#ifndef OPENCVUNDISTORTCONCRETE_H
#define OPENCVUNDISTORTCONCRETE_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <Eigen/Core>
#include "util/settings.h"

typedef cv::Mat Image;

class OpenCVUndistortConcrete : public UndistortStrategy{
    public:
        /** @brief Creates an Undistorter by reading the distortion parameters from a file.
            * 
            *       The calibration file format is as follows:
            *       d1 d2 d3 d4 d5
            *       inputWidth inputHeight
            *       crop / full / none
            *       outputWidth outputHeight
            * 
            *  @param configFileName path to the calibration file.
            *  @return void
            */
    	OpenCVUndistortConcrete(const char* configFileName);
        /** @brief Creates an Undistorter by reading the distortion parameters from the CameraCalibration struct.
            *
            *  @param cameraCalibration camera calibration structure.
            *  @return void
            */
        OpenCVUndistortConcrete(CameraCalibration cameraCalibration);
        OpenCVUndistortConcrete();
        ~OpenCVUndistortConcrete();
        OpenCVUndistortConcrete(const OpenCVUndistortConcrete&) = delete;
        OpenCVUndistortConcrete& operator=(const OpenCVUndistortConcrete&) = delete;
        /** @brief undistort Undistorts the given image and returns the result image.
            * 
            *  @param image A reference of the source image.
            *  @param result A reference of the pyramid data structure. Structure contain vector of images.
            *  @return void
            */
        /* virtual */ void undistort(const Image& image, cv::OutputArray result);
        /** @brief getK Returns the intrinsic parameter matrix of the undistorted images.
            * 
            *  @return cv::Mat
            */
        /* virtual */ const cv::Mat& getK();
        /** @brief getOriginalK Returns the intrinsic parameter matrix of the original images.
            * 
            *  @return cv::Mat
            */
        /* virtual */ const cv::Mat& getOriginalK();
        /** @brief getOutputWidth Returns the width of the undistorted images in pixels.
            * 
            *  @return int
            */
        /* virtual */ int getOutputWidth();
        /** @brief getOutputHeight Returns the height of the undistorted images in pixels.
            * 
            *  @return int
            */        
        /* virtual */ int getOutputHeight();
        /** @brief getInputWidth Returns the width of the input images in pixels.
            * 
            *  @return int
            */               
        /* virtual */ int getInputWidth();
        /** @brief getInputWidth Returns the height of the input images in pixels.
            * 
            *  @return int
            */                    
        /* virtual */ int getInputHeight();
        /** @brief Returns if the undistorter was initialized successfully.
            * 
            *  @return bool
            */                    
        /* virtual */ bool isValid();
    private:
        cv::Mat K_;
        cv::Mat originalK_;
        float inputCalibration[10];
        float outputCalibration;
        int out_width, out_height;
        int in_width, in_height;
        cv::Mat map1, map2;
        
        /// Is true if the undistorter object is valid (has been initialized with
        /// a valid configuration)
        bool valid;
};

#endif
