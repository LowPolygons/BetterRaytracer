## Colour Configuration

A list of colours are defined starting with the name and then the following parameters

`COLOUR_NAME = {r} {g} {b} {l_r} {l_g} {l_b} {strength} {reflectivity}`

All values are between 0 and 1 excluding `strength` which can go above

#### For a non light emitting surface:
`l_r, l_g, l_b, strength` = 0

`r, g, b` = 0 -> 1
#### For a light emitting surface:
`l_r, l_g, l_b` = 0 -> 1 

`strength` = 1 -> ... 

`r, g, b` = 0

If there are few lights in the scene, consider a larger strength brightness such as 10

#### For a mirrored surface 
`reflectivity` = 1
#### For a typical surface
`reflectivity` = 0
#### For metalic looking surfaces 
Try `reflectivity` = 0.5 

### Example `colour_config.ini`

```
BRIGHT_WHITE_LIGHT = 0 0 0 1 1 1 10 0

BLUE_Z = 0.482 0.709 0.843 0 0 0 0 0
PINK_Z = 0.986 0.00784 0.5529 0 0 0 0 0
GREEN_Z = 0.6627 0.8313 0.709 0 0 0 0 0

BLUE_L = 0.482 0.709 0.843 0 0 0 0 0.35
PINK_L = 0.986 0.00784 0.5529 0 0 0 0 0.35
GREEN_L = 0.6627 0.8313 0.709 0 0 0 0 0.35

BLUE_H = 0.482 0.709 0.843 0 0 0 0 0.65
PINK_H = 0.986 0.00784 0.5529 0 0 0 0 0.65
GREEN_H = 0.6627 0.8313 0.709 0 0 0 0 0.65

BLUE_SH = 0 0 0 0.282 0.509 0.943 3 0 
PINK_SH = 0 0 0 0.986 0.00784 0.5529 6 0
GREEN_SH = 0 0 0 0.6627 0.8313 0.709 6 0

WHITE = 0.92 0.92 0.92 0 0 0 0 0
```
