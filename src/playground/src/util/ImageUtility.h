/** @file ImageUtility.h
 *  @brief Prototype of concrete class definitions to perform various image manipulations.
 * 
 *  @author Ao Y. Yu 
 *  @bug No known bugs.
 */

#pragma once

#include "opencv2/imgproc.hpp"
#include "DataStructures.hpp"

#include <iostream>
#include <assert.h>     /* assert */

class ImageUtility{
    public:
        ImageUtility();
        ~ImageUtility();
        /** @brief split_image Splits an image in half along either x or y dimensions. Input image must have
            *                     dimension that is multiple of 2.
            * 
            *  @param src A reference of the source image to split.
            *  @param dest_1 A reference of the destination image. Top image if split along horizontal
            *                       and Left if split along vertical. 
            *  @param dest_2 A reference of the destination image. Bottom image if split along horizontal
            *                       and Right if split along vertical. 
            *  @param imdgim A reference for dimension to split. Enum of type ImageDimension. 
            *  @return void
            */
        void split_image(cv::Mat& src, cv::Mat& dest_1, cv::Mat& dest_2, int imdgim = 0);

        /** @brief image_pyramid Calculate Gaussian and Laplacian (option) pyramids
            * 
            *  @param src A reference of the source image.
            *  @param input_pyramid A reference of the pyramid data structure. Structure contain vector of images.
            *  @param level Number of levels to calculate pyramid for.  
            *  @param calc_laplacian Boolean to specify if Laplacian pyramid is also needed.  
            *  @return void
            */
        void image_pyramid(cv::Mat& src, Pyramid& input_pyramid, int level, bool calc_laplacian);
        /** @brief image_pyramid Calculate Gaussian and Laplacian (option) pyramids
            * 
            *  @param src A reference of the source image.
            *  @param level Number of levels to calculate pyramid for.  
            *  @param calc_laplacian Boolean to specify if Laplacian pyramid is also needed.  
            *  @return input_pyramid A Pyramid data structure defined in DatStructure.hpp. 
            *                        Structure contain vector of images.
            */
        Pyramid image_pyramid(cv::Mat& src, int level, bool calc_laplacian);

        void draw_pyramid(cv::Mat&, Pyramid&);
        /** @brief resize Resize an image by up or down sampling the srouce image using different interpolation methods.
            *                To shrink an image, it will generally look best with INTER_AREA interpolation, whereas to 
            *                enlarge an image, it will generally look best with c::INTER_CUBIC (slow) or INTER_LINEAR 
            *                (faster but still looks OK).
            * 
            *  @param src A reference of the source image.
            *  @param dst A reference to store the resulting resized image.
            *  @param w   Desired new width of the image. 
            *  @param h   Desired new height of the image.
            *  @param interp_flag   Method of interpolating near-by pixels in resizing algorithm.
            *  @return void
            */
        void resize(cv::Mat& src, cv::Mat& dst, int w, int h, cv::InterpolationFlags interp_flag = cv::INTER_AREA);

        void affine_transform_image(cv::Mat& src, cv::Mat& dst, cv::Point2f srcTri[3], cv::Point2f dstTri[3]);
        void affine_transform_image(cv::Mat& src, cv::Mat& dst, cv::Mat warp_mat);
        /** @brief draw_rectangle Draws a simple rectangle in a specified location. 
            * 
            *  @param src A reference of the source image.
            *  @param dst A reference of the destination image. Result will be saved in this reference.
            *  @param rectangles   A vector of Rectangle defined in DataStructures.hpp
            *  @param alpha   A value between 0 and 1 to determine level of transparency of a filled rectangle. 
            *  @param with_dot   Boolean to indicate if a point in every rectangle's center should also be drawn. 
            *  @return void
            */
        void draw_rectangle(cv::Mat& src, cv::Mat& dst, std::vector<Rectangle> rectangles, double alpha = 0.0, bool with_dot = false);
        /** @brief draw_circle Draws a simple circle in a specified location. 
            * 
            *  @param src A reference of the source image.
            *  @param dst A reference of the destination image. Result will be saved in this reference.

            *  @param circles   A vector of cv::Vec3f where elements are x, y, and radius.
            *  @param alpha   A value between 0 and 1 to determine level of transparency of a filled circle. 
            *  @param with_dot   Boolean to indicate if a point in every circle's center should also be drawn. 
            *  @return void
            */
        void draw_circle(cv::Mat& src, cv::Mat& dst, std::vector<Circle> circles, double alpha = 0.0, bool with_dot = false);
        /** @brief pad_image Pads the border of an image by a specified amount of additional pixels. Also the padding
            *                   can be either constant color or replicating border pixels.
            * 
            *  @param src A reference of the source image.
            *  @param dst A reference of the destination image. Result will be saved in this reference.
            *  @param number_of_pixel   Number of pixels to add to each border side. Element order is top, bottom, 
            *                              left, and right. 
            *  @param borderType   Padding method of either constant color or replicating border pixels.
            *  @param color   Color of the border padding if constant color type is selected. Defaulted black.
            *  @return void
            */
        void pad_image(cv::Mat& src, cv::Mat& dst, int number_of_pixel[4], int borderType = cv::BORDER_CONSTANT, int color = 0);
        /** @brief print_img_info Prints various information regarding the image in form of cv::Mat data type.
            * 
            *  @param src The source image to print information of.
            *  @return void
            */
        void print_img_info(cv::Mat& src);
        private:
            float fx, fy;
};