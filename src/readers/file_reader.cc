#include "file_reader.hh"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

auto FileReader::read_file_lines(std::string filename)
    -> std::optional<std::vector<std::string>> {
  auto file_container = std::ifstream(filename);

  if (!file_container)
    return std::nullopt;

  auto line = std::string{};
  auto lines = std::vector<std::string>{};

  // If it can't get any line,
  if (!std::getline(file_container, line))
    return std::vector<std::string>{};

  // Reset stream state back to beginning of file
  file_container.clear();
  file_container.seekg(std::size_t{0});

  while (std::getline(file_container, line)) {
    lines.push_back(line);
  }
  return lines;
}

// Removes any lines beginning with invalid_line_start parameter
auto FileReader::filter_desired_lines(const std::vector<std::string> &lines,
                                      char &invalid_line_start)
    -> std::vector<std::string> {
  auto filtered_lines = std::vector<std::string>{};

  // Any lines starting with the invalid_line_start omitted from filtered_lines
  std::copy_if(lines.begin(), lines.end(), std::back_inserter(filtered_lines),
               [&](const auto &line) {
                 if (!line.empty())
                   return line[0] != invalid_line_start;

                 return false;
               });

  return filtered_lines;
}

auto FileReader::split_lines_across_equals(std::vector<std::string> &lines)
    -> std::unordered_map<std::string, std::string> {
  auto line_pairs = std::unordered_map<std::string, std::string>{};

  for (const auto &line : lines) {
    auto equals = line.find("=");

    if (equals != std::string::npos) {
      // TODO: If it contains an equals, split it and then trim whitespace
    }
  }
}
