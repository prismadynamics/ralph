#!/bin/bash
# Author: Ao Yu
# Description: This script is intended to install and clone software needed for initial setup.
#	       Run this script from a clean Ubuntu 16.04 install.

cd 

gitdirectroy="$HOME/githome"
machine_pswd="prismadynamics" #User input password of their own computer here.
install_system_dependancy=true
# Check for nvidia driver, cuda, cuDNN, and TensorRT

cvVersion="4.1.0"
# Save current working directory
mkdir installation
mkdir installation/OpenCV-"$cvVersion"

cwd=$(pwd)


if $install_system_dependancy;then
	echo $machine_pswd | sudo -S -k apt-get -y update
	echo $machine_pswd | sudo -S -k apt-get -y upgrade

#install system dependencies
if $install_system_dependancy;then

sudo apt -y remove x264 libx264-dev

## Install dependencies
	echo $machine_pswd | sudo -S -k apt -y install build-essential checkinstall cmake pkg-config yasm
	echo $machine_pswd | sudo -S -k apt -y install git gfortran
	echo $machine_pswd | sudo -S -k apt -y install libjpeg8-dev libpng-dev

	echo $machine_pswd | sudo -S -k apt -y install software-properties-common
	echo $machine_pswd | sudo -S -k add-apt-repository "deb http://security.ubuntu.com/ubuntu xenial-security main"
	echo $machine_pswd | sudo -S -k apt -y update

	echo $machine_pswd | sudo -S -k apt -y install libjasper1
	echo $machine_pswd | sudo -S -k apt -y install libtiff-dev

	echo $machine_pswd | sudo -S -k apt -y install libavcodec-dev libavformat-dev libswscale-dev libdc1394-22-dev
	echo $machine_pswd | sudo -S -k apt -y install libxine2-dev libv4l-dev
	cd /usr/include/linux
	echo $machine_pswd | sudo -S -k ln -s -f ../libv4l1-videodev.h videodev.h
	cd "$cwd"

	echo $machine_pswd | sudo -S -k apt -y install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
	echo $machine_pswd | sudo -S -k apt -y install libgtk2.0-dev libtbb-dev qt5-default
	echo $machine_pswd | sudo -S -k apt -y install libatlas-base-dev
	echo $machine_pswd | sudo -S -k apt -y install libfaac-dev libmp3lame-dev libtheora-dev
	echo $machine_pswd | sudo -S -k apt -y install libvorbis-dev libxvidcore-dev
	echo $machine_pswd | sudo -S -k apt -y install libopencore-amrnb-dev libopencore-amrwb-dev
	echo $machine_pswd | sudo -S -k apt -y install libavresample-dev
	echo $machine_pswd | sudo -S -k apt -y install x264 v4l-utils

	# Optional dependencies
	echo $machine_pswd | sudo -S -k apt -y install libprotobuf-dev protobuf-compiler
	echo $machine_pswd | sudo -S -k apt -y install libgoogle-glog-dev libgflags-dev
	echo $machine_pswd | sudo -S -k apt -y install libgphoto2-dev libeigen3-dev libhdf5-dev doxygen

	echo $machine_pswd | sudo -S -k apt -y install python3-dev python3-pip python3-venv
	echo $machine_pswd | sudo -S -k -H pip3 install -U pip numpy
	echo $machine_pswd | sudo -S -k apt -y install python3-testresources

cvVersion="4.1.0"
# create virtual environment
python3 -m venv OpenCV-"$cvVersion"-py3
echo "# Virtual Environment Wrapper" >> ~/.bashrc
echo "alias workoncv-$cvVersion=\"source $cwd/OpenCV-$cvVersion-py3/bin/activate\"" >> ~/.bashrc
source "$cwd"/OpenCV-"$cvVersion"-py3/bin/activate

# now install python libraries within this virtual environment
pip install wheel numpy scipy matplotlib scikit-image scikit-learn ipython

# quit virtual environment
deactivate
######################################

cd $gitdirectroy

git clone https://github.com/opencv/opencv.git
cd opencv
git checkout tags/"$cvVersion"
cd ..
git clone https://github.com/opencv/opencv_contrib.git
cd opencv_contrib
git checkout tags/"$cvVersion"
cd ..

cd opencv
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE \
      -D CMAKE_INSTALL_PREFIX=$cwd/installation/OpenCV-"$cvVersion" \
      -D INSTALL_C_EXAMPLES=ON \
      -D INSTALL_PYTHON_EXAMPLES=ON \
      -D WITH_TBB=ON \
      -D WITH_V4L=ON \
      -D WITH_QT=ON \
      -D WITH_OPENGL=ON \
      -D OPENCV_PYTHON3_INSTALL_PATH=$cwd/OpenCV-$cvVersion-py3/lib/python3.6/site-packages \
      -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules \
      -D BUILD_EXAMPLES=ON \
      -D WITH_CUDA=ON \
      -D ENABLE_FAST_MATH=1 \
      -D CUDA_FAST_MATH=1 \
      -D WITH_CUBLAS=1 ..

make -j4
make install
