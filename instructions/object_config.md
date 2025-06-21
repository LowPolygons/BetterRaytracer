## Object Configuration

The most important thing in the object config is that for multi line objects that there are no line breaks and no interrupts

### Parameters 

A Sphere is defined on one line with the following format:
`sphere {x} {y} {z} {radius} {colour_name}`

A Cuboid is defined on multiple lines with the following format:
`cuboid`
`{x} {y} {z}` x8 (corners)
`{colour_name} {colour_name}` x6 (2 colours per wall)

Wall order:
- Down1 Down2 
- Left1 Left2 
- Right1 Right2
- Back1 Back2 
- Front1 Front2
- Up1 Up1

Note: colour_name should be a colour listed in the [colour_config](./colour_config.md)

#### TODO: Triangles!

### Example `object_config.ini`

```
sphere 0 -9.75 6.5 1.75 BRIGHT_WHITE_LIGHT

sphere -4.85 0 0.5 3.25 BLUE_L
sphere -4.85 0 -3.0 0.34 PINK_SH 

sphere 4.85 0 0.5 3.25 PINK_L
sphere 4.85 0 -3.0 0.34 BLUE_SH 

cuboid
-10 -10 -10
10 -10 -10
10 -10 10
-10 -10 10
-10 10 -10
10 10 -10
10 10 10
-10 10 10
BLUE_Z WHITE
PINK_H BLUE_SH 
GREEN_H PINK_Z
WHITE BLUE_L
PINK_L GREEN_L
BLUE_H WHITE
```
