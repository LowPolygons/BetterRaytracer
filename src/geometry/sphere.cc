#include "sphere.hh"
#include "vectors/vector_advanced.hh"
#include "vectors/vector_definitions.hh"
#include "vectors/vector_methods.hh"
#include "vectors/vector_overloads.hh"
#include <cmath>
using Vectors::operator+;
using Vectors::operator-;
using Vectors::Vec;

auto Sphere::check_intersection(Line<3, double> ray) const
    -> IntersectionReturnData {
  // Check the shortest distance between the line and a point
  // Clone then normalise the ray direction vector
  auto ray_normalised = ray;
  auto return_containter = IntersectionReturnData();

  Vectors::normalise(ray_normalised.second);

  auto lambda_dist_pair =
      Vectors::point_to_line_distance(ray_normalised, centre);
  auto dist_mag = Vectors::magnitude(lambda_dist_pair.second);

  if (dist_mag - radius < 1e-07 and lambda_dist_pair.first >= 0.0) {
    // Using pythagoras, you can now get the point of intersection
    // Only possible if using a normalised direction vector

    // Hypotenuse = radius, Side B = lambda_dist_pair.second, Solve for A
    auto lambda_offset = std::sqrt(radius * radius - dist_mag * dist_mag);

    auto p_of_i =
        ray.first + Vectors::scale(ray_normalised.second,
                                   lambda_dist_pair.first - lambda_offset);
    // Normal at the point of intersection
    auto normal_at_poi = p_of_i - centre;
    Vectors::normalise(normal_at_poi);

    return_containter.intersects = true;
    return_containter.point_of_intersection = p_of_i;
    return_containter.colour = colour_properties;
    return_containter.normal = normal_at_poi;
    return_containter.lambda = lambda_dist_pair.first - lambda_offset;

    return return_containter;
  }
  return return_containter;
}
