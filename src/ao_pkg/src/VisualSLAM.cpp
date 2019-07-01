#include "VisualSLAM.h"

using namespace SLAM;

VisualSLAM::VisualSLAM(): EnableVisualSLAM(false){

}

VisualSLAM::VisualSLAM(int w, int h, bool enableSLAM): EnableVisualSLAM(enableSLAM){
    if(w%16 != 0 || h%16 != 0){
		printf("image dimensions must be multiples of 16! Please crop your images / video accordingly.\n");
		assert(false);
	}
	this->width = w;
	this->height = h;
	trackingIsGood = true;

}
