#include "ColourReader.hh"
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

auto ColourReader::validate_config(const std::string &file_path)
    -> std::optional<std::vector<std::string>> {
  return FileReader::read_file_lines(file_path);
}

// Sanitise everything and get the lines you want
auto ColourReader::clean_up_lines(std::vector<std::string> &lines)
    -> std::unordered_map<std::string, std::string> {
  auto constexpr invalid_char = '[';

  // Headers removed
  auto only_desired_lines =
      FileReader::filter_desired_lines(lines, invalid_char);
  // Get the K:V Pairs
  auto key_value_pairs =
      FileReader::split_lines_across_equals(only_desired_lines);

  return key_value_pairs;
}

// Try convert and populate the SceneConfig object, excluding the objects
auto ColourReader::interpret_lines(
    std::unordered_map<std::string, BasicColour> &colours,
    std::unordered_map<std::string, std::string> lines) -> bool {
  // Checks number of items is fine, they all get converted correctly
  // Any undesirable values get converted to -1
  auto sanitise_line =
      [&](const std::string &colour_vals) -> std::optional<BasicColour> {
    auto vals = std::vector<std::string>{};
    auto stream = std::istringstream(colour_vals);
    auto curr_word = std::string{};

    while (stream >> curr_word) {
      vals.push_back(curr_word);
    }

    if (vals.size() != 8)
      return std::nullopt;

    auto output = BasicColour{0, 0, 0, 0, 0, 0, 0, 0};

    std::transform(
        vals.begin(), vals.end(), output.begin(), [&](auto value) -> float {
          auto maybe_casted_val = FileReader::generalised_cast<float>(value);

          if (!maybe_casted_val.has_value())
            return -1.0;

          return maybe_casted_val.value();
        });

    return output;
  };

  for (auto &pair : lines) {
    auto current_colour = sanitise_line(pair.second);

    // If any colours fail then return early
    if (!current_colour.has_value())
      return false;

    colours[pair.first] = current_colour.value();
  }

  return true;
}
