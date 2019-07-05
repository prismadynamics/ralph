/** @file DepthEstimatorStrategy.h
 *  @brief Purely virtual prototypes of stereo depth estimation.
 *         This class is based on opencv's tutorial 
 *         https://docs.opencv.org/4.1.0/d3/d14/tutorial_ximgproc_disparity_filtering.html 
 * 
 *  @author Ao Y. Yu 
 *  @bug No known bugs.
 */

#ifndef DEPTHESTIMATORSTRATEGY_H
#define DEPTHESTIMATORSTRATEGY_H

#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/ximgproc.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"

#include "DataStructures/CameraCalibration.hpp"

typedef cv::Mat Image;
typedef cv::Mat DisparityMap;
typedef cv::Mat ConfidenceMap;
typedef cv::Mat DepthMap;
class DepthEstimatorStrategy{
    public:
        DepthEstimatorStrategy();
        ~DepthEstimatorStrategy();
        virtual int create();
        virtual void get_disparity(Image &left, Image &right, DisparityMap &dst);
        virtual void get_disparity_viz(DisparityMap &filtered_disp, Image &filtered_disp_vis);
        virtual void get_depth(Image &left, Image &right, DepthMap &dst);
    protected:
        inline cv::Rect computeROI(cv::Size2i src_sz, cv::Ptr<cv::StereoMatcher> matcher_instance){
            int min_disparity = matcher_instance->getMinDisparity();
            int num_disparities = matcher_instance->getNumDisparities();
            int block_size = matcher_instance->getBlockSize();

            int bs2 = block_size/2;
            int minD = min_disparity, maxD = min_disparity + num_disparities - 1;

            int xmin = maxD + bs2;
            int xmax = src_sz.width + minD - bs2;
            int ymin = bs2;
            int ymax = src_sz.height - bs2;

            cv::Rect rect(xmin, ymin, xmax - xmin, ymax - ymin);
            return rect;
        }  
    private:
    
};
#endif