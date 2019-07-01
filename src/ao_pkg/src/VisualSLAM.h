#pragma once
#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>
#include "IOWrapper/Timestamp.h"

namespace SLAM{

class VisualSLAM{
    public:
        int width;
        int height;
        const bool EnableVisualSLAM;
        VisualSLAM();
        VisualSLAM(int w, int h, bool enableSLAM = true);
        VisualSLAM(const VisualSLAM&) = delete;
        VisualSLAM& operator=(const VisualSLAM&) = delete;
        ~VisualSLAM();
        bool trackingIsGood;

};

}