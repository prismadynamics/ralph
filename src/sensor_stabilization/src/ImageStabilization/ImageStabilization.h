/** @file ImageStabilization.h
 *  @brief Contrete implimentation of image stabilization methods.
 * 
 *  @author Ao Y. Yu 
 *  @bug No known bugs.
 */

#ifndef _IMAGESTABILIZATION_H
#define _IMAGESTABILIZATION_H

#include <assert.h>     /* assert */
#include <vector>

#include "opencv2/core.hpp"
#include "opencv2/imagproc.hpp"

typedef cv::Mat Image;
typedef std::vector<Image> ImageVector;
typedef std::vector<Trajectory> TrajectoryVector;
typedef std::vector<Transform> TransformVector;
class ImageStabilization{
    public:
        ImageStabilization();
        ~ImageStabilization();
        void run();
        void motion_estimate(ImageVector& src);
        void smooth();
        void fix_border(cv::Mat &frame_stabilized);
    protected:

    private:
        int image_width = 0;
        int image_height = 0;
        struct TransformParam{
            TransformParam() {}
            TransformParam(double _dx, double _dy, double _da){
                dx = _dx;
                dy = _dy;
                da = _da;
            }
            double dx;
            double dy;
            double da; // angle
            
            void getTransform(Mat &T){
                // Reconstruct transformation matrix accordingly to new values
                T.at<double>(0,0) = cos(da);
                T.at<double>(0,1) = -sin(da);
                T.at<double>(1,0) = sin(da);
                T.at<double>(1,1) = cos(da);
            
                T.at<double>(0,2) = dx;
                T.at<double>(1,2) = dy;
            }
        };
        struct Trajectory{
            Trajectory() {}
            Trajectory(double _x, double _y, double _a) {
                x = _x;
                y = _y;
                a = _a;
            }
            double x;
            double y;
            double a; // angle
        };



        TrajectoryVector smooth(std::vector<Trajectory> trajectory, int radius);
        
        
        inline bool is_grayscale(cv::Mat& src){
            if(src.channels() == 3){
                return false;
            }else{
                return true;
            }
        }
};

#endif
