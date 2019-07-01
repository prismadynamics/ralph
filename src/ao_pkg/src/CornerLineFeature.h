#pragma once

#include "opencv2/imgproc.hpp"
#include <iostream>

#include "FeatureExtractor.h"

namespace SLAM{
    enum CORNERTYPE{
        HARRIS,
        CORNER_SUBPIX,
        HOUGH_LINES,
        HOUGH_LINES_P,
        NUM_OF_CORNERTYPE,
    };
    class CornerLineFeature : FeatureExtractor{
        public:
            CornerLineFeature();
            ~CornerLineFeature();

            void ExtractCornerLine(cv::Mat&, cv::Mat&);

        private:

    };
}