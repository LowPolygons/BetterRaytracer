#include "ObjectConfig.hh"
#include "colour/colour.hh"
#include "geometry/geometry_menu.hh"
#include "geometry/shape.hh"
#include "geometry/sphere.hh"
#include "geometry/triangle.hh"
#include "readers/file_reader.hh"
#include "scene/SceneConfig.hh"
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
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

  return only_desired_lines;
}

auto ObjectConfigReader::interpret_lines(
    SceneConfig &scene_config, std::vector<std::string> lines,
    std::unordered_map<std::string, BasicColour> colours) -> bool {
  auto sanitise_line = [&](const std::string &input_vals) {
    auto vals = std::vector<std::string>{};
    auto stream = std::istringstream(input_vals);
    auto curr_word = std::string{};

    while (stream >> curr_word) {
      vals.push_back(curr_word);
    }

    return vals;
  };
  auto confirm_entity_exists = [&](const auto &key) {
    if (colours.find(key) == colours.end())
      return false;

    return true;
  };
  auto vector_grab = [&](const auto &line) -> std::optional<Vec<3, double>> {
    auto line_components = sanitise_line(line);

    if (line_components.size() != 3)
      return std::nullopt;

    auto maybe_x = FileReader::generalised_cast<double>(line_components[0]);
    auto maybe_y = FileReader::generalised_cast<double>(line_components[1]);
    auto maybe_z = FileReader::generalised_cast<double>(line_components[2]);

    // They must all exist
    if (!maybe_x.has_value() or !maybe_y.has_value() or !maybe_z.has_value())
      return std::nullopt;

    return Vec<3, double>{maybe_x.value(), maybe_y.value(), maybe_z.value()};
  };
  auto colours_grab = [&](const auto &line)
      -> std::optional<std::pair<BasicColour, BasicColour>> {
    auto line_components = sanitise_line(line);

    if (line_components.size() != 2)
      return std::nullopt;

    if (!confirm_entity_exists(line_components[0]) or
        !confirm_entity_exists(line_components[1]))
      return std::nullopt;

    return std::make_pair(colours[line_components[0]],
                          colours[line_components[1]]);
  };
  auto current_line_index = std::size_t{0};

  for (auto &line : lines) {
    if (line.find("sphere") != std::string::npos) {
      auto sphere_vals = sanitise_line(line);
      if (sphere_vals.size() != 6)
        return false;

      auto sphere_numeric_vals = std::vector<double>{};

      for (auto current_value : sphere_vals) {
        // Cast value to double and if it works add it to vals
        auto typed_val_maybe =
            FileReader::generalised_cast<double>(current_value);

        if (typed_val_maybe.has_value())
          sphere_numeric_vals.push_back(typed_val_maybe.value());
      }
      // This new list should exclude the sphere tag and the colour
      if (sphere_numeric_vals.size() != 4)
        return false;

      // The colour must exist
      if (!confirm_entity_exists(sphere_vals[5]))
        return false;
      // Create sphere with newly confirmed sphere
      scene_config.SceneSetup.add_sphere(
          Geometry(Sphere({sphere_numeric_vals[0], sphere_numeric_vals[1],
                           sphere_numeric_vals[2]},
                          sphere_numeric_vals[3], colours[sphere_vals[5]])));
    }
    if (line.find("cuboid") != std::string::npos) {
      // Check if the current index + 14 exists
      try {
        auto test_line = lines.at(current_line_index + 14);
      } catch (std::out_of_range &e) {
        return false;
      }
      // The eight coordinates
      auto maybe_coord1 = vector_grab(lines[current_line_index + 1]);
      auto maybe_coord2 = vector_grab(lines[current_line_index + 2]);
      auto maybe_coord3 = vector_grab(lines[current_line_index + 3]);
      auto maybe_coord4 = vector_grab(lines[current_line_index + 4]);
      auto maybe_coord5 = vector_grab(lines[current_line_index + 5]);
      auto maybe_coord6 = vector_grab(lines[current_line_index + 6]);
      auto maybe_coord7 = vector_grab(lines[current_line_index + 7]);
      auto maybe_coord8 = vector_grab(lines[current_line_index + 8]);
      // The six pairs of colours
      auto maybe_colours1 = colours_grab(lines[current_line_index + 9]);
      auto maybe_colours2 = colours_grab(lines[current_line_index + 10]);
      auto maybe_colours3 = colours_grab(lines[current_line_index + 11]);
      auto maybe_colours4 = colours_grab(lines[current_line_index + 12]);
      auto maybe_colours5 = colours_grab(lines[current_line_index + 13]);
      auto maybe_colours6 = colours_grab(lines[current_line_index + 14]);
      // Must all be valid
      if (!maybe_coord1.has_value() or !maybe_coord2.has_value() or
          !maybe_coord3.has_value() or !maybe_coord4.has_value() or
          !maybe_coord5.has_value() or !maybe_coord6.has_value() or
          !maybe_coord7.has_value() or !maybe_coord8.has_value() or
          !maybe_colours1.has_value() or !maybe_colours2.has_value() or
          !maybe_colours3.has_value() or !maybe_colours4.has_value() or
          !maybe_colours5.has_value() or !maybe_colours6.has_value()) {
        return false;
      }
      // All data valid so construct cuboid
      scene_config.SceneSetup.add_cuboid(
          maybe_coord1.value(), maybe_coord2.value(), maybe_coord3.value(),
          maybe_coord4.value(), maybe_coord5.value(), maybe_coord6.value(),
          maybe_coord7.value(), maybe_coord8.value(),
          maybe_colours1.value().first, maybe_colours1.value().second,
          maybe_colours2.value().first, maybe_colours2.value().second,
          maybe_colours3.value().first, maybe_colours3.value().second,
          maybe_colours4.value().first, maybe_colours4.value().second,
          maybe_colours5.value().first, maybe_colours5.value().second,
          maybe_colours6.value().first, maybe_colours6.value().second);
    }
    if (line.find("triangle") != std::string::npos) {
      // Check if the current index + 14 exists
      try {
        auto test_line = lines.at(current_line_index + 4);
      } catch (std::out_of_range &e) {
        return false;
      }
      // The 3 coordinates
      auto maybe_coord1 = vector_grab(lines[current_line_index + 1]);
      auto maybe_coord2 = vector_grab(lines[current_line_index + 2]);
      auto maybe_coord3 = vector_grab(lines[current_line_index + 3]);
      // Must all be valid
      if (!maybe_coord1.has_value() or !maybe_coord2.has_value() or
          !maybe_coord3.has_value()) {
        return false;
      }

      auto colour_name = sanitise_line(lines[current_line_index + 4]);
      if (colour_name.size() != 1)
        return false;

      if (!confirm_entity_exists(colour_name[0]))
        return false;

      // All data valid so construct cuboid
      scene_config.SceneSetup.add_triangle(
          Geometry(Triangle(maybe_coord1.value(), maybe_coord2.value(),
                            maybe_coord3.value(), colours[colour_name[0]])));
    }
    current_line_index++;
  }

  return true;
}
