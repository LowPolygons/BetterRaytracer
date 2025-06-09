#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "ConfigReader.hh"
#include "readers/file_reader.hh"
#include "scene/SceneConfig.hh"

auto ConfigReader::validate_config(const std::string &file_path)
    -> std::optional<std::vector<std::string>> {
  return FileReader::read_file_lines(file_path);
}

auto ConfigReader::clean_up_lines(std::vector<std::string> &lines)
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

auto ConfigReader::interpret_lines(
    SceneConfig &scene_config,
    std::unordered_map<std::string, std::string> lines) -> bool;
