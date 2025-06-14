#include "SceneConfig.hh"
#include "image/scene_objects/scene_objects.hh"
#include <iostream>

void SceneConfig::DisplaySceneSetup() {
  std::cout << "//====| " << WindowTitle << " Setup |=====//" << std::endl;

  std::cout << "\n<======> Screen Config <======>" << std::endl;
  std::cout << "<-=| Screen Width  => " << Width << std::endl;
  std::cout << "<-=| Aspect Ratio  => " << AspectRatio << std::endl;

  std::cout << "\n<======> Simulation Config <======>" << std::endl;
  std::cout << "<-=| Number of Threads         => " << NumThreads << std::endl;
  std::cout << "<-=| Number of Rays Per Pixel  => " << NumRays << std::endl;
  std::cout << "<-=| Number of Bounces Per Ray => " << NumBounces << std::endl;
  if (SceneSeed)
    std::cout << "<-=| Random Seed => " << SceneSeed.value() << std::endl;

  std::cout << "\n<======> Camera Config <======>" << std::endl;

  std::cout << "<-=| Field Of View => " << FieldOfView << std::endl;
  std::cout << "<-=| Horizontal Rotation (rad) => " << HorizontalRotation
            << std::endl;
  std::cout << "<-=| Vertical Rotation   (rad) => " << VerticalRotation
            << std::endl;
  std::cout << "<-=| Camera Rotation     (rad) => " << CameraRotation
            << std::endl;
  std::cout << "<-=| Camera Position => [" << CameraPosition[0] << ", "
            << CameraPosition[1]

            << ", " << CameraPosition[2] << "]" << std::endl;

  std::cout << "\n<======> Objects Config <======>" << std::endl;
  std::cout << "<-=| Number of shapes   => " << SceneSetup.get_shapes().size()
            << std::endl;

  std::cout << "\n<======> Misc Config <======>" << std::endl;
  std::cout << "<-=| Print Percentage Progress Every => "
            << PrintPercentStatusEvery << "%" << std::endl;

  if (StoreResultToFile) {
    std::cout << "<-=| Store Scene to PNG File => True" << std::endl;
  } else {
    std::cout << "<-=| Store Scene to PNG File => False" << std::endl;
  }

  if (DisplayResultOnScreen) {
    std::cout << "<-=| Display Scene to Screen => True" << std::endl;
  } else {
    std::cout << "<-=| Display Scene to Screen => False" << std::endl;
  }

  std::cout << "\n" << std::endl;
}
