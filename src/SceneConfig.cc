#include "SceneConfig.hh"
#include "vectors/vector_definitions.hh"
#include "window/scene_objects/scene_objects.hh"

#include "colour/colour.hh"

#include <cstdint>
#include <optional>
#include <string>
using Colours::BasicColour;
using Vectors::PI;

// Some Colour Definitions
BasicColour constexpr PLAIN_WHITE = {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0};

BasicColour constexpr WHITE_MIRROR = {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};

BasicColour constexpr DARK_MIRROR = {0.75, 0.75, 0.75, 0.0, 0.0, 0.0, 0.0, 1.0};

BasicColour constexpr WHITE_LIGHT = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};

BasicColour constexpr BRIGHT_WHITE_LIGHT = {0.0, 0.0, 0.0,  1.0,
                                            1.0, 1.0, 10.0, 0.0};

BasicColour constexpr SILVER_METAL = {0.7, 0.7, 0.7, 0.0, 0.0, 0.0, 0.0, 0.35};

BasicColour constexpr GENTLE_RED = {0.76, 0.235, 0.235, 0.0,
                                    0.0,  0.0,   0.0,   0.0};
BasicColour constexpr GENTLE_BLUE = {0.235, 0.235, 0.76, 0.0,
                                     0.0,   0.0,   0.0,  0.0};
BasicColour constexpr GENTLE_GREEN = {0.235, 0.76, 0.235, 0.0,
                                      0.0,   0.0,  0.0,   0.0};

BasicColour constexpr GENTLE_RED_GLOW = {0.0,   0.0,   0.0, 0.76,
                                         0.235, 0.235, 1.0, 0.0};

BasicColour constexpr GOLDEN_GLOW = {0.0, 0.0, 0.0, 1.0, 0.843, 0.0, 1.0, 0.0};

;

Function SceneConfig::GetSceneSetup() {
  WindowTitle = "Example Scene";

  NumThreads = 16;
  NumRays = 100;
  NumBounces = 1;

  Width = 1000;
  Height = 1000;
  FieldOfView = 300;
  HorizontalRotation = 0.0;
  VerticalRotation = 0.0;
  CameraRotation = 0.0;
  CameraPosition = {0.0, 0.0, -100.0};

  PrintPercentStatusEvery = 10;

  StoreResultToFile = false;
  DisplayResultOnScreen = false;

  SceneSetup.add_sphere(Sphere({-400.0, -700.0, 600.0}, 300.0, SILVER_METAL));

  SceneSetup.add_sphere(Sphere({100.0, 800.0, 400.0}, 200.0, WHITE_LIGHT));
  SceneSetup.add_sphere(Sphere({-1000.0, 1000.0, 1000.0}, 100.0, GOLDEN_GLOW));

  SceneSetup.add_cuboid({-1000.0, -1000.0, -1000.0}, {1000.0, -1000.0, -1000.0},
                        {1000.0, -1000.0, 1000.0}, {-1000.0, -1000.0, 1000.0},
                        {-1000.0, 1000.0, -1000.0}, {1000.0, 1000.0, -1000.0},
                        {1000.0, 1000.0, 1000.0}, {-1000.0, 1000.0, 1000.0},
                        PLAIN_WHITE,
                        PLAIN_WHITE,               //
                        GENTLE_RED, GENTLE_BLUE,   //
                        GENTLE_BLUE, WHITE_LIGHT,  //
                        GENTLE_RED, GENTLE_GREEN,  //
                        WHITE_MIRROR, DARK_MIRROR, //
                        PLAIN_WHITE, PLAIN_WHITE   //
  );
}

Function SceneConfig::DisplaySceneSetup() {
  std::cout << "//====| " << WindowTitle << " Setup |=====//" << std::endl;

  std::cout << "\n<======> Screen Config <======>" << std::endl;
  std::cout << "<-=| Screen Width  => " << Width << std::endl;
  std::cout << "<-=| Screen Height => " << Height << std::endl;

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
  std::cout << "<-=| Number of Spheres   => " << SceneSetup.get_spheres().size()
            << std::endl;
  std::cout << "<-=| Number of Triangles => "
            << SceneSetup.get_triangles().size() << std::endl;

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
