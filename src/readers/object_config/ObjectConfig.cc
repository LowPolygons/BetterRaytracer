#include "ObjectConfig.hh"
#include "colour/colour.hh"
#include "readers/file_reader.hh"
#include "scene/SceneConfig.hh"
#include <functional>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>
using Colours::BasicColour;

auto ObjectConfigReader::validate_config(const std::string &file_path)
    -> std::optional<std::vector<std::string>> {
  return FileReader::read_file_lines(file_path);
}

auto ObjectConfigReader::clean_up_lines(std::vector<std::string> &lines)
    -> std::vector<std::string> {
  auto constexpr invalid_char = '[';

  // Headers removed
  auto only_desired_lines =
      FileReader::filter_desired_lines(lines, invalid_char);
}

auto ObjectConfigReader::interpret_lines(
    SceneConfig &scene_config, std::vector<std::string> lines,
    std::unordered_map<std::string, BasicColour> colours) -> bool {
  auto sanitise_line = [&](const std::string &colour_vals) {
    auto vals = std::vector<std::string>{};
    auto stream = std::istringstream(colour_vals);
    auto curr_word = std::string{};

    while (stream >> curr_word) {
      vals.push_back(curr_word);
    }

    return vals;
  };
  for (auto &line : lines) {
    if (line.find("sphere")) {
      auto sphere_vals = sanitise_line(line);

      if (sphere_vals.size() != 6)
        return false;

      // TODO: cast to correct types and then add to the scene config
    }
    // TODO: do logic for cuboids and triangles
  }
}
