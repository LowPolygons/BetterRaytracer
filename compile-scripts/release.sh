#!/bin/bash

echo "============== Compiling Optimised Build =============="
meson setup --wipe build --buildtype="release" -Dcpp_args="-O3"
meson test -C build
meson compile -C build
