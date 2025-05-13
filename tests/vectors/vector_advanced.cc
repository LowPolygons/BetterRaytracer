#include "vectors/vector_advanced.hh"
#include "vectors/vector_definitions.hh"
#include "vectors/vector_methods.hh"
#include "vectors/vector_overloads.hh"

#include <numeric>
#include <random>
#include <ranges>
#include <utility>

using Vectors::Line;
using Vectors::magnitude;
using Vectors::Plane;
using Vectors::Vec;

auto main() -> int {
  // Plane for use in all testing in this file
  auto test_plane =
      Plane<double>(std::make_pair(Vec<3, double>({0.0, 0.0, 1.0}), 10));

  // Line-Plane intersection
  auto line_intersecting_plane =
      Line<3, double>({0.0, 0.0, 0.0}, {0.0, 0.0, 1.0});
  auto line_parallel_to_plane =
      Line<3, double>({0.0, 0.0, 0.0}, {1.0, 0.0, 0.0});
  auto line_intersect_res =
      Vectors::line_intersects_plane(line_intersecting_plane, test_plane);
  auto line_parallel_res =
      Vectors::line_intersects_plane(line_parallel_to_plane, test_plane);

  // Point-Plane Check
  auto point_on_plane = Vec<3, double>({0.0, 0.0, 10.0});
  auto point_off_plane = Vec<3, double>({10.0, 0.0, 0.0});

  // Point-to-line distance
  auto test_line = Line<3, double>({0.0, 0.0, 0.0}, {1.0, 0.0, 0.0});
  auto test_point = Vec<3, double>({5.0, 5.0, 0.0});
  auto point_to_line = Vectors::point_to_line_distance(test_line, test_point);

  // Point Reflection
  auto point_to_reflect = Vec<3, double>({0.0, 0.0, 0.0});
  auto reflected_point =
      Vectors::reflect_point_across_plane(point_to_reflect, test_plane);

  // Angle between line and plane
  auto perp_line = Line<3, double>({0.0, 0.0, 0.0}, {0.0, 0.0, 1.0});
  auto parallel_line = Line<3, double>({0.0, 0.0, 0.0}, {1.0, 2.0, 0.0});
  auto perp_angle =
      Vectors::angle_between_line_and_plane(perp_line, test_plane);
  auto para_angle =
      Vectors::angle_between_line_and_plane(parallel_line, test_plane);

  if (Vectors::is_point_on_plane(point_on_plane, test_plane) &&
      !Vectors::is_point_on_plane(point_off_plane, test_plane) &&
      line_intersect_res && !line_parallel_res &&
      std::abs(magnitude(point_to_line.second)) - 5.0 < 1e-09 &&
      std::abs(reflected_point.at(2)) - 20.0 < 1e-09 && !para_angle &&
      std::abs(perp_angle.value()) - (3.1415926535 / 2.0) < 1e-09) {
    return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;
}
