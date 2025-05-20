#ifndef CAMERA_CAMERA_HH

#define CAMERA_CAMERA_HH

#include "vectors/vector_definitions.hh"

#include <optional>
#include <vector>

using Vectors::Vec;

class Camera {
private:
  std::size_t width;
  std::size_t height;
  std::size_t dist_from_screen;

  float xz_angle;
  float y_xz_angle;
  float camera_angle;

  Vec<3, double> offset;
  Vec<3, double> pinhole_pos;

  // TODO: consider swapping to an mdspan for its contiguous memory as well as
  // its non owning nature. Then you can compress this into a 1d array and
  // access via mdspan
  // Ask an AI to explain how to use experimental/mdspan in a meson project
  std::vector<std::vector<Vec<3, double>>> pixel_directions;

  // Do some spherical coordinate magic again
  // Most likely store this instantiated object in a mutex so that all threads
  // can access it
  // Probably in a shared_ptr
  Camera(std::size_t _width, std::size_t _height, std::size_t _dist_from_screen,
         float horiz_rotation, float vert_rotation, float camera_rotation,
         Vec<3, double> _offset) {
    width = _width;
    height = _height;
    dist_from_screen = _dist_from_screen;
    xz_angle = horiz_rotation;
    y_xz_angle = vert_rotation;
    camera_angle = camera_rotation;
    offset = _offset;

    pixel_directions = std::vector<std::vector<Vec<3, double>>>(
        height, std::vector<Vec<3, double>>(width));
  };

  auto populate_pixel_directions() -> void;

  auto get_pinhole_pos() -> Vec<3, double>;

  auto get_pixel(std::size_t width, std::size_t height)
      -> std::optional<Vec<3, double>>;

  auto get_row(std::size_t height)
      -> std::optional<std::vector<Vec<3, double>>>;
};

#endif
