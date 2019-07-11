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
        virtual ~UndistortStrategy();
        virtual void undistort(const Image& image, cv::OutputArray result) const = 0;
        virtual const cv::Mat& getK() const = 0;
        virtual const cv::Mat& getOriginalK() const = 0;
        virtual int getOutputWidth() const = 0;
        virtual int getOutputHeight() const = 0;
        virtual int getInputWidth() const = 0;
        virtual int getInputHeight() const = 0;
        virtual bool isValid() const = 0;
        static UndistortStrategy* getUndistorterForFile(const char* configFilename);
};
#endif
