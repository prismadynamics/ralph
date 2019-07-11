/** @file StereoBMConcrete.h
 *  @brief Concrete implimenetaion of stereo BM matching algorithm.
 *         This class is based on opencv's documentation 
 *         https://docs.opencv.org/4.1.0/d3/d14/tutorial_ximgproc_disparity_filtering.html 
 * 
 *  @author Ao Y. Yu 
 *  @bug No known bugs.
 */

#ifndef STEREOBM_H
#define STEREOBM_H

#include <string>

#include "DepthEstimatorStrategy.h"

class StereoBMConcrete : public DepthEstimatorStrategy{
    public:
        //StereoBMConcrete(CameraCalibration cam_cal); //Instantiate with camera calibrations.
        StereoBMConcrete(); //Instantiate with default values.
        ~StereoBMConcrete();
        /* virtual */ int create();
        /* virtual */ void get_disparity(Image &left, Image &right, DisparityMap &dst);
        /* virtual */ void get_disparity_viz(DisparityMap &filtered_disp, Image &filtered_disp_vis);
        /* virtual */ void get_depth(DisparityMap &src, DepthMap &dst);
    protected:
    private:
        double vis_mult = 1.0;
        bool no_downscale = true;
        //stereo matching patameters
        int max_disp = 160;
        int wsize = 15;
        //filter parameters
        double lambda = 500.0;
        double sigma = 1.0;
        double fbs_spatial = 16.0;
        double fbs_luma = 8.0;
        double fbs_chroma = 8.0;
        double fbs_lambda = 128.0;

        //CameraCalibration cam_cal;
	DisparityMap left_disp, right_disp;
        DisparityMap filtered_disp,solved_disp,solved_filtered_disp;
        ConfidenceMap conf_map;
        cv::Rect ROI;
        cv::Ptr<cv::ximgproc::DisparityWLSFilter> wls_filter;
        double matching_time, filtering_time;
	    double solving_time = 0;
        std::string filter = "wls_conf";


        cv::Ptr<cv::StereoBM> left_matcher;
        cv::Ptr<cv::StereoMatcher> right_matcher;

        void match(Image &left, Image &right);
        void wls_confidence_filter(Image &left, Image &filtered_disp);
        void fbs_confidence_filter(Image &left, Image &filtered_disp);
        void wls_no_confidence_filter(Image &left, Image &filtered_disp);
        void downscale(Image &left, Image &right);

};
#endif
