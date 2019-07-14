/** @file DataStructures.h
 *  @brief Global definition of data structures and enumerations
 *
 *  This contains definition for data structures and enumerations used globally. 
 *
 *  @author Ao Y. Yu 
 *  @bug No known bugs.
 */

#pragma once
#include <vector>
#include "opencv2/imgproc.hpp"

enum ImageDimension{
    IMG_WIDTH,
    IMG_HEIGHT,
    NUMBER_OF_DIM
};
enum Shape{
    RECTANGLE,
    TRIANGLE,
    SQUARE,
    CIRCLE,
    ELLIPSE,
    CONVEXHULL,
    NUMBER_OF_SHAPE
};
struct Pyramid{
    int levels;
    std::vector<cv::Mat> gaussian_pyramid;
    std::vector<cv::Mat> laplacian_pyramid;
};

struct Rectangle{
    int x = 0; //Centroid X
    int y = 0; // Centroid y
    int w = 1;
    int h = 1;
    cv::Scalar color = 0;
    int thickness = 1;
    int lineType = 8;
    int shift = 0;
};

struct Circle{
    int x = 0; //Centroid X
    int y = 0; // Centroid y
    int radius = 0; // Centroid y
    cv::Scalar color = 0;
    int thickness = 1;
    int lineType = 8;
    int shift = 0;
};