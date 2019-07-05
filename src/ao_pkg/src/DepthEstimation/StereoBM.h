#ifndef STEREOBM_H
#define STEREOBM_H

#include <string>

#include "DepthEstimatorStrategy.h"
typedef cv::Mat Image;
typedef cv::Mat DisparityMap;
typedef cv::Mat ConfidenceMap;
typedef cv::Mat DepthMap;

class StereoBM : public DepthEstimatorStrategy{
    public:
        StereoBM(CameraCalibration cam_cal);
        ~StereoBM();
        /* virtual */ int create();
        /* virtual */ void get_disparity(Image &left, Image &right, DisparityMap &dst);
        /* virtual */ void get_disparity_viz(DisparityMap &filtered_disp, Image &filtered_disp_vis);
        /* virtual */ void get_depth(DisparityMap &src, DepthMap &dst);
    protected:
    private:
        double vis_mult;
        bool no_downscale;
        int max_disp;
        int wsize;
        CameraCalibration cam_cal;
	    DisparityMap left_disp, right_disp;
        DisparityMap filtered_disp,solved_disp,solved_filtered_disp;
        ConfidenceMap conf_map;
        cv::Rect ROI;
        cv::Ptr<cv::ximgproc::DisparityWLSFilter> wls_filter;
        double matching_time, filtering_time;
	    double solving_time = 0;

        std::string filter;
        cv::Ptr<cv::StereoBM> left_matcher;
        cv::Ptr<cv::StereoMatcher> right_matcher;

        void match(Image &left, Image &right);
        void wls_confidence_filter(Image &left, Image &filtered_disp);
        void fbs_confidence_filter(Image &left, Image &filtered_disp);
        void wls_no_confidence_filter(Image &left, Image &filtered_disp);
        void downscale(Image &left, Image &right);

};
#endif