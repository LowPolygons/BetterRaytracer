#include <algorithm>
#include <cstdint>
#include <iostream>
#include <ranges>
#include <thread>

#include "geometry/shape.hh"

#include "camera/camera.hh"
#include "colour/colour.hh"
#include "raylogic/raylogic.hh"
#include "vectors/vector_definitions.hh"

#include "image/image.hh"

#include <random>

using Vectors::Line;
using Vectors::Vec;

using Colours::BasicColour;
using Colours::ColourData;

// Less ugly than an inline ints
auto constexpr ONE = 1;
auto constexpr FOUR = 4;
auto constexpr ZERO = 0;

// Distribute the number of rows across the threads including indexes
// - Need the indexes as if it is a 1D array for the pixel buffer
// - Need the indexes as if it is a 2D array for accessing the ray dirs
auto Image::PopulateIndexArrays(
    std::size_t &num_threads, std::size_t &row_width, std::size_t &num_rows,
    std::vector<std::pair<std::size_t, std::size_t>> &pixel_direcs_indexs,
    std::vector<std::pair<std::size_t, std::size_t>> &pixel_buffer_indexs)
    -> void {

  auto thread_iterator = std::views::iota(std::size_t{ZERO}, num_threads);
  // Number of excess rows
  auto t_mod_r = num_rows % num_threads;
  // Base num of rows per thread
  auto t_div_r = (num_rows - t_mod_r) / num_threads;

  // First, calculate the indexes as if it was a whole divsion
  for (auto thread_id : thread_iterator) {
    auto row_start = thread_id * t_div_r;
    auto row_end = (thread_id + ONE) * t_div_r - ONE;
    auto buf_start = row_start * row_width * FOUR;
    auto buf_end = (row_end + ONE) * row_width * FOUR - ONE;

    pixel_direcs_indexs.emplace_back(row_start, row_end);
    pixel_buffer_indexs.emplace_back(buf_start, buf_end);
  }
  // If the mod was not zero, add one row to as many threads as needed
  if (t_mod_r != ZERO) {
    auto accounted_for_rows = ZERO;

    for (auto thread_id : thread_iterator) {
      // Add however many have already been accounted for to start
      pixel_direcs_indexs[thread_id].first += accounted_for_rows;

      // If we need to account for another, increment
      if (static_cast<unsigned long>(accounted_for_rows) !=
          static_cast<unsigned long>(t_mod_r))
        accounted_for_rows += ONE;

      // Offset the rest by the number accounted for
      pixel_direcs_indexs[thread_id].second += accounted_for_rows;
      pixel_buffer_indexs[thread_id].first =
          pixel_direcs_indexs[thread_id].first * row_width * FOUR;
      pixel_buffer_indexs[thread_id].second =
          (pixel_direcs_indexs[thread_id].second + ONE) * row_width * FOUR -
          ONE;
    }
  }
}

auto Image::render(const std::size_t &width, const std::size_t &height,
                   const SceneObjects &objects, const std::size_t &num_threads,
                   Camera &camera, const std::size_t &num_rays,
                   const std::size_t &num_bounces, std::mt19937 &rand_gen,
                   const std::size_t &stat_log_every, const float &contribution,
                   const float &colour_gamma, const bool &rasterised_mode_on)
    -> std::vector<std::uint8_t> {
  // Every 4 indexes represets a pixels RGBA channels
  std::vector<std::uint8_t> pixel_buffer(width * height * FOUR);
  // Incase the scene window is tiny
  auto num_threads_needed =
      std::min(num_threads, static_cast<std::size_t>(height));
  auto thread_iterator = std::views::iota(std::size_t{0}, num_threads_needed);

  // Initialise a number of threads
  auto threads = std::vector<std::thread>();

  // Start and end index for each thread in the pixel buffer
  auto pixel_buffer_indexs = std::vector<std::pair<std::size_t, std::size_t>>();
  // Start and end index for each row from camera for a given thread
  auto pixel_direcs_indexs = std::vector<std::pair<std::size_t, std::size_t>>();
  auto indexs_paired = std::make_pair(pixel_direcs_indexs, pixel_buffer_indexs);

  auto row_width = static_cast<std::size_t>(width);
  auto num_rows = static_cast<std::size_t>(height);

  PopulateIndexArrays(num_threads_needed, row_width, num_rows,
                      pixel_direcs_indexs, pixel_buffer_indexs);

  auto camera_origin = camera.get_pinhole_pos();

  // Lambda called by eadh thread
  auto render_call = [&](std::size_t thread_id) {
    // Get the rows the current thread controls
    auto row_range =
        std::views::iota(pixel_direcs_indexs[thread_id].first,
                         pixel_direcs_indexs[thread_id].second + ONE);
    auto num_pixels_done = 0;
    auto lowest_passed = 0;
    // Iterators used on a per-pixel basis
    auto num_ray_iterator = std::views::iota(std::size_t{0}, num_rays);
    auto num_bounce_iterator = std::views::iota(std::size_t{0}, num_bounces);

    // For each row the thread it was assigned
    for (auto row : row_range) {
      // Logging information - based off thread zero not an average
      if (!rasterised_mode_on) {
        auto perc_done = 100.0 * row / pixel_direcs_indexs[thread_id].second;
        if (thread_id == 0 and static_cast<int>(perc_done) > lowest_passed) {
          lowest_passed = lowest_passed + stat_log_every;
          std::cout << ">>> Approximate Completion: " << perc_done << "%"
                    << std::endl;
        }
      };
      // Get the direction vectors for the rows threads
      auto directions_optional = camera.get_row(row);
      if (!directions_optional)
        return false;
      auto directions = directions_optional.value();

      // Loop over the directions (pixels) on the row
      for (auto pixel_ray_direction : directions) {
        auto initial_ray_hits_nothing = false;
        auto colours_for_pixel = std::vector<BasicColour>();
        for (auto ray_num [[maybe_unused]] : num_ray_iterator) {
          if (initial_ray_hits_nothing)
            break; // If the initial ray hits nothing, none of them will so skip

          auto ray = Line<3, double>(camera_origin, pixel_ray_direction);
          auto ray_colour = ColourData();

          Vectors::normalise(ray.second);

          for (auto bounce_num [[maybe_unused]] : num_bounce_iterator) {
            // Container for the hit data
            auto closest_object = IntersectionReturnData();

            for (auto &obj : objects.get_shapes()) {
              auto return_data = obj->check_intersection(ray);
              // If intersects, make sure it is closer than any stored object
              if (return_data.intersects) {
                if (return_data.lambda < closest_object.lambda or
                    closest_object.lambda < 0) {
                  closest_object = return_data;
                }
              }
            }
            // If it hit nothing then you can skip this entire pixel
            if (closest_object.intersects) {
              ray = RayLogic::calculate_new_ray_direction(
                  ray, closest_object.point_of_intersection,
                  closest_object.normal, closest_object.colour, rand_gen);
            } else {
              if (bounce_num == 0)
                initial_ray_hits_nothing = true;
              break;
            }
            // Add the colour from the curren rays journey to the pixels data
            ray_colour.combine_colour_as_average(closest_object.colour,
                                                 bounce_num, contribution);
          }
          colours_for_pixel.push_back(ray_colour.get_total_colour());
        }
        // Get the averaged colour and add the elements to the pixel buffer
        auto pixel_colour =
            Colours::get_average_of_colours(colours_for_pixel, colour_gamma);
        auto current_pixel_start_index =
            pixel_buffer_indexs[thread_id].first + (num_pixels_done * 4);

        auto r_index = rasterised_mode_on ? 0 : 3;
        auto g_index = rasterised_mode_on ? 1 : 4;
        auto b_index = rasterised_mode_on ? 2 : 5;
        pixel_buffer[current_pixel_start_index] =
            static_cast<std::uint8_t>(255.0 * pixel_colour[r_index]);
        pixel_buffer[current_pixel_start_index + 1] =
            static_cast<std::uint8_t>(255.0 * pixel_colour[g_index]);
        pixel_buffer[current_pixel_start_index + 2] =
            static_cast<std::uint8_t>(255.0 * pixel_colour[b_index]);
        pixel_buffer[current_pixel_start_index + 3] = 255;

        num_pixels_done++;
      }
    }
    return true;
  };

  if (!rasterised_mode_on)
    std::cout << ">>> Beginning Computation" << std::endl;

  // Create the threads
  for (auto thread_id : thread_iterator) {
    threads.emplace_back(render_call, thread_id);
  }
  // Wait for threads to join
  for (auto &thread : threads) {
    thread.join();
  }
  if (!rasterised_mode_on)
    std::cout << ">>> Computation Finished" << std::endl;

  return pixel_buffer;
}
