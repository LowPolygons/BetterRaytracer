#!/bin/bash

#echo "============== Compiling Debug Build =============="
#meson setup --wipe build-debug --buildtype=debug
#meson compile -C build-debug
#echo "============== Running Debug Build =============="
#time ./build-debug/src/raytracer

echo "============== Compiling Optimised Build =============="
meson setup --wipe build --buildtype=debugoptimized -Dcpp_args='-O3'
meson compile -C build
echo "============== Running Optimised Build =============="
time ./build/src/raytracer
