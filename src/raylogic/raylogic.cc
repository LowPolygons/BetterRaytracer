#include "raylogic.hh"
#include "colour/colour.hh"
#include "vectors/vector_advanced.hh"
#include "vectors/vector_definitions.hh"
#include "vectors/vector_methods.hh"
#include "vectors/vector_overloads.hh"

#include <random>

using Colours::BasicColour;
using Vectors::Line;
using Vectors::operator+;

// TODO: generate a program wide rand gen and device and pass it by ref
auto RayLogic::calculate_new_ray_direction(const Line<3, double> &ray,
                                           const Vec<3, double> &p_of_i,
                                           const Vec<3, double> &normal,
                                           const BasicColour &object_colour)
    -> Line<3, double> {
  // To get the perfectly specular bounce direction, cross the ray direction
  // with the normal, and then cross the ray direction with that result.
  // Lastly, dot it with the normal and if the result is negative then invert
  // each axis
  //
  // First confirm that the normal is the correct direction
  auto normal_clone = normal;
  Vectors::normalise(normal_clone);

  auto ray_normal_dot = Vectors::dot(ray.second, normal_clone);

  if (ray_normal_dot >= 0) {
    normal_clone = Vectors::scale(normal_clone, -1);
  }

  // Perp to the incoming ray and the normal
  auto ray_normal_cross = Vectors::cross(ray.second, normal_clone);

  // Perp to the normal and the previous result - this will lie in the same 2d
  // place as the incoming ray and the normal
  auto horizontal = Vectors::cross(normal_clone, ray_normal_cross);
  // The perfect specular ray
  auto specular_bounce_dir = Vectors::cross(ray.second, ray_normal_cross);

  // Confirm the ray is actually bouncing away from the object
  if (Vectors::dot(specular_bounce_dir, normal_clone) < 0) {
    specular_bounce_dir = Vectors::scale(specular_bounce_dir, -1);
  }

  // The angle between the perfect bounce and the horizontal
  auto acute_spec_horiz =
      Vectors::angle_between_lines<true>(specular_bounce_dir, horizontal);

  auto angle_offset = PI / 2 - acute_spec_horiz;

  // RNG things
  auto obj_col_specularity = object_colour[7];
  std::random_device randomised_seed;
  auto rand_gen = std::mt19937(randomised_seed());
  auto bounce_rand_range = PI * (1 - obj_col_specularity);
  // TODO: look for a cosine weighted distribution

  auto spec_distribution = std::uniform_real_distribution<double>(
      -bounce_rand_range / 2, bounce_rand_range / 2);

  // This is the angle which a vector should be formed from which lies in the
  // same plane as the normal clone, incoming bounce and outgoing bounce
  auto spec_norm_angle = spec_distribution(rand_gen) - angle_offset;
  auto perpendicular_angle = spec_distribution(rand_gen);

  // Normalised versions of the 3 local axis for Spherical coordinates
  Vectors::normalise(horizontal);
  Vectors::normalise(ray_normal_cross);
  Vectors::normalise(specular_bounce_dir);

  auto actual_bounce_dir =                            //
      Vectors::scale(specular_bounce_dir,             //
                     std::cos(perpendicular_angle)) + //
      Vectors::scale(horizontal,                      //
                     std::sin(perpendicular_angle) *  //
                         std::cos(spec_norm_angle)) + //
      Vectors::scale(ray_normal_cross,                //
                     std::sin(perpendicular_angle) *  //
                         std::sin(spec_norm_angle));  //

  Vectors::normalise(actual_bounce_dir);

  return Line<3, double>(
      std::make_pair(ray.first + normal_clone, actual_bounce_dir));
}
