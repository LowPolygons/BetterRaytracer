#!/bin/bash 

set -e 

echo "Installing Necessary Packages for Program: "
echo " -> build-essential "
echo " -> meson "
echo " -> ninja-build "

sudo apt update
sudo apt install -y build-essential meson ninja-build libsdl2-dev 
sudo apt upgrade

echo "This is but a humble script, correct any errors the terminal gives accordingly."
