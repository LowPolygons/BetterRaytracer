#include "camera.hh"
#include "vectors/vector_definitions.hh"
#include "vectors/vector_methods.hh"
#include "vectors/vector_overloads.hh"

#include <algorithm>
#include <iostream>
#include <optional>
#include <ranges>
#include <vector>

using Vectors::Vec;
using Vectors::operator-;
using Vectors::operator+;

auto Camera::get_pinhole_pos() -> Vec<3, double> { 
  return pinhole_pos; 
}

auto Camera::get_pixel(std::size_t width, std::size_t height)
    -> std::optional<Vec<3, double>> {
  if (0 <= height and height < pixel_directions.size()) {
    if (0 <= width and width < pixel_directions[height].size()) {
      return pixel_directions[height][width];
    }

  }
  return std::nullopt;
};

auto Camera::get_row(std::size_t height)
    -> std::optional<std::vector<Vec<3, double>>> {
  if (0 <= height and height < pixel_directions.size()) {
    return pixel_directions[height];
  }
  return std::nullopt;
}

auto Camera::populate_pixel_directions() -> void {
  // Use the dist from screen to calculate the initial vectors needed for each
  // pixel Then loop through the vectors and rotate them all by the given angles
  // Finally use the offset given to offset the entire thing

  pinhole_pos =
      Vec<3, double>({0.0, 0.0, -1.0 * static_cast<double>(dist_from_screen)});

  // Needed during the spherical coordinates
  auto global_z_axis = Vec<3, double>({0.0, 0.0, 1.0});
  auto global_y_axis = Vec<3, double>({0.0, 1.0, 0.0});

  // Temporarily store the positions of each pixel in the pixel_directions array
  auto x = std::views::iota(std::size_t{0}, width);
  auto y = std::views::iota(std::size_t{0}, height);

  auto x_offset = (static_cast<double>(width) - 1.0) / 2.0;
  auto y_offset = (static_cast<double>(height) - 1.0) / 2.0;

  // Populate the array
  for (auto y_index : y) {
    for (auto x_index : x) {
      pixel_directions[y_index][x_index] =
          Vec<3, double>({static_cast<double>(x_index) - x_offset,
                          static_cast<double>(y_index) - y_offset, 0.0});
    }
  }

  // Make the new formed vectors become the vectors from the origin,
  // then rotate them correctly
  std::transform(
      pixel_directions.begin(), pixel_directions.end(),
      pixel_directions.begin(), //
      [&](auto &row) {
        std::transform(row.begin(), row.end(), row.begin(), [&](auto &vec) {
          vec = vec - pinhole_pos; // The new local Z axis
          auto rotated_vec = vec;
          auto pitched_vec = rotated_vec;
          // First rotate the points in the angle the camera should be at
          rotated_vec[0] =
              vec[0] * std::cos(camera_angle) - vec[1] * std::sin(camera_angle);
          rotated_vec[1] =
              vec[0] * std::sin(camera_angle) + vec[1] * std::cos(camera_angle);
          rotated_vec[2] = vec[2];

          // Rotation in the x axis to then control pitch
          pitched_vec[0] = rotated_vec[0];
          pitched_vec[1] = rotated_vec[1] * std::cos(y_xz_angle) -
                           rotated_vec[2] * std::sin(y_xz_angle);
          pitched_vec[2] = rotated_vec[1] * std::sin(y_xz_angle) +
                           rotated_vec[2] * std::cos(y_xz_angle);

          // Rotation in the y axis to control yaw
          vec[0] = pitched_vec[0] * std::cos(xz_angle) +
                   pitched_vec[2] * std::sin(xz_angle);
          vec[1] = pitched_vec[1];
          vec[2] = pitched_vec[2] * std::cos(xz_angle) -
                   pitched_vec[0] * std::sin(xz_angle);

          return vec;
        });
        return row;
      });

  // Finally, add the offset to the pinhole camera
  pinhole_pos = pinhole_pos + offset;
};
