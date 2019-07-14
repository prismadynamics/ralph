/** @file UndistortStrategy.h
 *  @brief Virtual prototypes of for undistorting an image.
 * 
 *  @author Ao Y. Yu 
 *  @bug No known bugs.
 */

#ifndef _UNDISTORTSTRATEGY_H
#define _UNDISTORTSTRATEGY_H

#include <sstream>
#include <fstream>

#include <opencv2/core/core.hpp>

#include "util/CameraCalibration.hpp"

typedef cv::Mat Image;
class UndistortStrategy{
    public:
        virtual ~UndistortStrategy(){};
        virtual void undistort(const Image& image, cv::OutputArray result) {};
        virtual const cv::Mat& getK() {};
        virtual const cv::Mat& getOriginalK() {};
        virtual int getOutputWidth() {};
        virtual int getOutputHeight() {};
        virtual int getInputWidth() {};
        virtual int getInputHeight() {};
        virtual bool isValid() {};
        virtual void print_debug() {};
        static UndistortStrategy* getUndistorterForFile(const char* configFilename);
};
#endif
