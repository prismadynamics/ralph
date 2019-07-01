#!/bin/bash
# Author: Ao Yu
# Description: This script is intended to install and clone software needed for initial setup.
#	       Run this script from a clean Ubuntu 16.04 install.

cd
gitdirectroy="$HOME/githome"
github_url="https://github.com/prismadynamics/ralph.git"

github_username="prismadynamics@gmail.com"
github_pswd="prismagithub123"

machine_pswd="prismadynamics"

#git repo checkout versions
ralph_version="master"
opencv_version="4.1.0"
pcl_version="pcl-1.9.1"
g2o_version="20170730_git"

#install system dependencies
echo $machine_pswd | sudo -S -k apt-get update
echo $machine_pswd | sudo -S -k apt-get upgrade -y
echo $machine_pswd | sudo -S -k apt-get install git -y
echo $machine_pswd | sudo -S -k apt-get install build_essential -y
echo $machine_pswd | sudo -S -k apt-get install openssh-server -y
echo $machine_pswd | sudo -S -k cp /etc/ssh/sshd_config /etc/ssh/sshd_config.factory-defaults
echo $machine_pswd | sudo -S -k chmod a-w /etc/ssh/sshd_config.factory-defaults
echo $machine_pswd | sudo -S -k systemctl restart ssh
echo $machine_pswd | sudo -S -k sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
echo $machine_pswd | sudo -S -k apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654
echo $machine_pswd | sudo -S -k apt-get update
echo $machine_pswd | sudo -S -k apt-get install ros-kinetic-desktop-full
echo $machine_pswd | sudo -S -k rosdep init
rosdep update
echo "source /opt/ros/kinetic/setup.bash" >> ~/.bashrc
source ~/.bashrc
echo $machine_pswd | sudo -S -k apt install python-rosinstall python-rosinstall-generator python-wstool build-essential -y



#create githome directory if it does not exist already. 
if [ ! -d $gitdirectroy ]; then
	mkdir gitdirectroy
else
	echo "INFO: $gitdirectroy directory already exists."
fi
#git clone ralph
cd $gitdirectroy
if [ ! -d "ralph" ]; then
	git clone $github_url
else
	echo "INFO: ralph repo detected in $gitdirectroy. Skipping clone."
	echo "INFO: This script is intended for first time setup."
fi
cd "$gitdirectroy/ralph"
if test $(pwd) = "$gitdirectroy/ralph"
then
	git fetch --all
	git checkout "$ralph_version"
else
	echo "ERROR: Could not cd into ralph folder. Check if it exists."
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
cd $gitdirectroy

#git clone pcl
cd $gitdirectroy
if [ ! -d "$gitdirectroy/pcl" ]; then
	git clone https://github.com/PointCloudLibrary/pcl.git
else
	echo "INFO: pcl repo detected in $gitdirectroy. Skipping clone."
fi
cd "$gitdirectroy/pcl"
if test $(pwd) = "$gitdirectroy/pcl"
then
	git fetch --all
	git checkout "$pcl_version"
else
	echo "ERROR: Could not cd into pcl folder. Check if it exists."
fi

#git clone g2o
cd $gitdirectroy
if [ ! -d "$gitdirectroy/g2o" ]; then
	git clone https://github.com/RainerKuemmerle/g2o.git
else
	echo "INFO: g2o repo detected in $gitdirectroy. Skipping clone."
fi
cd "$gitdirectroy/g2o"
if test $(pwd) = "$gitdirectroy/g2o"
then
	git fetch --all
	git checkout "$g2o_version"
else
	echo "ERROR: Could not cd into g2o folder. Check if it exists."
fi
echo "All done, remember to build and have a nice day :)"


