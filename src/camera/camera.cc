#include "camera.hh"
#include "vectors/vector_definitions.hh"

#include <vector>

using Vectors::Vec;

auto Camera::populate_pixel_positions() -> void {};

auto Camera::get_pixel(std::size_t width, std::size_t height)
    -> Vec<3, double> {};

auto Camera::get_row(std::size_t height) -> std::vector<Vec<3, double>> {};
