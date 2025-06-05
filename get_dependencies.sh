#!/bin/bash 

set -e 

echo "Installing Necessary Packages for Program: "
echo " -> build-essential "
echo " -> meson "
echo " -> ninja-build "
echo " -> libsfml-dev "
echo " -> libsdl2-dev"
echo " -> libgl1-mesa-dev "


sudo apt update
sudo apt install -y build-essential meson ninja-build libsfml-dev libgl1-mesa-dev libsdl2-dev 
sudo apt upgrade

echo "This is but a humble script, correct any errors the terminal gives accordingly."
