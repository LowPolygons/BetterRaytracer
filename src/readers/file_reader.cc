#include "file_reader.hh"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

auto FileReader::read_file_lines(const std::string &filename)
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
    if (line.size() != 0)
      lines.push_back(line);
  }
  return lines;
}

// Removes any lines beginning with invalid_line_start parameter
auto FileReader::filter_desired_lines(const std::vector<std::string> &lines,
                                      const char &invalid_line_start)
    -> std::vector<std::string> {
  auto filtered_lines = std::vector<std::string>{};

  // TODO: Swap so it accepts a string of any invalid chars
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

  auto trim = [&](auto line) {
    auto start = line.find_first_not_of(" \t\r\n");

    // In case the whole line is invalid
    if (start == std::string::npos)
      return std::string{};

    auto end = line.find_last_not_of(" \t\r\n");

    return line.substr(start, end - start + 1);
  };

  for (const auto &line : lines) {
    auto equals = line.find("=");

    if (equals != std::string::npos) {
      // TODO: If it contains an equals, split it and then trim whitespace
      if (equals != line.length() - 1) {
        auto key = trim(line.substr(0, equals));
        auto value = trim(line.substr(equals + 1));

        line_pairs[key] = value;
      }
    }
  }

  return line_pairs;
}
