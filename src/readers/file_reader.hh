#ifndef FILE_READER_HH
#define FILE_READER_HH

#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace FileReader {

auto read_file_lines(const std::string &filename)
    -> std::optional<std::vector<std::string>>;

// Removes any lines beginning with invalid_line_start parameter
auto filter_desired_lines(const std::vector<std::string> &lines,
                          const char &invalid_line_start)
    -> std::vector<std::string>;

auto split_lines_across_equals(std::vector<std::string> &lines)
    -> std::unordered_map<std::string, std::string>;

template <typename T>
auto generalised_cast(const std::string &value) -> std::optional<T> {
  auto try_catch_numeric_cast = //
      [](const std::string &string_val, auto func) -> std::optional<T> {
    try {
      return std::optional<T>{func(string_val)};
    } catch (const std::invalid_argument &e) {
      return std::nullopt;
    } catch (const std::out_of_range &e) {
      return std::nullopt;
    }
  };

  // String-like
  if constexpr (std::is_same_v<T, std::string>)
    return value;

  // Bool-like
  if constexpr (std::is_same_v<T, bool>) {
    if (value == "true")
      return bool{true};
    if (value == "false")
      return bool{false};

    return std::nullopt;
  }

  // Numeric-like
  if constexpr (std::is_arithmetic<T>::value) {
    if (std::is_same_v<T, int>)
      return try_catch_numeric_cast(value, [](auto s) { return std::stoi(s); });

    if (std::is_same_v<T, double>)
      return try_catch_numeric_cast(value, [](auto s) { return std::stod(s); });

    if (std::is_same_v<T, float>)
      return try_catch_numeric_cast(value, [](auto s) { return std::stof(s); });
  }

  return std::nullopt;
}

}; // namespace FileReader

#endif
