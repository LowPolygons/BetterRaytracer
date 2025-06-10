#include "SceneConfig.hh"
#include "vectors/vector_definitions.hh"
#include "window/scene_objects/scene_objects.hh"
#include <iostream>

#include "colour/colour.hh"

#include <cstdint>
#include <optional>
#include <string>
using Colours::BasicColour;
using Vectors::PI;

// Some Colour Definitions
BasicColour constexpr PLAIN_WHITE = {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0};
BasicColour constexpr WASHED_RED = {0.699, 0.25, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0};
BasicColour constexpr WASHED_GREEN = {0.25, 0.699, 0.25, 0.0,
                                      0.0,  0.0,   0.0,  0.0};
BasicColour constexpr WASHED_BLUE = {0.25, 0.25, 0.699, 0.0,
                                     0.0,  0.0,  0.0,   0.0};
BasicColour constexpr LIGHT_GREY = {0.8, 0.8, 0.8, 0.0, 0.0, 0.0, 0.0, 0.0};

BasicColour constexpr WHITE_MIRROR = {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};

BasicColour constexpr DARK_MIRROR = {0.75, 0.75, 0.75, 0.0, 0.0, 0.0, 0.0, 1.0};

BasicColour constexpr WHITE_LIGHT = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 2.0, 0.0};

BasicColour constexpr BRIGHT_WHITE_LIGHT = {
    0.0, 0.0, 0.0, 247.0 / 255.0, 223.0 / 225.0, 178.0 / 225.0, 10.0, 0.0};

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

BasicColour constexpr MATTE_BLACK = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
BasicColour constexpr RUBY_RED = {0.6, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.25};
BasicColour constexpr NEON_CYAN = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0};
BasicColour constexpr SOFT_PURPLE = {0.5, 0.3, 0.6, 0.0, 0.0, 0.0, 0.0, 0.1};
BasicColour constexpr ELECTRIC_BLUE = {0.0, 0.0, 0.0, 0.3, 0.7, 1.0, 1.0, 0.0};
BasicColour constexpr DULL_ORANGE_METAL = {0.8, 0.4, 0.1, 0.0,
                                           0.0, 0.0, 0.0, 0.3};
BasicColour constexpr GLASSY_GREEN = {0.6, 0.9, 0.6, 0.0, 0.0, 0.0, 0.0, 0.6};
BasicColour constexpr SKY_BLUE = {0.4, 0.6, 1.0, 0.0, 0.0, 0.0, 0.0, 0.2};
BasicColour constexpr RED_METAL = {0.76, 0.235, 0.235, 0.0,
                                   0.0,  0.0,   0.0,   0.35};

Function SceneConfig::GetSceneSetup() {
  // --- Central Cluster of Spheres ---
  SceneSetup.add_sphere(Sphere({0.0, 100.0, 750.0}, 325.0, RED_METAL));
  SceneSetup.add_sphere(
      Sphere({0.0, -1000.0, 750.0}, 200.0, BRIGHT_WHITE_LIGHT));

  // --- Cuboid 1: Dark pedestal ---
  SceneSetup.add_cuboid({-300.0, -1000.0, -300.0}, {0.0, -1000.0, -300.0},
                        {0.0, -1000.0, 0.0}, {-300.0, -1000.0, 0.0},
                        {-300.0, -700.0, -300.0}, {0.0, -700.0, -300.0},
                        {0.0, -700.0, 0.0}, {-300.0, -700.0, 0.0}, DARK_MIRROR,
                        DARK_MIRROR,                // Down
                        GENTLE_RED, GENTLE_GREEN,   // Left
                        SILVER_METAL, SILVER_METAL, // Right
                        PLAIN_WHITE, PLAIN_WHITE,   // Back
                        GENTLE_BLUE, GENTLE_BLUE,   // Front
                        PLAIN_WHITE, PLAIN_WHITE    // Up
  );

  SceneSetup.add_cuboid({-1000.0, -1000.0, -1000.0}, {1000.0, -1000.0, -1000.0},
                        {1000.0, -1000.0, 1000.0}, {-1000.0, -1000.0, 1000.0},
                        {-1000.0, 1000.0, -1000.0}, {1000.0, 1000.0, -1000.0},
                        {1000.0, 1000.0, 1000.0}, {-1000.0, 1000.0, 1000.0},
                        PLAIN_WHITE,
                        PLAIN_WHITE,                //
                        WASHED_RED, WASHED_RED,     //
                        PLAIN_WHITE, PLAIN_WHITE,   //
                        WASHED_GREEN, WASHED_GREEN, //
                        WASHED_BLUE, WASHED_BLUE,   //
                        LIGHT_GREY, LIGHT_GREY      //
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
  switch (RenderOption) {
  case NOGPU: {
    std::cout << "<-=| Rendering Mode => No GPU" << std::endl;
    if (!StoreResultToFile)
      std::cout
          << "\n<-WARN-> Without saving the file, you will have no way to "
             "view your scene!\n"
          << std::endl;
    break;
  }
  case SFML:
    std::cout << "<-=| Rendering Mode => SFML" << std::endl;
  }

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
