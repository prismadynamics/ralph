/** @file ImageUtility.cpp
 *  @brief Concrete class definition to perform various image manipulations.
 *
 *  @author Ao Y. Yu 
 *  @bug No known bugs.
 */

#include "ImageUtility.h"

ImageUtility::ImageUtility(){

}

ImageUtility::~ImageUtility(){
    try{
        delete this;
    }catch(int e){
    };
}

void ImageUtility::print_img_info(cv::Mat& src){
    std::cout << "================== Start of Image Information ============" << std::endl;
    std::cout << "Address of Mat: " << &src << std::endl;
    std::cout << "Number of Channels: " << src.channels() << std::endl;
    std::cout << "Image Width: " << src.cols << std::endl;
    std::cout << "Image Height: " << src.rows << std::endl;
    std::cout << "================== End of Image Information ============" << std::endl;
}

void ImageUtility::resize(cv::Mat& src, cv::Mat& dst, int w, int h, cv::InterpolationFlags interp_flag){
    if(src.rows == h && src.cols == w){
        dst = src;
        std::cout << "================== Start of ImageUtility Error: down_sample ============" << std::endl;
        std::cout << "Image dimension already the desired dimension. No further action is taken." << std::endl;
        std::cout << "================== End of ImageUtility Error: down_sample ============" << std::endl;
    }else{
        cv::resize(src, dst, cv::Size(w, h), 0, 0, interp_flag);
    }
}

void ImageUtility::split_image(cv::Mat& src, cv::Mat& dst_1, cv::Mat& dst_2, ImageDimension imgdim){
    if(src.cols % 2 != 0 || src.rows % 2 != 0){
        std::cout << "================== Start of ImageUtility Error: split_image ============" << std::endl;
        std::cout << "Image dimension is not a multiple of 2. Unable to split the image." << std::endl;
        std::cout << "================== End of ImageUtility Error: split_image ============" << std::endl;
        assert(src.cols % 2 == 0 || src.rows % 2 == 0);
    }else{
        cv::Rect ROI_1(0, 0, src.cols/2, src.rows);
        cv::Rect ROI_2(src.cols/2, 0, src.cols/2, src.rows);
        if(imgdim == IMG_WIDTH){
            // Image ROI is initialized as a split in the width dimension. No further action is required.
        }else if(imgdim == IMG_HEIGHT){
            // Image ROI is initialized as a split in the width dimension. Setting new dimension parameters. 
            ROI_1.x = 0;
            ROI_1.y = 0;
            ROI_1.width = src.cols;
            ROI_1.height = src.rows/2;
                
            ROI_2.x = 0;
            ROI_2.y = src.rows/2;
            ROI_2.width = src.cols;
            ROI_2.height = src.rows/2;
        }else{
        }
        dst_1 = src(ROI_1);
        dst_2 = src(ROI_2);
    }
}

void ImageUtility::image_pyramid(cv::Mat& src, Pyramid& input_pyramid, int level, bool calc_laplacian){
    cv::Mat temp1, temp2, temp3;
    cv::Mat Lap;
    src.copyTo(temp1);
    input_pyramid.levels = level;
    for(int i=0; i<level; i++){
        cv::pyrDown(temp1, temp2);
        input_pyramid.gaussian_pyramid.push_back(temp2);
        if(calc_laplacian){
            cv::pyrUp(temp2, temp3, temp1.size());  
            Lap = temp1-temp3;
            input_pyramid.laplacian_pyramid.push_back(Lap);
        }
        temp1=temp2;
    }
}

Pyramid ImageUtility::image_pyramid(cv::Mat& src, int level, bool calc_laplacian){
    Pyramid input_pyramid;
    cv::Mat temp1, temp2, temp3;
    cv::Mat Lap;
    src.copyTo(temp1);
    input_pyramid.levels = level;
    for(int i=0; i<level; i++){
        cv::pyrDown(temp1, temp2);
        input_pyramid.gaussian_pyramid.push_back(temp2);
        if(calc_laplacian){
            cv::pyrUp(temp2, temp3, temp1.size());  
            Lap = temp1-temp3;
            input_pyramid.laplacian_pyramid.push_back(Lap);
        }
        temp1=temp2;
    }
    return input_pyramid;
}

void ImageUtility::draw_pyramid(cv::Mat& dst, Pyramid& input_pyramid){
    int num_of_levels = input_pyramid.levels;

}

void ImageUtility::draw_rectangle(cv::Mat& src, cv::Mat& dst, std::vector<Rectangle> rectangles, double alpha, bool with_dot){
    int num_of_shape = rectangles.size();
    dst = src;
    if(num_of_shape == 0){
    }else{
        if(alpha <= 0.0001){
            for(int i = 0; i < num_of_shape; i++){
                if(with_dot){
                    circle(dst, cv::Point(rectangles[i].x, rectangles[i].y), 3, cv::Scalar(0,255,0), -1, 8, 0 );
                }
                cv::rectangle(dst, cv::Rect(rectangles[i].x, rectangles[i].y, rectangles[i].w, rectangles[i].h), 
                        rectangles[i].color, rectangles[i].thickness, rectangles[i].lineType, rectangles[i].shift);
            }
        }else{
            cv::Mat overlay;
            dst.copyTo(overlay);
            for(int i = 0; i < num_of_shape; i++){
                if(with_dot){
                    circle(overlay, cv::Point(rectangles[i].x, rectangles[i].y), 3, cv::Scalar(0,255,0), -1, 8, 0 );
                }
                cv::rectangle(overlay, cv::Rect(rectangles[i].x, rectangles[i].y, rectangles[i].w, rectangles[i].h), 
                        rectangles[i].color, -1, rectangles[i].lineType, rectangles[i].shift);
            }
            cv::addWeighted(overlay, alpha, dst, 1 - alpha, 0, dst);            
        }
    }
}

void ImageUtility::pad_image(cv::Mat& src, cv::Mat& dst, int number_of_pixel[4], int borderType, int color){
    cv::copyMakeBorder(src, dst, number_of_pixel[0], number_of_pixel[1], number_of_pixel[2], number_of_pixel[3], borderType, color);
}

void draw_circle(cv::Mat& src, cv::Mat& dst, std::vector<Circle> circles, double alpha, bool with_dot){
    /// Draw the circles detected
    int num_of_shape = circles.size();
    dst = src;
    if(num_of_shape == 0){
    }else{
        if(alpha <= 0.0001){
            for( size_t i = 0; i < circles.size(); i++ ){
                cv::Point center(circles[i].x, circles[i].y);
                int radius = circles[i].radius;
                if(with_dot){
                    cv::circle( dst, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
                }
                cv::circle(dst, center, radius, circles[i].color, circles[i].thickness, circles[i].lineType, circles[i].shift);
            }
        }else{
            cv::Mat overlay;
            dst.copyTo(overlay);
            for( size_t i = 0; i < circles.size(); i++ ){
                cv::Point center(circles[i].x, circles[i].y);
                int radius = circles[i].radius;
                if(with_dot){
                    cv::circle(overlay, center, 3, cv::Scalar(0,255,0), -1, 8, 0);
                }
                cv::circle(overlay, center, radius, circles[i].color, circles[i].thickness, circles[i].lineType, circles[i].shift);
            }
            cv::addWeighted(overlay, alpha, dst, 1 - alpha, 0, dst);            
        }
    }
}

void affine_transform_image(cv::Mat& src, cv::Mat& dst, cv::Point2f srcTri[3], cv::Point2f dstTri[3]){
    cv::Mat warp_mat(2, 3, CV_32FC1);
    warp_mat = cv::getAffineTransform(srcTri, dstTri);
    cv::warpAffine(src, dst, warp_mat, dst.size());
}

void affine_transform_image(cv::Mat& src, cv::Mat& dst, cv::Mat warp_mat){
    cv::warpAffine(src, dst, warp_mat, dst.size());
}