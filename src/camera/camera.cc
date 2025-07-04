#include "camera.hh"
#include "vectors/vector_definitions.hh"
#include "vectors/vector_methods.hh"
#include "vectors/vector_overloads.hh"

#include <algorithm>
#include <optional>
#include <ranges>
#include <vector>

using Vectors::Vec;
using Vectors::operator-;
using Vectors::operator+;

auto Camera::get_pinhole_pos() -> Vec<3, double> { return pinhole_pos; }

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

auto Camera::get_calculated_height() -> std::size_t & { return height; }

auto Camera::populate_pixel_directions() -> void {
  // Fixed distance between the pinhole and the projection plane
  pinhole_pos = Vec<3, double>({0.0, 0.0, -1.0});

  // The passed width and height are known in pixels, now we need a coord space
  // which links the FOV and the dimensions

  // First convert to radians
  fov = fov * Vectors::PI / 180.0;

  // Assuming the pinhole distance is 1 - hardcoded true
  auto proj_plane_width = tan(fov / 2) * 2;
  auto proj_plane_height = proj_plane_width / aspect_ratio;

  auto x_offset = proj_plane_width / 2.0;
  auto y_offset = proj_plane_height / 2.0;

  // The iterators
  auto const width_iterator = std::views::iota(std::size_t{0}, width);
  auto const height_iterator = std::views::iota(std::size_t{0}, height);

  auto const x_scale = proj_plane_width / static_cast<double>(width);
  auto const y_scale = proj_plane_height / static_cast<double>(height);

  // Populate the array initially with the local coordinate of each pixel
  for (auto y_index : height_iterator) {
    for (auto x_index : width_iterator) {
      pixel_directions[y_index][x_index] =
          Vec<3, double>({(x_scale * x_index) - x_offset,
                          (y_scale * y_index) - y_offset, 0.0});
    }
  }
  // clang-format off

  // Turns the coordinates into directions from the pinhole position, and applies rotations
  // Transformation order: Roll, Pitch, Yaw, Offset
  std::transform(
      pixel_directions.begin(), pixel_directions.end(), pixel_directions.begin(),
      [&](auto &row) {
        std::transform(row.begin(), row.end(), row.begin(), [&](auto& vec) {
          // Direction from origin
          vec = vec - pinhole_pos;
          // Rotating the directions
          auto rotated_vec = Vec<3, double>{
            vec[0] * std::cos(roll) - vec[1] * std::sin(roll),
            vec[0] * std::sin(roll) + vec[1] * std::cos(roll),
            vec[2]
          };
          auto pitched_vec = Vec<3, double>{
            rotated_vec[0],
            rotated_vec[1] * std::cos(pitch) - rotated_vec[2] * std::sin(pitch),
            rotated_vec[1] * std::sin(pitch) + rotated_vec[2] * std::cos(pitch)
          };
          auto yawed_vec = Vec<3, double>{
            pitched_vec[0] * std::cos(yaw) + pitched_vec[2] * std::sin(yaw),
            pitched_vec[1],
            pitched_vec[2] * std::cos(yaw) - pitched_vec[0] * std::sin(yaw),
          };
          return yawed_vec;
        });
        return row;
      });
  // clang-format on

  // Finally, offset the pinhole position to move the relative position of the
  // direction vectors
  pinhole_pos = pinhole_pos + offset;
};
