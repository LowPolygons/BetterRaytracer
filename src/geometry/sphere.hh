#ifndef GEOMETRY_SHAPES_SPHERE_HH

#define GEOMETRY_SHAPES_SPHERE_HH

#include "colour/colour.hh"
#include "shape.hh"
#include "vector_advanced.hh"
#include "vector_methods.hh"
#include "vector_overloads.hh"
#include "vectors/vector_definitions.hh"

using Colours::BasicColour;
using Vectors::Vec;

class Sphere {
public:
  Sphere(Vec<3, double> ctr, //
         double rds,         //
         BasicColour clr     //
  ) {
    centre = ctr;
    radius = rds;
    colour_properties = clr;
  };

  constexpr auto check_intersection(const Line<3, double> &ray) const
      -> IntersectionReturnData;

private:
  Vec<3, double> centre;
  double radius;
  // Colour: R, G, B, Emission: R, G, B, Strength, Specular: %
  // Index:  0, 1, 2,           3, 4, 5         6,           7
  BasicColour colour_properties;
};

using Vectors::operator+;
using Vectors::operator-;
using Vectors::Vec;

constexpr auto Sphere::check_intersection(const Line<3, double> &ray) const
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

#endif
