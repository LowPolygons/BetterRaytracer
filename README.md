# Raytracer
This is a total remaster of an old Raytracer I made, only now I aim for pleasant code

All maths used was derived by me - that is not to say it is brand new!

This was done to learn better programming practises in C++, as such I chose to use no external libraries. Why not reinvent the wheel! 

## How To Use 
Clone the Repository

Run the `get_dependencies.sh` script (for Debian/Ubuntu platforms) to automatically install all needed packages
Alternatively, requirements are:
- Meson
- Ninja
- 
(both can be pip installed)
- C++23 compilter

Run `compile-scripts/release.sh` to compile the program

Modify the Colour, Object and Scene `.ini` and set the configuration you want.

* [Input Values](./instructions/ini_values.md)

Run `./build/raytracer`

## Example Scenes

## 500 Rays, 10 Bounces 

![Example Scene 1](scenes/SimpleColour.png)

## 1000 Rays, 20 Bounces 

![Example Scene 2](scenes/RedSphereDarkRoom.png)

![Example Scene 3](scenes/StunningPink.png)

![Example Scene 4](scenes/DarkPinkRoom.png)

## 1200 Rays, 20 Bounces

![Example Scene 5](scenes/MirrorsAllAround.png)
