#include "sphere.hh"
#include "vectors/vector_advanced.hh"
#include "vectors/vector_definitions.hh"
#include "vectors/vector_methods.hh"
#include "vectors/vector_overloads.hh"

using Vectors::operator+;
using Vectors::Vec;

auto Sphere::check_intersection(Line<3, double> ray) const -> bool {
  // Check the shortest distance between the line and a point
  // Clone then normalise the ray direction vector
  auto ray_normalised = ray;

  Vectors::normalise(ray_normalised.second);

  auto lambda_dist_pair =
      Vectors::point_to_line_distance(ray_normalised, centre);
  auto dist_mag = Vectors::magnitude(lambda_dist_pair.second);

  if (dist_mag <= radius) {
    // Using pythagoras, you can now get the point of intersection
    // Only possible if using a normalised direction vector

    // Hypotenuse = radius, Side B = lambda_dist_pair.second, Solve for A
    auto lambda_offset = std::sqrt(radius * radius - dist_mag * dist_mag);

    auto p_of_i =
        ray.first + Vectors::scale(ray_normalised.second,
                                   lambda_dist_pair.first - lambda_offset);

    // TODO: Implement correct return type
    return true;
  }
  return false;
}
