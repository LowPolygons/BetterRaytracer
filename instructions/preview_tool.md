# Preview Tool

The preview tool can be enabled by setting `PreviewEnabled` to true in the scene_config.ini

It is used to ensure your set configuration is as desired before the expensive computation 

#### The preview tool requires SDL2 to be installed

## Graphics Info
The preview tool uses the same method of rendering as the raytracer with a few changes:

- The colour of an object in the final scene is determined by the colour of the object, not the light it receives
- #### This means that objects that will be light sources in the scene should appear completely black
- Each ray only shoots once and only hits once - no consideration for glossiness or reflections will be considered
- The dimensions of the preview are 800 pixels in width, and then matching your set aspect ratio

Essentially, is a low quality rasteriser

## Controls

### W A S D 
Move the position of the camera forward/left/back/right 

### PageUp/PageDown 
Move the position of the camera up/down 

### Up/Down/Left/Right Arrows
Pitch the camera camera up/down, yaw the camera left/right 

### Square Bracket [ ]
Rotate the camera anti-clockwise/clockwise 

### Enter 
Pressing enter means the Raytracer will copy the configuration you set inside the preview 

#### Note: these changes are not saved to the ini files, so make sure to look at the info dump before rendering if you want to save the changes

### Backspace 
Backspace will cancel any of the changes made and will run the raytracer with the original config 

#### Note: closing the window will mimic the effects of backspace
