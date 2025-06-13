#!/bin/bash

echo "============== Compiling Optimised Build =============="
meson setup --wipe build --buildtype=release -Dcpp_args='-O3 -lGL'
meson compile -C build

