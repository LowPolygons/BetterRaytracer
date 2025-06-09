#ifndef READERS_SCENE_CONFIG_READER_HH
#define READERS_SCENE_CONFIG_READER_HH

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "scene/SceneConfig.hh"

namespace ConfigReader {
// Confirm if the config file exists, and if it does reads the lines
auto validate_config(const std::string &file_path)
    -> std::optional<std::vector<std::string>>;

// Sanitise everything and get the lines you want
auto clean_up_lines(std::vector<std::string> &lines)
    -> std::unordered_map<std::string, std::string>;

// Try convert and populate the SceneConfig object, excluding the objects
auto interpret_lines(SceneConfig &scene_config,
                     std::unordered_map<std::string, std::string> lines)
    -> bool;
} // namespace ConfigReader

#endif
