#include "SceneConfig.hh"
#include "readers/object_config/ColourReader.hh"
#include "readers/object_config/ObjectConfig.hh"
#include "readers/scene_config/ConfigReader.hh"
#include "scene/scene_objects/scene_objects.hh"
#include <iostream>
#include <thread>

auto Scene::set_scene_config(SceneConfig &scene_config) -> bool {
  //==// Read the ini files to set the configuration //==//
  auto config_file_lines = ConfigReader::validate_config("scene_config.ini");
  auto colour_file_lines = ColourReader::validate_config("colour_data.ini");
  auto objects_file_lines =
      ObjectConfigReader::validate_config("object_config.ini");

  auto files_read_successfully = true;

  // Confirm they all exist
  if (!config_file_lines.has_value()) {
    std::cout << "Could not find scene_config.ini" << std::endl;
    files_read_successfully = false;
  }
  if (!colour_file_lines.has_value()) {
    std::cout << "Could not find colour_config.ini" << std::endl;
    files_read_successfully = false;
  }
  if (!objects_file_lines.has_value()) {
    std::cout << "Could not find object_config.ini" << std::endl;
    files_read_successfully = false;
  }

  if (!files_read_successfully)
    return false;

  auto cleaned_up_lines =
      ConfigReader::clean_up_lines(config_file_lines.value());
  auto colours_cleaned =
      ColourReader::clean_up_lines(colour_file_lines.value());
  auto cleaned_objects =
      ObjectConfigReader::clean_up_lines(objects_file_lines.value());

  auto colours = std::unordered_map<std::string, BasicColour>{};

  // Confirm all values were aquired
  if (!ConfigReader::interpret_lines(scene_config, cleaned_up_lines)) {
    std::cout << "Some values were invalid or missing in scene_config.ini"
              << std::endl;
    files_read_successfully = false;
  }
  if (!ColourReader::interpret_lines(colours, colours_cleaned)) {
    std::cout << "Some values were invalid in colour.ini" << std::endl;
    files_read_successfully = false;
  }
  if (!ObjectConfigReader::interpret_lines(scene_config, cleaned_objects,
                                           colours)) {
    std::cout << "Something went wrong interpreting object config" << std::endl;
    files_read_successfully = false;
  }

  if (!files_read_successfully)
    return false;

  // If number of threads wasn't specified it uses the maximum
  if (scene_config.NumThreads == 0)
    scene_config.NumThreads = std::thread::hardware_concurrency();

  return true;
}

// clang-format off
void Scene::SceneConfig::DisplaySceneSetup() {
  std::cout << "//====| Scene Setup |=====//" << std::endl;

  std::cout << "\n<======> Screen Config <======>" << std::endl;
  std::cout << "<-=| Screen Width  => " << Width << std::endl;
  std::cout << "<-=| Aspect Ratio  => " << AspectRatio << std::endl;

  std::cout << "\n<======> Simulation Config <======>" << std::endl;
  std::cout << "<-=| Number of Threads         => " << NumThreads << std::endl;
  std::cout << "<-=| Number of Rays Per Pixel  => " << NumRays << std::endl;
  std::cout << "<-=| Number of Bounces Per Ray => " << NumBounces << std::endl;
  if (SceneSeed) std::cout << "<-=| Random Seed => " << SceneSeed.value() << std::endl;

  std::cout << "\n<======> Camera Config <======>" << std::endl;

  std::cout << "<-=| Field Of View => " << FieldOfView << std::endl;
  std::cout << "<-=| Horizontal Rotation (rad) => " << HorizontalRotation << std::endl;
  std::cout << "<-=| Vertical Rotation   (rad) => " << VerticalRotation << std::endl;
  std::cout << "<-=| Camera Rotation     (rad) => " << CameraRotation << std::endl;
  std::cout << "<-=| Camera Position => [" 
    << CameraPosition[0] << ", " << CameraPosition[1] << ", " << CameraPosition[2] << "]" << std::endl;

  std::cout << "\n<======> Objects Config <======>" << std::endl;
  std::cout << "<-=| Number of shapes   => " << SceneSetup.get_triangles().size() + SceneSetup.get_spheres().size() << std::endl;

  std::cout << "\n<======> Misc Config <======>" << std::endl;
  std::cout << "<-=| Print Percentage Progress Every => " << PrintPercentStatusEvery << "%" << std::endl;

  if (StoreResultToFile) {
    std::cout << "<-=| Store Scene to PNG File => True" << std::endl;
    std::cout << "<-=| File Name => " << FileName << ".bmp" << std::endl;
  } else {
    std::cout << "<-=| Store Scene to PNG File => False" << std::endl;
  }
  std::cout << std::endl;
}
