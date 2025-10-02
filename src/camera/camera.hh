#ifndef CAMERA_CAMERA_HH

#define CAMERA_CAMERA_HH

#include "vectors/vector_definitions.hh"

#include <optional>
#include <vector>

using Vectors::Vec;

// MANUALLY TESTED
class Camera {
public:
  Camera(std::size_t _width, double _aspect_ratio, double _fov_degrees,
         float horiz_rotation, float vert_rotation, float camera_rotation,
         Vec<3, double> _offset)
      : width(_width), height(width / _aspect_ratio),
        aspect_ratio(_aspect_ratio), fov(_fov_degrees), yaw(horiz_rotation),
        pitch(vert_rotation), roll(camera_rotation), offset(_offset),
        pixel_directions(std::vector<std::vector<Vec<3, double>>>(
            height, std::vector<Vec<3, double>>(width))) {};

  auto populate_pixel_directions() -> void;

  // Not called very often, so the overhead of std::optional not concerning
  auto get_pixel(std::size_t width, std::size_t height)
      -> std::optional<Vec<3, double>>;
  auto get_row(std::size_t height)
      -> std::optional<std::vector<Vec<3, double>>>;
  auto get_pinhole_pos() -> Vec<3, double>;
  auto get_calculated_height() -> std::size_t &;

  auto get_pixel_directions_flattened() const
      -> const std::vector<Vec<3, double>>;

private:
  std::size_t width;
  std::size_t height;

  double aspect_ratio;
  double fov;

  float yaw;
  float pitch;
  float roll;

  Vec<3, double> offset;
  Vec<3, double> pinhole_pos;

  // TODO: Rework to be 1D as well to make indexes match with pixel buffer
  std::vector<std::vector<Vec<3, double>>> pixel_directions;
};

#endif
