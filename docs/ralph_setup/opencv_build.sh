#!/bin/bash
# Author: Ao Yu
# Description: This script is intended to install and clone software needed for initial setup.
#	       Run this script from a clean Ubuntu 16.04 install.

cd 

gitdirectroy="$HOME/githome"
opencv_version="4.1.0"
machine_pswd="prismadynamics" #User input password of their own computer here.
install_system_dependancy=false
make_and_build=false

FGRED="\033[0;31m"
FGCYAN="\033[0;36m"
BGRED="\033[0;41m"
FGBLUE="\033[0;35m"
NORMAL="\033[0m"
# Check for nvidia driver, cuda, cuDNN, and TensorRT

#install system dependencies
if $install_system_dependancy;then
	echo $machine_pswd | sudo -S -k apt-get remove x264 libx264-dev -y
	echo $machine_pswd | sudo -S -k apt-get install build-essential checkinstall cmake pkg-config yasm -y
	echo $machine_pswd | sudo -S -k apt-get install git gfortran -y
	echo $machine_pswd | sudo -S -k apt-get install libjpeg8-dev libjasper-dev libpng12-dev -y

	echo $machine_pswd | sudo -S -k apt-get install libavcodec-dev libavformat-dev libswscale-dev libdc1394-22-dev -y
	echo $machine_pswd | sudo -S -k apt-get install libxine2-dev libv4l-dev -y
	echo $machine_pswd | sudo -S -k apt-get install libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev -y
	echo $machine_pswd | sudo -S -k apt-get install qt5-default libgtk2.0-dev libtbb-dev -y
	echo $machine_pswd | sudo -S -k apt-get install libatlas-base-dev -y
	echo $machine_pswd | sudo -S -k apt-get install libfaac-dev libmp3lame-dev libtheora-dev -y
	echo $machine_pswd | sudo -S -k apt-get install libvorbis-dev libxvidcore-dev -y
	echo $machine_pswd | sudo -S -k apt-get install libopencore-amrnb-dev libopencore-amrwb-dev -y
	echo $machine_pswd | sudo -S -k apt-get install x264 v4l-utils -y
	# Optional dependencies
	echo $machine_pswd | sudo -S -k apt-get install libprotobuf-dev protobuf-compiler -y
	echo $machine_pswd | sudo -S -k apt-get install libgoogle-glog-dev libgflags-dev -y
	echo $machine_pswd | sudo -S -k apt-get install libgphoto2-dev libeigen3-dev libhdf5-dev doxygen -y
	echo $machine_pswd | sudo -S -k apt-get install python-dev python-pip python3-dev python3-pip -y
	echo $machine_pswd | sudo -S -k -H pip2 install -U pip numpy -y
	echo $machine_pswd | sudo -S -k -H pip3 install -U pip numpy -y
fi
#create githome directory if it does not exist already. 
if [ ! -d $gitdirectroy ]; then
	mkdir gitdirectroy
else
	echo "INFO: $gitdirectroy directory already exists."
fi

#git clone opencv
cd $gitdirectroy
if [ ! -d "$gitdirectroy/opencv" ]; then
	git clone https://github.com/opencv/opencv.git
else
	echo "INFO: opencv repo detected in $gitdirectroy. Skipping clone."
fi
if [ ! -d "$gitdirectroy/opencv_contrib" ]; then
	git clone https://github.com/opencv/opencv_contrib.git
else
	echo "INFO: opencv_contrib repo detected in $gitdirectroy. Skipping clone."
fi
cd "$gitdirectroy/opencv"
if test $(pwd) = "$gitdirectroy/opencv"
then
	git fetch --all
	git checkout "$opencv_version"
else
	echo "ERROR: Could not cd into opencv folder. Check if it exists."
fi
cd "$gitdirectroy/opencv_contrib"
if test $(pwd) = "$gitdirectroy/opencv_contrib"
then
	git fetch --all
	git checkout "$opencv_version"
else
	echo "ERROR: Could not cd into opencv_contrib folder. Check if it exists."
fi
cd $gitdirectroy/opencv
if [ ! -d "$gitdirectroy/opencv/build" ]
then
	mkdir build
	if test $(pwd) != "$gitdirectroy/opencv/build"
	then
		echo "ERROR: Could not cd into ./opencv/build folder. Check if it exists."
	fi
fi
cd build
if $make_and_build;then
	cmake -DCMAKE_BUILD_TYPE=RELEASE \
	      -DCMAKE_INSTALL_PREFIX=/usr/local \
	      -DINSTALL_C_EXAMPLES=ON \
	      -DINSTALL_PYTHON_EXAMPLES=ON \
	      -DWITH_TBB=ON \
	      -DWITH_V4L=ON \
	      -DWITH_QT=ON \
	      -DWITH_OPENGL=ON \
	      -DOPENCV_EXTRA_MODULES_PATH=$gitdirectroy/opencv_contrib/modules \
	      -DBUILD_EXAMPLES=ON \
	      -DWITH_CUDA=ON \
	      -DENABLE_FAST_MATH=1 \
	      -DCUDA_FAST_MATH=1 \
	      -DWITH_CUBLAS=1 ..
	make -j4
	echo $machine_pswd | sudo -S -k make install
fi
echo "ATTENTION: CONFIRM THE CORRECT OPENCV VERSION IS LISTED BELOW!!!"
cat /usr/local/include/opencv4/opencv2/core/version.hpp | grep "#define CV_VERSION_"

