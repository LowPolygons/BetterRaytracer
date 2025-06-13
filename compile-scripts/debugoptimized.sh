#!/bin/bash

echo "============== Compiling Optimised Build =============="
meson setup --wipe build --buildtype=debugoptimized -Dcpp_args='-O3'
meson compile -C build
echo "============== Running Optimised Build =============="
time ./build/src/raytracer
