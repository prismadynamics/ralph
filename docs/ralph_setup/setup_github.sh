#!/bin/bash
cd
gitdirectroy="$HOME/githome"
github_url="https://github.com/prismadynamics/ralph.git"

github_username="prismadynamics@gmail.com"
github_pswd="prismagithub123"

if [ ! -d $gitdirectroy ]; then
	mkdir gitdirectroy
else
	echo "INFO: $gitdirectroy directory already exists."
fi
cd $gitdirectroy

if [ ! -d "ralph" ]; then
	git clone $github_url
else
	echo "INFO: ralph repo detected in $gitdirectroy. Skipping clone."
	echo "INFO: This script is intended for first time setup."
fi
cd ralph
git fetch --all
git pull
git config --global credential.helper wincred

# git clone opencv
opencv_version="4.1.0"
cd gitdirectroy
if [ ! -d "opencv" ]; then
	git clone https://github.com/opencv/opencv.git
else
	echo "INFO: opencv repo detected in $gitdirectroy. Skipping clone."
fi
if [ ! -d "opencv_contrib" ]; then
	git clone https://github.com/opencv/opencv_contrib.git
else
	echo "INFO: opencv_contribnCV repo detected in $gitdirectroy. Skipping clone."
fi
cd opencv
if [$pwd -eq "$HOME/githome/opencv"]; then
	git fetch --all
	git checkout $opencv_version
fi
cd gitdirectroy
cd opencv_contrib
if [$pwd -eq "$HOME/githome/opencv"]; then
	git fetch --all
	git checkout $opencv_version
fi
cd gitdirectroy





