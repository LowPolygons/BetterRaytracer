#!/bin/bash 

meson setup --wipe build 

meson test -C build 

meson compile -C build 

./build/src/raytracer
