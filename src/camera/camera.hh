#ifndef CAMERA_CAMERA_HH

#define CAMERA_CAMERA_HH

#include "vectors/vector_definitions.hh"

#include <optional>
#include <vector>

using Vectors::Vec;

// MANUALLY TESTED
class Camera {
private:
  std::size_t width;

  double aspect_ratio;
  double fov;

  float xz_angle;
  float y_xz_angle;
  float camera_angle;
  Vec<3, double> offset;

  std::size_t height;

  Vec<3, double> pinhole_pos;

  std::vector<std::vector<Vec<3, double>>> pixel_directions;

public:
  Camera(std::size_t _width, double _aspect_ratio, double _fov_degrees,
         float horiz_rotation, float vert_rotation, float camera_rotation,
         Vec<3, double> _offset)
      : width(_width), aspect_ratio(_aspect_ratio), fov(_fov_degrees),
        xz_angle(horiz_rotation), y_xz_angle(vert_rotation),
        camera_angle(camera_rotation), offset(_offset),
        height(width / aspect_ratio),
        pixel_directions(std::vector<std::vector<Vec<3, double>>>(
            height, std::vector<Vec<3, double>>(width))) {};

  auto populate_pixel_directions() -> void;

  auto get_pinhole_pos() -> Vec<3, double>;
  auto get_calculated_height() -> std::size_t &;

  auto get_pixel(std::size_t width, std::size_t height)
      -> std::optional<Vec<3, double>>;

  auto get_row(std::size_t height)
      -> std::optional<std::vector<Vec<3, double>>>;
};

#endif
