#ifndef CAMERA_CAMERA_HH

#define CAMERA_CAMERA_HH

#include "vectors/vector_definitions.hh"

#include <vector>

using Vectors::Vec;

class Camera {
private:
  std::size_t width;
  std::size_t height;
  std::size_t dist_from_screen;

  float xz_angle;
  float y_xz_angle;

  // TODO: consider swapping to an mdspan for its contiguous memory as well as
  // its non owning nature. Then you can compress this into a 1d array and
  // access via mdspan
  // Ask an AI to explain how to use experimental/mdspan in a meson project
  std::vector<std::vector<Vec<3, double>>> pixel_positions;

  // Do some spherical coordinate magic again
  // Most likely store this instantiated object in a mutex so that all threads
  // can access it
  // Probably in a shared_ptr
  Camera(std::size_t _width, std::size_t _height, std::size_t _dist_from_screen,
         float _xz_angle, float _y_xz_angle) {
    width = _width;
    height = _height;
    dist_from_screen = _dist_from_screen;
    xz_angle = _xz_angle;
    y_xz_angle = _y_xz_angle;
  };

  auto populate_pixel_positions() -> void;

  auto get_pixel(std::size_t width, std::size_t height) -> Vec<3, double>;
  auto get_row(std::size_t height) -> std::vector<Vec<3, double>>;
};

#endif
