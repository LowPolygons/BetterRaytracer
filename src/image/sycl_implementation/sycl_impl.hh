#ifndef RAYTRACER_SYCL_IMPLEMENTATION_METHODS
#define RAYTRACER_SYCL_IMPLEMENTATION_METHODS

#include "camera/camera.hh"
#include "scene/scene_objects/scene_objects.hh"
#include <cstddef>
#include <cstdint>
#include <expected>
#include <string>
#include <vector>

auto constexpr BIAS = 1e-09;
auto constexpr EPSILON = 1e-07;

namespace SyclImpl {
// Function which contains the
auto sycl_render(const std::size_t &width, const std::size_t &height,
                 const SceneObjects &objects, Camera &camera,
                 const std::size_t &num_rays, const std::size_t &num_bounces,
                 const float &contribution, const float &colour_gamma,
                 const bool &rasterised_mode_on)
    -> std::expected<std::vector<std::uint8_t>, std::string>;

// Random number between 0 and 1
constexpr auto random(std::uint32_t &seed) -> float;

constexpr auto seed_randomiser(std::uint32_t &input) -> std::uint32_t;

constexpr auto sycl_calculate_new_ray_direction(
    const Line<3, double> &ray, const Vec<3, double> &p_of_i,
    const Vec<3, double> &normal, const BasicColour &object_colour,
    const std::size_t &seed) -> Line<3, double>;

} // namespace SyclImpl

constexpr auto SyclImpl::random(std::uint32_t &seed_hash) -> float {
  return static_cast<float>(seed_randomiser(seed_hash)) / 4294967295.0;
}

constexpr auto SyclImpl::seed_randomiser(std::uint32_t &input)
    -> std::uint32_t {
  auto val = static_cast<std::uint32_t>(input);
  val ^= val << 13;
  val ^= val >> 17;
  val ^= val << 5;
  input = val;
  return val;
}

constexpr auto SyclImpl::sycl_calculate_new_ray_direction(
    const Line<3, double> &ray, const Vec<3, double> &p_of_i,
    const Vec<3, double> &normal, const BasicColour &object_colour,
    const std::size_t &seed) -> Line<3, double> {
  auto ray_dir = ray.second;

  // Confirm normal is going the right way
  auto normal_clone =
      Vectors::dot(ray_dir, normal) > 0 ? Vectors::scale(normal, -1.0) : normal;
  Vectors::normalise(normal_clone);

  auto rand_state = static_cast<std::uint32_t>(seed);
  // Generate random direction and specular bounce then lerp between them
  auto diffuse_direction =
      Vec<3, double>{2.0 * SyclImpl::random(rand_state) - 1.0,
                     2.0 * SyclImpl::random(rand_state) - 1.0,
                     2.0 * SyclImpl::random(rand_state) - 1.0};

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
};

#endif
