#include "raylogic.hh"
#include "colour/colour.hh"
#include "vectors/vector_advanced.hh"
#include "vectors/vector_definitions.hh"
#include "vectors/vector_methods.hh"

#include <random>

using Colours::BasicColour;
using Vectors::Line;

auto RayLogic::calculate_new_ray_direction(const Line<3, double> &ray,
                                           const Vec<3, double> &p_of_i,
                                           const Vec<3, double> &normal,
                                           const BasicColour &object_colour)
    -> Line<3, double> {
  /*
   To calculate effective directions, the method will do the following:
   - Calculate the direction of the perfectly specular bounce direction
   - Calculate the acute angle between that and the normal
   - Calculate the acute angle between that and the common horizontal between
       the new specular bounce and the normal (this is already calculated when
       getting the new specular bounce), or just pi/2 - previous angle
   - It will then using the specularilty parameter as a % to generate a random
     offset represented by two angles
   - One angle will be the same plane which the angle between the normal and
   the bounce is in
   - The other angle is perpendicular to this
   - The angle range for Angle1 is pi * (1-specularily), which is randomly
       chosen.
   - Once the angle is chosen, it is rotated backwards by the angle between
       the bounce and the normal which will ensure that the bounce is not
       going back into the shape
   - The same thing is done for the perpendicular angle, however there is
       no angle rotation necessary

   - The two vectors are added together and finally added to the bounce
       direction before finally being normalised

  */

  // To get the perfectly specular bounce direction, cross the ray direction
  // with the normal, and then cross the ray direction with that result.
  // Lastly, dot it with the normal and if the result is negative then invert
  // each axis
  //
  // First confirm that the normal is the correct direction
  auto normal_clone = normal;
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
  auto rand_gen = std::mt19937();
  auto bounce_rand_range = PI * (1 - obj_col_specularity);
  // TODO: look for a cosine weighted distribution

  auto spec_distribution = std::uniform_real_distribution<double>(
      -bounce_rand_range / 2, bounce_rand_range / 2);

  // This is the angle which a vector should be formed from which lies in the
  // same plane as the normal clone, incoming bounce and outgoing bounce
  auto spec_norm_angle = spec_distribution(rand_gen) - angle_offset;

  // To actually calculate that vector:
  // - idea one, 2d polar coordinates which you then project into 3d?
  // - idea two, some kind of simultaneous equation involving a Mod(cross) and
  //     dot product
  // - Spherical Polar Coordinates
  // - Form a plane which contains the incoming ray angle, the normal and the
  //     perfect specular bounce
  // - Simultaneous equations of dot product matching what you want, and that
  //     resolves the normal form of the EQ for a plane
  // TODO: FIGURE IT OUT
}
