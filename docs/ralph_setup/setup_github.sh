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
	echo "INFO: Ralph repo detected in $gitdirectroy. Skipping clone."
	echo "INFO: This script is intended for first time setup."
fi
cd ralph
git fetch --all
git pull
git config --global credential.helper wincred





