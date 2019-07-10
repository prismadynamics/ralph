/** @file OpenCVUndistortConcrete.h
 *  @brief Concrete implimenetaion for undistorting an image using OpenCV's native algorithms.
 * 
 *  @author Ao Y. Yu 
 *  @bug No known bugs.
 */

#ifndef OPENCVUNDISTORTCONCRETE_H
#define OPENCVUNDISTORTCONCRETE_H

#include <sstream>
#include <fstream>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <Eigen/Core>
#include "util/settings.h"

typedef cv::Mat Image;

class OpenCVUndistortConcrete : public UndistortStrategy{
    public:
    	OpenCVUndistortConcrete(const char* configFileName);
        OpenCVUndistortConcrete();
        ~OpenCVUndistortConcrete();
        OpenCVUndistortConcrete(const OpenCVUndistortConcrete&) = delete;
        OpenCVUndistortConcrete& operator=(const OpenCVUndistortConcrete&) = delete;
        void undistort(const Image& image, cv::OutputArray result) const;
        const cv::Mat& getK() const;
        const cv::Mat& getOriginalK() const;
        int getOutputWidth() const;
        int getOutputHeight() const;
        int getInputWidth() const;
        int getInputHeight() const;
        bool isValid() const;
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
