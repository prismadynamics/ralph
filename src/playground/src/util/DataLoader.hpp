/** @file DataLoader.hpp
 *  @brief This is a header impliemention of a structure used to load data into ram during runtime. It currently works with KITTI dataset. 
 * 
 *  @author Ao Y. Yu 
 *  @bug No known bugs.
 */
#pragma once

#include <string>
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <sstream>

#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"

struct DataLoader{
	std::string dir;

	std::vector<std::string> split(std::string strToSplit, char delimeter){
		std::stringstream ss(strToSplit);
	    std::string item;
	    std::vector<std::string> splittedStrings;
	    while (std::getline(ss, item, delimeter))
	    {
	       splittedStrings.push_back(item);
	    }
	    return splittedStrings;
	}

	int getdir(std::string dir, std::vector<std::string> &files){
	    DIR *dp;
	    struct dirent *dirp;
	    if((dp  = opendir(dir.c_str())) == NULL) {
	        std::cout << "Error(" << errno << ") opening " << dir << std::endl;
	        return errno;
	    }
	    while ((dirp = readdir(dp)) != NULL) {
	        std::string temp_file = dirp->d_name;
	        files.push_back(temp_file);
	    }
	    closedir(dp);
	    return 0;
	}

	int getkittidir(std::string dir, std::vector<std::string> &files_out){
		std::vector<std::string> files;
		std::vector<int> file_number;
		getdir(dir, files);
		for(int i = 0; i < files.size(); i++){
			std::string current_file = files[i];
	        	std::vector<std::string> current_file_split = split(current_file,'.');
			file_number.push_back(std::atoi(current_file_split[0].c_str()));	
		}
		int max = (int)(*max_element(file_number.begin(), file_number.end()));
		int min = (int)(*min_element(file_number.begin(), file_number.end()));
		for(int i = min; i <= max; i++){
			std::string temp_sorted_filename = std::to_string(i);
			std::string sorted_filename = std::to_string(i);
			for(int j = 0; j < (10 - temp_sorted_filename.length()); j++){
				sorted_filename = "0" + sorted_filename;
			}
			files_out.push_back(sorted_filename + ".png");	
		}
		return 0;
	}

	void print_loadingbar(float in_pct){
		// This function prints a simple status bar with 30 devisions.
		std::string status_bar = "                                                ";
		int indx = status_bar.length()*(in_pct/100);
		for(int i = 0; i <= indx; i++){
			status_bar.replace(i,1,"=");
		}
		std::cout << "PERCENT LOADED|" << status_bar <<  "|" << in_pct << "%" << std::endl;
	}

	std::vector<cv::Mat> loadimages(std::string dir, std::vector<std::string> &files){
		std::vector<cv::Mat> vector_src;
		std::cout << "Loading images to memory. Verbose at every 50 images." << std::endl;
		for(int i = 0; i < files.size(); i++){
			std::string file_name = files[i];
			cv::Mat temp_Mat = cv::imread(dir + file_name);
			if(temp_Mat.empty()){
		        std::cout << "Could not open or find the image!" << std::endl;
		        break;
	    	}else{
	    		vector_src.push_back(temp_Mat);
	    	}
			if(i%100 == 0){
				print_loadingbar((float)i/files.size()*100);
			}
		}
		return vector_src;
	}

	void set_dir(std::string in_dir){
		dir = in_dir;
	}

	std::vector<cv::Mat> getkittiimages(){
		std::vector<std::string> files = std::vector<std::string>();
		getkittidir(dir, files);
		return loadimages(dir, files);
	}

	std::vector<cv::Mat> getkittiimages(std::string in_dir){
		dir = in_dir;
		std::vector<std::string> files = std::vector<std::string>();
		getkittidir(dir, files);
		return loadimages(dir, files);
	}


};
