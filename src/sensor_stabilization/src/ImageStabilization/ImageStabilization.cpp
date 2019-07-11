/** @file ImageStabilization.h
 *  @brief Contrete implimentation of image stabilization methods.
 * 
 *  @author Ao Y. Yu 
 *  @bug No known bugs.
 */

#include "ImageStabilization/ImageStabilization.h"

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
    for(size_t i = 0; i <src.size(); i++){
        if(ImageStabilization::is_grayscale(src[i])){
            cv::cvtColor(src[i], src[i], cv::COLOR_BGR2GRAY);
            std::cout <<"ATTENTION: Input image is was not grayscale for image stablization. Check Set-up." <<std::endl;
        }
    }

    std::vector<TransformParam> transforms; 
    cv::Mat last_T;
    for(size_t i = 0; i < src.size(); i++){
        std::vector<cv::Point2f> prev_pts, curr_pts;
        cv::goodFeaturesToTrack(src[i], prev_pts, 200, 0.01, 30);
        cv::cvtColor(curr, curr_gray, cv::COLOR_BGR2GRAY);
        std::vector<uchar> status;
        std::vector<float> err;
        cv::calcOpticalFlowPyrLK(src[i], curr_gray, prev_pts, curr_pts, status, err);
        auto prev_it = prev_pts.begin(); 
        auto curr_it = curr_pts.begin(); 
        for(size_t k = 0; k < status.size(); k++){
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
    Mat T = cv::getRotationMatrix2D(cv::Point2f(frame_stabilized.cols/2, frame_stabilized.rows/2), 0, 1.04); 
    cv::warpAffine(frame_stabilized, frame_stabilized, T, frame_stabilized.size()); 
}

TrajectoryVector smooth(TrajectoryVector trajectory, int radius){
    TrajectoryVector smoothed_trajectory; 
    for(size_t i=0; i <trajectory.size(); i++) {
        double sum_x = 0;
        double sum_y = 0;
        double sum_a = 0;
        int count = 0;
        for(int j = -radius; j <= radius; j++) {
            if(i+j >= 0 && i+j <trajectory.size()){
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

TrajectoryVector cumsum(TransformVector transforms){
    TrajectoryVector  trajectory; // trajectory at all frames
    // Accumulated frame to frame transform
    double a = 0;
    double x = 0;
    double y = 0;
    for(size_t i=0; i <transforms.size(); i++){
        x += transforms[i].dx;
        y += transforms[i].dy;
        a += transforms[i].da;
        trajectory.push_back(Trajectory(x,y,a));
    }
    return trajectory; 
}

TransformVector transform_smoother(TransformVector transforms, TrajectoryVector smoothed_trajectory){
    TransformVector transforms_smooth;

    for(size_t i = 0; i <transforms.size(); i++){
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

void apply_smooth(){
    cap.set(CV_CAP_PROP_POS_FRAMES, 1);
    Mat T(2,3,CV_64F);
    Mat frame, frame_stabilized, frame_out; 
    for( int i = 0; i <n_frames-1; i++) { bool success = cap.read(frame); if(!success) break; // Extract transform from translation and rotation angle. transforms_smooth[i].getTransform(T); // Apply affine wrapping to the given frame warpAffine(frame, frame_stabilized, T, frame.size()); // Scale image to remove black border artifact fixBorder(frame_stabilized); // Now draw the original and stabilised side by side for coolness hconcat(frame, frame_stabilized, frame_out); // If the image is too big, resize it. if(frame_out.cols > 1920) 
    {
        cv::resize(frame_out, frame_out, Size(frame_out.cols/2, frame_out.rows/2));
    }
    
    imshow("Before and After", frame_out);
    out.write(frame_out);
    waitKey(10);
    }
}