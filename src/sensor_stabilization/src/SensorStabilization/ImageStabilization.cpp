/** @file ImageStabilization.h
 *  @brief Contrete implimentation of image stabilization methods.
 * 
 *  @author Ao Y. Yu 
 *  @bug No known bugs.
 */

#include "ImageStabilization.h"

ImageStabilization::ImageStabilization(){

}

ImageStabilization::~ImageStabilization(){
}

void ImageStabilization::run(){

}

void ImageStabilization::motion_estimate(ImageVector& src){
    assert(image_width > 0 && image_height > 0);
    if(image_width == 0 || image_height == 0){
        return;
    }
    for(std::size_t i = 0; i <src.size(); i++){
        if(ImageStabilization::is_grayscale(src[i])){
            cv::cvtColor(src[i], src[i], cv::COLOR_BGR2GRAY);
            std::cout << "ATTENTION: Input image is not grayscale. Check Set-up." << std::endl;
        }
    }

    TransformVector transforms; 
    cv::Mat last_T;
    for(std::size_t i = 0; i < src.size(); i++){
        PointsVector prev_pts, curr_pts;
        cv::goodFeaturesToTrack(src[i], prev_pts, 200, 0.01, 30);
        cv::cvtColor(curr, curr_gray, cv::COLOR_BGR2GRAY);
        std::vector<uchar> status;
        std::vector<float> err;
        cv::calcOpticalFlowPyrLK(src[i], curr_gray, prev_pts, curr_pts, status, err);
        auto prev_it = prev_pts.begin(); 
        auto curr_it = curr_pts.begin(); 
        for(std::size_t k = 0; k < status.size(); k++){
            if(status[k]){
                prev_it++; 
                curr_it++; 
            }
            else{
                prev_it = prev_pts.erase(prev_it);
                curr_it = curr_pts.erase(curr_it);
            }
        }
        cv::Mat T = cv::estimateRigidTransform(prev_pts, curr_pts, false);
        if(display_image){
            ImageStabilization::display_img_with_points(curr_gray, curr_pts)
        } 
        if(T.data == NULL) last_T.copyTo(T);
        T.copyTo(last_T);
        double dx = T.at<double>(0,2);
        double dy = T.at<double>(1,2);
        double da = atan2(T.at<double>(1,0), T.at<double>(0,0));
        transforms.push_back(TransformParam(dx, dy, da));
        curr_gray.copyTo(src[i]);
    }
}

void ImageStabilization::fix_border(cv::Mat &frame_stabilized){
    cv::Mat T = cv::getRotationMatrix2D(cv::Point2f(frame_stabilized.cols/2, frame_stabilized.rows/2), 0, 1.04); 
    cv::warpAffine(frame_stabilized, frame_stabilized, T, frame_stabilized.size()); 
}

TrajectoryVector ImageStabilization::smooth(TrajectoryVector trajectory, int radius){
    TrajectoryVector smoothed_trajectory; 
    for(std::size_t i=0; i < trajectory.size(); i++) {
        double sum_x = 0;
        double sum_y = 0;
        double sum_a = 0;
        int count = 0;
        for(int j = -radius; j <= radius; j++) {
            if(i+j >= 0 && i+j < trajectory.size()){
                sum_x += trajectory[i+j].x;
                sum_y += trajectory[i+j].y;
                sum_a += trajectory[i+j].a;
                count++;
            }
        }
        double avg_a = sum_a / count;
        double avg_x = sum_x / count;
        double avg_y = sum_y / count;

        smoothed_trajectory.push_back(Trajectory(avg_x, avg_y, avg_a));
    }
    return smoothed_trajectory; 
}

TrajectoryVector ImageStabilization::cumsum(TransformVector transforms){
    TrajectoryVector  trajectory;
    double a = 0;
    double x = 0;
    double y = 0;
    for(std::size_t i=0; i <transforms.size(); i++){
        x += transforms[i].dx;
        y += transforms[i].dy;
        a += transforms[i].da;
        trajectory.push_back(Trajectory(x,y,a));
    }
    return trajectory; 
}

TransformVector ImageStabilization::transform_smoother(TransformVector transforms, TrajectoryVector smoothed_trajectory){
    TransformVector transforms_smooth;
    for(std::size_t i = 0; i <transforms.size(); i++){
        // Calculate difference in smoothed_trajectory and trajectory
        double diff_x = smoothed_trajectory[i].x - trajectory[i].x;
        double diff_y = smoothed_trajectory[i].y - trajectory[i].y;
        double diff_a = smoothed_trajectory[i].a - trajectory[i].a;

        // Calculate newer transformation array
        double dx = transforms[i].dx + diff_x;
        double dy = transforms[i].dy + diff_y;
        double da = transforms[i].da + diff_a;

        transforms_smooth.push_back(TransformParam(dx, dy, da));
    }
    return transforms_smooth;
}

void ImageStabilization::apply_smooth(){
    cv::Mat T(2,3,CV_64F);
    cv::Mat frame, frame_stabilized, frame_out; 
    for( int i = 0; i < n_frames-1; i++){
        bool success = cap.read(frame); 
        if(!success){
            break;
        }
        {
            cv::resize(frame_out, frame_out, Size(frame_out.cols/2, frame_out.rows/2));
        }
    }
}

void ImageStabilization::display_img_with_points(Image& dst, PointsVector& points, int color, std::string windowname){
    cv::Mat display_img;
    cv::drawKeypoints(dst, points, display_img, cv::Scalar(color));
    cv::imshow(windowname,display_img);
    cv::waitKey(50);     
}