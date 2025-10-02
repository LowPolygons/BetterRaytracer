#!/bin/bash

echo "============== Compiling Optimised Build =============="
CXX=acpp ACPP_TARGETS=omp OMP_NUM_THREADS=1 meson setup --wipe build --buildtype="release" -Dcpp_args="-O3"
meson test -C build
bear -- meson compile -C build
