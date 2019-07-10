/** @file UndistortStrategy.cpp
 *  @brief Virtual prototypes of for undistorting an image.
 * 
 *  @author Ao Y. Yu 
 *  @bug No known bugs.
 */

#include "UndistortStrategy.h"

UndistortStrategy::~UndistortStrategy(){
}

UndistortStrategy* UndistortStrategy::getUndistorterForFile(const char* configFilename){
	std::string completeFileName = configFilename;
	printf("Reading Calibration from file %s",completeFileName.c_str());
	std::ifstream f(completeFileName.c_str());
	if (!f.good())
	{
		f.close();

		completeFileName = packagePath+"calib/"+configFilename;
		printf(" ... not found!\n Trying %s", completeFileName.c_str());

		f.open(completeFileName.c_str());

		if (!f.good())
		{
			printf(" ... not found. Cannot operate without calibration, shutting down.\n");
			f.close();
			return 0;
		}
	}
	printf(" ... found!\n");
	std::string l1;
	std::getline(f,l1);
	f.close();
	float ic[10];
	if(std::sscanf(l1.c_str(), "%f %f %f %f %f %f %f %f",
			&ic[0], &ic[1], &ic[2], &ic[3], &ic[4],
			&ic[5], &ic[6], &ic[7]) == 8)
	{
		printf("found OpenCV camera model, building rectifier.\n");
		Undistorter* u = new UndistorterOpenCV(completeFileName.c_str());
		if(!u->isValid()) return 0;
		return u;
	}
	else
	{
		printf("found ATAN camera model, building rectifier.\n");
		Undistorter* u = new UndistorterPTAM(completeFileName.c_str());
		if(!u->isValid()) return 0;
		return u;
	}
}
