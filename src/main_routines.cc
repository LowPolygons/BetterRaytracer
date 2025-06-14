
#include "main_routines.hh"
#include "scene/SceneConfig.hh"

#include "readers/object_config/ColourReader.hh"
#include "readers/object_config/ObjectConfig.hh"
#include "readers/scene_config/ConfigReader.hh"

#include <iostream>

auto MainMethods::set_scene_configuration(SceneConfig &scene_setup) -> bool {
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
  if (!ConfigReader::interpret_lines(scene_setup, cleaned_up_lines)) {
    std::cout << "Some values were invalid or missing in scene_config.ini"
              << std::endl;
    files_read_successfully = false;
  }
  if (!ColourReader::interpret_lines(colours, colours_cleaned)) {
    std::cout << "Some values were invalid in colour.ini" << std::endl;
    files_read_successfully = false;
  }
  if (!ObjectConfigReader::interpret_lines(scene_setup, cleaned_objects,
                                           colours)) {
    std::cout << "Something went wrong interpreting object config" << std::endl;
    files_read_successfully = false;
  }

  if (!files_read_successfully)
    return false;

  return true;
}
