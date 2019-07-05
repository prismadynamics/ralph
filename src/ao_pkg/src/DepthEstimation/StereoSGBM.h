#ifndef STEREOSGBM_H
#define STEREOSGBM_H
#include "DepthEstimatorStrategy.h"
typedef cv::Mat Image;

class StereoSGBM : public DepthEstimatorStrategy{
    public:
        StereoSGBM();
        ~StereoSGBM();
        /* virtual */ void get_disparity(Image &left, Image &right, Image &dst);
        /* virtual */ void get_depth(Image &left, Image &right, Image &dst);
    protected:
    private:
        cv::Ptr<cv:StereoSGBM> left_matcher;
        cv::Ptr<cv:StereoMatcher> right_matcher;
};
#endif