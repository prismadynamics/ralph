#ifndef QUASIDENSESTEREO_H
#define QUASIDENSESTEREO_H


#include <opencv2/stereo.hpp>

#include "DepthEstimatorStrategy.h"

typedef cv::Mat Image;

class QuasiDenseStereo : public DepthEstimatorStrategy{
    public:
        QuasiDenseStereo();
        ~QuasiDenseStereo();
        /* virtual */ void get_disparity(Image &left, Image &right, Image &dst);
        /* virtual */ void get_depth(Image &left, Image &right, Image &dst);

    protected:
    private:
        cv::Ptr<cv::stereo::QuasiDenseStereo> left_matcher;
        cv::Ptr<cv::StereoMatcher> right_matcher;
};
#endif