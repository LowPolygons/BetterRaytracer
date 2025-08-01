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
using Vectors::operator-;
using Vectors::PI;

auto constexpr BIAS = 1e-09;
auto constexpr EPSILON = 1e-07;

auto RayLogic::calculate_new_ray_direction(const Line<3, double> &ray,
                                           const Vec<3, double> &p_of_i,
                                           const Vec<3, double> &normal,
                                           const BasicColour &object_colour,
                                           std::mt19937 &rand_gen)
    -> Line<3, double> {
  auto ray_dir = ray.second;

  // Confirm normal is going the right way
  auto normal_clone =
      Vectors::dot(ray_dir, normal) > 0 ? Vectors::scale(normal, -1.0) : normal;
  Vectors::normalise(normal_clone);

  // Generate random direction and specular bounce then lerp between them
  auto normal_distribution = std::uniform_real_distribution<double>(-1.0, 1.0);
  auto diffuse_direction = Vec<3, double>{normal_distribution(rand_gen),
                                          normal_distribution(rand_gen),
                                          normal_distribution(rand_gen)};
  // Flip its direction depending on whether it has an accute angle with the
  // normal
  diffuse_direction = Vectors::dot(diffuse_direction, normal_clone) >= 0
                          ? diffuse_direction
                          : Vectors::scale(diffuse_direction, -1);

  Vectors::normalise(diffuse_direction);

  auto specular_bounce =
      ray_dir -
      Vectors::scale(normal_clone, 2 * Vectors::dot(normal_clone, ray_dir));

  specular_bounce = Vectors::dot(specular_bounce, normal_clone) > EPSILON
                        ? specular_bounce
                        : Vectors::scale(specular_bounce, -1);

  Vectors::normalise(specular_bounce);

  auto specular_diffuse_diff = specular_bounce - diffuse_direction;

  auto interpolated_bounce =
      diffuse_direction +
      Vectors::scale(specular_diffuse_diff, object_colour[7]);
  Vectors::normalise(interpolated_bounce);

  return Line<3, double>{p_of_i + Vectors::scale(normal_clone, BIAS),
                         interpolated_bounce};
}

/*
auto RayLogic::calculate_new_ray_direction(const Line<3, double> &ray,
                                           const Vec<3, double> &p_of_i,
                                           const Vec<3, double> &normal,
                                           const BasicColour &object_colour,
                                           std::mt19937 &rand_gen)
    -> Line<3, double> {
  // To get the perfectly specular bounce direction, cross the ray direction
  // with the normal, and then cross the ray direction with that result.
  // Lastly, dot it with the normal and if the result is negative then invert
  // each axis
  //
  // First confirm that the normal is the correct direction
  auto normal_clone = normal;
  auto ray_clone = ray;

  Vectors::normalise(ray_clone.second);
  Vectors::normalise(normal_clone);

  auto ray_normal_dot = Vectors::dot(ray_clone.second, normal_clone);

  if (ray_normal_dot >= 0) {
    normal_clone = Vectors::scale(normal_clone, -1.0);
  }

  // Perp to the incoming ray and the normal
  auto ray_normal_cross = Vectors::cross(ray_clone.second, normal_clone);

  // Perp to the normal and the previous result - this will lie in the same 2d
  // place as the incoming ray and the normal
  auto horizontal = Vectors::cross(normal_clone, ray_normal_cross);

  // The perfect specular ray
  // - auto specular_bounce_dir = Vectors::cross(ray_clone.second,
  // ray_normal_cross);
  auto specular_bounce_dir =
      ray_clone.second -
      Vectors::scale(normal_clone,
                     2 * Vectors::dot(ray_clone.second, normal_clone));

  // Confirm the ray is actually bouncing away from the object
  if (Vectors::dot(specular_bounce_dir, normal_clone) < 0) {
    specular_bounce_dir = Vectors::scale(specular_bounce_dir, -1);
  }

  // The angle between the perfect bounce and the horizontal
  auto acute_spec_horiz =
      Vectors::angle_between_lines<true>(specular_bounce_dir, horizontal);

  // Offset is necessary because it it the angle range centered around the
  // specular, not the normal
  auto angle_offset = PI / 2 - acute_spec_horiz;

  // RNG things
  auto obj_col_specularity = object_colour[7];
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

  return Line<3, double>{std::make_pair(
      p_of_i + Vectors::scale(normal_clone, BIAS), actual_bounce_dir)};
}
*/
