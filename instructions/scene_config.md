## Scene Configuration

Most of the values in the scene config are mandatory, with few exceptions

Any lines beginning with a square bracket are ignored

### Mandatory Parameters 
```
WindowTitle - String 
Width - Int (pixels)
AspectRatio - Float 
NumRays - Int 
NumBounces - Int 
ContributionPerBounce - Float 
FieldOfView - Float (Degrees)
HorizontalRotation - Float (Radians)
VerticalRotation - Float (Radians)
CameraRotation - Float (Radians)
CameraOffset_X - Float 
CameraOffset_Y - Float 
CameraOffset_Z - Float 
PrintPercentStatusEvery - Int
StoreResultToFile - true/false 
```

### Optional Parameters
```
NumThreads - Int
RandomSeed - Int
FileName - String
ColourGamma - Float
```

If you do not specify each of these, they default to:

- Max Available threads
- The result of `random_device` 
- "OutputScene_[Random(1,999999)]"
- 1 / 2.2

### Example `scene_config.ini`

```
[Screen]
WindowTitle = Example Scene
Width = 2560
AspectRatio = 1.777

[Simulation]
NumRays = 15
NumBounces = 5
ContributionPerBounce = 0.999

[Camera]
FieldOfView = 118
HorizontalRotation = 0
VerticalRotation = 0
CameraRotation = 0

CameraOffset_X = 0
CameraOffset_Y = 0
CameraOffset_Z = -8.99

[Logging]
PrintPercentStatusEvery = 2

[Misc]
StoreResultToFile = true
FileName = FileNameTest
```
