#ifndef READERS_OBJECT_CONFIG_HH
#define READERS_OBJECT_CONFIG_HH

#include <unordered_map>

#include "colour/colour.hh"
#include "scene/SceneConfig.hh"

using Colours::BasicColour;
using Scene::SceneConfig;

namespace ObjectConfigReader {
// Confirm if the config file exists, and if it does reads the lines
auto validate_config(const std::string &file_path)
    -> std::optional<std::vector<std::string>>;

// Sanitise everything and get the lines you want
auto clean_up_lines(std::vector<std::string> &lines)
    -> std::vector<std::string>;

// Try convert and populate the SceneConfig object, excluding the objects
auto interpret_lines(SceneConfig &scene_config, std::vector<std::string> lines,
                     std::unordered_map<std::string, BasicColour> colours)
    -> bool;
} // namespace ObjectConfigReader

#endif
