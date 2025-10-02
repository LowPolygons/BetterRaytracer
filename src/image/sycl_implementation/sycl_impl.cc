#include "colour/colour.hh"
#include "raylogic/raylogic.hh"
#include "shape.hh"
#ifdef SYCL_DEPENDENCY_FOUND

#include "camera/camera.hh"
#include "pod_types/pod_vector.hh"
#include "scene/scene_objects/scene_objects.hh"
#include "sycl_impl.hh"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <format>
#include <hipSYCL/sycl/buffer.hpp>
#include <hipSYCL/sycl/device_selector.hpp>
#include <hipSYCL/sycl/exception.hpp>
#include <hipSYCL/sycl/libkernel/range.hpp>
#include <hipSYCL/sycl/usm.hpp>
#include <print>
#include <ranges>
#include <string>
#include <sycl/sycl.hpp>
#include <vector>

using Colours::BasicColour;
using Colours::ColourData;
using Vectors::operator-;
using Vectors::operator+;

using Vectors::Line;

// LCG RNG
auto SyclImpl::sycl_render(const std::size_t &width, const std::size_t &height,
                           const SceneObjects &objects, Camera &camera,
                           const std::size_t &num_rays,
                           const std::size_t &num_bounces,
                           const float &contribution, const float &colour_gamma,
                           const bool &rasterised_mode_on)
    -> std::expected<std::vector<std::uint8_t>, std::string> {
  try {
    auto queue = sycl::queue{sycl::default_selector()};

    const auto triangles = objects.get_triangles();
    const auto spheres = objects.get_spheres();
    const auto directions = camera.get_pixel_directions_flattened();
    const auto initial_pos = camera.get_pinhole_pos();

    const auto COLOUR_BUFFER_SIZE = width * height * num_rays;
    const auto NUM_TRIANGLES = triangles.size();
    const auto NUM_SPHERES = spheres.size();
    auto directions_buffer =
        sycl::malloc_shared<Vec<3, double>>(directions.size(), queue);
    auto colours_buffer =
        sycl::malloc_shared<BasicColour>(COLOUR_BUFFER_SIZE, queue);
    auto triangles_buffer =
        sycl::malloc_shared<Triangle>(triangles.size(), queue);
    auto spheres_buffer = sycl::malloc_shared<Sphere>(spheres.size(), queue);

    std::copy(directions.begin(), directions.end(), directions_buffer);
    std::copy(triangles.begin(), triangles.end(), triangles_buffer);
    std::copy(spheres.begin(), spheres.end(), spheres_buffer);
    // Initialise to an invalid so the post-sim computation can track
    queue
        .fill(colours_buffer,
              BasicColour{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0},
              COLOUR_BUFFER_SIZE)
        .wait();
    queue
        .parallel_for(
            (width * height),
            [=](sycl::id<1> index) {
              auto inital_direction = directions_buffer[index[0]];
              auto ray = Line<3, double>(initial_pos, inital_direction);

              for (auto ray_count = 0; ray_count < num_rays; ray_count++) {
                auto per_ray_line = Line<3, double>{ray};
                auto ray_colour = ColourData();

                for (auto bounce_count = 0; bounce_count < num_bounces;
                     bounce_count++) {
                  // Acutal logic
                  auto closest_object = IntersectionReturnData();

                  for (auto i = 0; i < NUM_TRIANGLES; i++) {
                    auto obj = triangles_buffer[i];
                    auto return_data = obj.check_intersection(per_ray_line);

                    if (return_data.intersects) {
                      if (return_data.lambda < closest_object.lambda or
                          closest_object.lambda < 0) {
                        closest_object = return_data;
                      }
                    }
                  }

                  for (auto i = 0; i < NUM_SPHERES; i++) {
                    auto obj = spheres_buffer[i];
                    auto return_data = obj.check_intersection(per_ray_line);

                    if (return_data.intersects) {
                      if (return_data.lambda < closest_object.lambda or
                          closest_object.lambda < 0) {
                        closest_object = return_data;
                      }
                    }
                  }

                  if (closest_object.intersects) {
                    per_ray_line = sycl_calculate_new_ray_direction(
                        per_ray_line, closest_object.point_of_intersection,
                        closest_object.normal, closest_object.colour,
                        (index[0] * 3626849) ^ (bounce_count * 2348914) ^
                            (ray_count * 4820131));
                  } else
                    break;

                  ray_colour.combine_colour_as_average(
                      closest_object.colour, bounce_count, contribution);
                }
                colours_buffer[(index[0] * num_rays) + ray_count] =
                    ray_colour.get_total_colour();
              }
            })
        .wait();

    std::vector<std::uint8_t> pixel_buffer(width * height * 4, 0);

    for (auto pixel : std::views::iota(std::size_t{0}, width * height)) {
      auto colour_vector = std::vector<BasicColour>{};

      for (auto i : std::views::iota(std::size_t{0}, num_rays)) {
        colour_vector.push_back(colours_buffer[(num_rays * pixel) + i]);
      }

      auto pixel_colour =
          Colours::get_average_of_colours(colour_vector, colour_gamma);

      auto r_index = rasterised_mode_on ? 0 : 3;
      auto g_index = rasterised_mode_on ? 1 : 4;
      auto b_index = rasterised_mode_on ? 2 : 5;
      pixel_buffer[4 * pixel + 0] =
          static_cast<std::uint8_t>(255.0 * pixel_colour[r_index]);
      pixel_buffer[4 * pixel + 1] =
          static_cast<std::uint8_t>(255.0 * pixel_colour[g_index]);
      pixel_buffer[4 * pixel + 2] =
          static_cast<std::uint8_t>(255.0 * pixel_colour[b_index]);
      pixel_buffer[4 * pixel + 3] = 255;
    }

    return pixel_buffer;

  } catch (const sycl::exception &err) {
    return std::unexpected<std::string>{std::format("{}", err.what())};
  }
}
#endif
