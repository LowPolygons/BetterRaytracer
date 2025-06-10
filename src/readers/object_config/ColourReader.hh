#ifndef READERS_OBJECT_COLOUR_READER_HH
#define READERS_OBJECT_COLOUR_READER_HH

#include "colour/colour.hh"
#include "readers/file_reader.hh"
#include "scene/SceneConfig.hh"

using Colours::BasicColour;

namespace ColourReader {
// Confirm if the config file exists, and if it does reads the lines
auto validate_config(const std::string &file_path)
    -> std::optional<std::vector<std::string>>;

// Sanitise everything and get the lines you want
auto clean_up_lines(std::vector<std::string> &lines)
    -> std::unordered_map<std::string, std::string>;

// Try convert and populate the SceneConfig object, excluding the objects
auto interpret_lines(std::unordered_map<std::string, BasicColour> &colours,
                     std::unordered_map<std::string, std::string> lines)
    -> bool;
} // namespace ColourReader

#endif
