#ifndef FILE_READER_HH
#define FILE_READER_HH

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace FileReader {

auto read_file_lines(std::string filename)
    -> std::optional<std::vector<std::string>>;

// Removes any lines beginning with invalid_line_start parameter
auto filter_desired_lines(const std::vector<std::string> &lines,
                          char &invalid_line_start) -> std::vector<std::string>;

auto split_lines_across_equals(std::vector<std::string> &lines)
    -> std::unordered_map<std::string, std::string>;

}; // namespace FileReader

#endif
