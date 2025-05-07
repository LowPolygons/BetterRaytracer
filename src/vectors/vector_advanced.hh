#ifndef VECTORS_VECTOR_ADVANCED_HH

#define VECTORS_VECTOR_ADVANCED_HH

#include "vector_definitions.hh"
#include "vector_methods.hh"

#include <cmath>
#include <cstddef>
#include <optional>
#include <utility>

const auto PI = 3.14159265358979323;

namespace Vectors {

// Returns the point of intersection and the lambda value, or nothing
template <typename V, typename P>
auto constexpr line_intersects_plane(Line<3, V> &line, Plane<P> &plane)
    -> std::optional<std::pair<JointType<V, P>, Vec<3, JointType<V, P>>>> {
  // Derivation comes from solving (a + lambda[b]) dot n = d for lambda

  auto descriminant = Vectors::dot(plane.first, line.second);

  // If determinant is zero then the line is likely parallel
  if (std::abs(descriminant) < 1e-09)
    return std::nullopt;

  auto lambda =
      (plane.second - Vectors::dot(plane.first, line.first)) / descriminant;

  return std::make_pair(lambda,
                        line.first + Vectors::scale(line.second, lambda));
}
// Does a given point lie on a plane of any type
template <typename V, typename P>
auto is_point_on_plane(Vec<3, V> &point, Plane<P> &plane) -> bool {
  // Form a new line with point (inp) and direction (plane normal) and solve for
  // lambda on line intersects plane. If lambda < a certain value then consider
  // it on the plane

  auto line = Line<3, JointType<V, P>>(std::make_pair(point, plane.first));
  auto result = line_intersects_plane(line, plane);

  // This shouldn't happen, but just in case
  if (!result)
    return false;

  // If less than a certain value for precision errors, its on the plane
  if (std::abs(result.value().first) < 1e-09)
    return true;

  return false;
}

// Returns the shortest distance between a line and a point
template <std::size_t Vs, typename V1, typename V2>
auto constexpr point_to_line_distance(Line<Vs, V1> &line, Vec<Vs, V2> &point)
    -> Vec<Vs, JointType<V1, V2>> {
  // Derivation can be completed by having a generic point, and then creating a
  // vector from that point to any point on the line. Then, dotting that generic
  // point + some unknown amount of the direction vector with teh direction
  // vector and solving equation to zero

  auto determinant = std::pow(Vectors::magnitude(line.second), 2);

  if (determinant < 1e-09)
    return {V1{0}, V1{0}, V1{0}};

  auto lambda =
      line.second.at(0) * (point.at(0) - line.first.at(0)) +
      line.second.at(1) * (point.at(1) - line.first.at(1)) +
      line.second.at(2) * (point.at(2) - line.first.at(2)) / determinant;

  return (line.first - point) + Vectors::scale(line.second, lambda);
}

// Returns the acute or obstuse angle between a line and a plane, or nothing
template <typename V, typename P>
auto constexpr angle_between_line_and_plane(Line<3, V> &line, Plane<P> &plane)
    -> std::optional<JointType<V, P>> {
  // Dot the lines direction vector with the normal of the plane, then get angle
  // Following this, two cases:
  // - Case 1: Angle is obtuse, do angle - 90
  // - Cast 2: angle is acute, do 90 - angle

  auto dot_product = Vectors::dot(line.second, plane.first);

  // If it is perpendicular with the normal it is parallel and no angle
  if (std::abs(dot_product) < 1e-09) {
    return std::nullopt;
  }

  auto cos_angle = dot_product / (Vectors::magnitude(line.second) *
                                  Vectors::magnitude(plane.first));

  auto final = std::acos(cos_angle);

  if (final >= PI / 2) {
    return final - PI / 2;
  } else {
    return PI / 2 - final;
  }
}

// Reflects a point across a plane
template <typename V, typename P>
auto constexpr reflect_point_across_plane(Vec<3, V> &point, Plane<P> &plane)
    -> Vec<3, JointType<V, P>> {
  // Dervation comes from creating a line from the point to the plane, finding
  // the P-O-I and then doubling its lambda value

  auto normal_magnitude = Vectors::magnitude(plane.first);

  if (std::abs(normal_magnitude) < 1e-09)
    return point;

  auto lambda = (plane.second - point.at(0) - point.at(1) - point.at(2)) /
                std::pow(normal_magnitude, 2);

  return (point + Vectors::scale(plane.first, 2 * lambda));
}

} // namespace Vectors

#endif
