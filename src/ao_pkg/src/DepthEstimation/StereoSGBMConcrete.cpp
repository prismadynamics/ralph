/** @file StereoSGBMConcrete.cpp
 *  @brief Concrete implimenetaion of stereo BM matching algorithm.
 *         This class is based on opencv's documentation 
 *         https://docs.opencv.org/4.1.0/d3/d14/tutorial_ximgproc_disparity_filtering.html 
 * 
 *  @author Ao Y. Yu 
 *  @bug No known bugs.
 */

#define HAVE_EIGEN
#include "StereoSGBMConcrete.h"

StereoSGBMConcrete::StereoSGBMConcrete(CameraCalibration cam_cal){
    conf_map = cv::Mat(cam_cal.height,cam_cal.width,CV_8U);
	conf_map = cv::Scalar(255);
}
StereoSGBMConcrete::StereoSGBMConcrete(){
}

StereoSGBMConcrete::~StereoSGBMConcrete(){

}
int StereoSGBMConcrete::create(){
    if(max_disp<=0 || max_disp%16!=0)
	{
		std::cout<<"Incorrect max_disparity value: it should be positive and divisible by 16"<< std::endl;
		return -1;
	}
	if(wsize<=0 || wsize%2!=1)
	{
		std::cout<<"Incorrect window_size value: it should be positive and odd"<< std::endl;
		return -1;
	}
    left_matcher = cv::StereoSGBM::create(0, max_disp, wsize);
    right_matcher =  cv::ximgproc::createRightMatcher(left_matcher);

    if(filter == "wls_conf"){
        wls_filter = cv::ximgproc::createDisparityWLSFilter(left_matcher);
    }else if(filter=="fbs_conf"){
        wls_filter = cv::ximgproc::createDisparityWLSFilter(left_matcher);
    }else if(filter == "wls_no_conf"){
        wls_filter = cv::ximgproc::createDisparityWLSFilterGeneric(false);
    }else{
		std::cout<<"Unsupported filter" << std::endl;
		return -1;
	}
    return 0;
}
void StereoSGBMConcrete::match(Image &left, Image &right){
    if(!no_downscale){
        StereoSGBMConcrete::downscale(left, right);
    }
    left_matcher->setP1(24*wsize*wsize);
    left_matcher->setP2(96*wsize*wsize);
    left_matcher->setPreFilterCap(63);
    left_matcher->setMode(cv::StereoSGBM::MODE_SGBM_3WAY);
    matching_time = (double)cv::getTickCount();
    left_matcher-> compute(left, right,left_disp);
    right_matcher->compute(right, left, right_disp);
    matching_time = ((double)cv::getTickCount() - matching_time)/cv::getTickFrequency();

}
void StereoSGBMConcrete::wls_confidence_filter(Image &left, Image &filtered_disp){
    //! [filtering]
    wls_filter->setLambda(lambda);
    wls_filter->setSigmaColor(sigma);
    filtering_time = (double)cv::getTickCount();
    wls_filter->filter(left_disp,left,filtered_disp,right_disp);
    filtering_time = ((double)cv::getTickCount() - filtering_time)/cv::getTickFrequency();
    //! [filtering]
    conf_map = wls_filter->getConfidenceMap();
    // Get the ROI that was used in the last filter call:
    ROI = wls_filter->getROI();
    if(!no_downscale)
    {
        // upscale raw disparity and ROI back for a proper comparison:
        cv::resize(left_disp,left_disp,cv::Size(),2.0,2.0,cv::INTER_LINEAR);
        left_disp = left_disp*2.0;
        ROI = cv::Rect(ROI.x*2,ROI.y*2,ROI.width*2,ROI.height*2);
    }
}
void StereoSGBMConcrete::fbs_confidence_filter(Image &left, Image &filtered_disp){
        //! [filtering_wls]
    wls_filter->setLambda(lambda);
    wls_filter->setSigmaColor(sigma);
    filtering_time = (double)cv::getTickCount();
    wls_filter->filter(left_disp,left,filtered_disp,right_disp);
    filtering_time = ((double)cv::getTickCount() - filtering_time)/cv::getTickFrequency();
    //! [filtering_wls]
    conf_map = wls_filter->getConfidenceMap();
    cv::Mat left_disp_resized;
    cv::resize(left_disp,left_disp_resized,left.size());
    // Get the ROI that was used in the last filter call:
    ROI = wls_filter->getROI();
    if(!no_downscale)
    {
        // upscale raw disparity and ROI back for a proper comparison:
        cv::resize(left_disp,left_disp,cv::Size(),2.0,2.0);
        left_disp = left_disp*2.0;
        left_disp_resized = left_disp_resized*2.0;
        ROI = cv::Rect(ROI.x*2,ROI.y*2,ROI.width*2,ROI.height*2);
    }
	#ifdef HAVE_EIGEN
			//! [filtering_fbs]
			solving_time = (double)cv::getTickCount();
			cv::ximgproc::fastBilateralSolverFilter(left, left_disp_resized, conf_map/255.0f, solved_disp, fbs_spatial, fbs_luma, fbs_chroma, fbs_lambda);
			solving_time = ((double)cv::getTickCount() - solving_time)/cv::getTickFrequency();
			//! [filtering_fbs]

			//! [filtering_wls2fbs]
			cv::ximgproc::fastBilateralSolverFilter(left, filtered_disp, conf_map/255.0f, solved_filtered_disp, fbs_spatial, fbs_luma, fbs_chroma, fbs_lambda);
			//! [filtering_wls2fbs]
	#else
			(void)fbs_spatial;
			(void)fbs_luma;
			(void)fbs_chroma;
			(void)fbs_lambda;
	#endif
}
void StereoSGBMConcrete::wls_no_confidence_filter(Image &left, Image &filtered_disp){
    wls_filter->setLambda(lambda);
    wls_filter->setSigmaColor(sigma);
    filtering_time = (double)cv::getTickCount();
    wls_filter->filter(left_disp,left,filtered_disp,cv::Mat(),ROI);
    filtering_time = ((double)cv::getTickCount() - filtering_time)/cv::getTickFrequency();

}
void StereoSGBMConcrete::downscale(Image &left, Image &right){
    // downscale the views to speed-up the matching stage, as we will need to compute both left
    // and right disparity maps for confidence map computation
    //! [downscale]
    max_disp/=2;
    if(max_disp%16!=0){
        max_disp += 16-(max_disp%16);
    }
    cv::resize(left ,left , cv::Size(), 0.5, 0.5, cv::INTER_LINEAR_EXACT);
    cv::resize(right ,right, cv::Size(), 0.5, 0.5, cv::INTER_LINEAR_EXACT);
    //! [downscale]
}

void StereoSGBMConcrete::get_disparity(Image &left, Image &right, DisparityMap &dst){
    StereoSGBMConcrete::match(left, right);
    if(filter == "wls_conf"){
        StereoSGBMConcrete::wls_confidence_filter(left, dst);
    }else if(filter=="fbs_conf"){
        StereoSGBMConcrete::fbs_confidence_filter(left, dst);
    }else if(filter == "wls_no_conf"){
        StereoSGBMConcrete::wls_no_confidence_filter(left, dst);
    }else{
		std::cout<<"Unsupported filter" << std::endl;
	}
}
void StereoSGBMConcrete::get_depth(DisparityMap &src, DepthMap &dst){
}
void StereoSGBMConcrete::get_disparity_viz(DisparityMap &filtered_disp, Image &filtered_disp_vis){
    cv::ximgproc::getDisparityVis(filtered_disp,filtered_disp_vis,vis_mult);
    cv::applyColorMap(filtered_disp_vis*5, filtered_disp_vis, cv::COLORMAP_JET);
}