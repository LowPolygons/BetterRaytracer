#include <algorithm>
#include <cstdint>
#include <ranges>
#include <thread>

#include "geometry/shape.hh"

#include "camera/camera.hh"
#include "colour/colour.hh"
#include "raylogic/raylogic.hh"
#include "screen.hh"
#include "vectors/vector_definitions.hh"

using Window::Screen_SFML;

using Vectors::Line;
using Vectors::Vec;

using Colours::BasicColour;
using Colours::ColourData;

// Less ugly than an inline ints
auto constexpr ONE = 1;
auto constexpr FOUR = 4;
auto constexpr ZERO = 0;

// TODO: possibly move to a screen function

// Distribute the number of rows across the threads including indexes
// - Need the indexes as if it is a 1D array for the pixel buffer
// - Need the indexes as if it is a 2D array for accessing the ray dirs
auto PopulateIndexArrays(
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
    auto buf_start = row_start * row_width;
    auto buf_end = (row_end + ONE) * row_width - ONE;

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
      if (accounted_for_rows != t_mod_r)
        accounted_for_rows += ONE;

      // Offset the rest by the number accounted for
      pixel_direcs_indexs[thread_id].second += accounted_for_rows;
      pixel_buffer_indexs[thread_id].first =
          pixel_direcs_indexs[thread_id].first * row_width;
      pixel_buffer_indexs[thread_id].second =
          (pixel_direcs_indexs[thread_id].second + ONE) * row_width - ONE;
    }
  }
}

// TODO: consider implementing std::promise and future to work along side
// threads so that they can finish in any order
auto Screen_SFML::render(std::size_t num_threads, Camera &camera,
                         std::size_t num_rays, std::size_t num_bounces)
    -> void {
  // Every 4 indexes represets a pixels RGBA channels
  std::vector<std::uint8_t> pixel_buffer(window_data.d_x * window_data.d_y *
                                         FOUR);
  // Incase the scene window is tiny
  auto num_threads_needed =
      std::min(num_threads, static_cast<std::size_t>(window_data.d_y));
  auto thread_iterator = std::views::iota(std::size_t{0}, num_threads_needed);

  // Initialise a number of threads
  auto threads = std::vector<std::thread>();

  // Start and end index for each thread in the pixel buffer
  auto pixel_buffer_indexs = std::vector<std::pair<std::size_t, std::size_t>>();
  // Start and end index for each row from camera for a given thread
  auto pixel_direcs_indexs = std::vector<std::pair<std::size_t, std::size_t>>();

  auto indexs_paired = std::make_pair(pixel_direcs_indexs, pixel_buffer_indexs);

  auto row_width = static_cast<std::size_t>(window_data.d_x);
  auto num_rows = static_cast<std::size_t>(window_data.d_y);

  PopulateIndexArrays(num_threads_needed, row_width, num_rows,
                      pixel_direcs_indexs, pixel_buffer_indexs);
  // Anonymous function for each thread to call
  // TODO: Possibly Move this to a screen method possibly so this isnt super
  // cluttered

  auto camera_origin = camera.get_pinhole_pos();

  // Return value is just a bool
  auto render_call = [&](std::size_t thread_id) {
    std::cout << thread_id << ": Starting Ray Tracing" << std::endl;
    // Loop through the row indexes:
    auto row_range = std::views::iota(pixel_direcs_indexs[thread_id].first,
                                      pixel_direcs_indexs[thread_id].second);
    auto num_pixels_done = 0;

    for (auto row : row_range) {
      // - Get the row from the camera
      auto directions_optional = camera.get_row(row);

      if (!directions_optional)
        return false;

      auto directions = directions_optional.value();
      // -  - For each Pixel:
      for (auto pixel_ray_direction : directions) {
        auto num_ray_iterator = std::views::iota(std::size_t{0}, num_rays);
        auto num_bounce_iterator =
            std::views::iota(std::size_t{0}, num_bounces);
        // Vector to contain all the ray colours
        auto colours_for_pixel = std::vector<BasicColour>();
        // -  -  - For each number of rays:
        for (auto ray_num [[maybe_unused]] : num_ray_iterator) {
          // -  -  -  - Form an initial ray Line
          auto ray = Line<3, double>(camera_origin, pixel_ray_direction);
          auto ray_colour = ColourData();

          // -  -  -  - For each number of bounces:
          for (auto bounce_num [[maybe_unused]] : num_bounce_iterator) {
            auto closest_object = IntersectionReturnData();
            // -  -  -  -  - For each object:
            // -  -  -  -  -  - Check if ray intersects object
            // -  -  -  -  -  - If it does, store its impact data
            // TODO: join spheres and triangles to be in the same list
            for (auto obj : objects.get_spheres()) {
              auto return_data = obj.check_intersection(ray);

              if (return_data.intersects) {
                // As rays travel forward, it will never be zero for an actual
                // intersection
                if (return_data.lambda < closest_object.lambda or
                    closest_object.lambda < 0) {
                  closest_object = return_data;
                }
              }
            }
            for (auto obj : objects.get_triangles()) {
              auto return_data = obj.check_intersection(ray);

              if (return_data.intersects) {
                // As rays travel forward, it will never be zero for an actual
                // intersection
                if (return_data.lambda < closest_object.lambda or
                    closest_object.lambda < 0) {
                  closest_object = return_data;
                }
              }
            }
            // -  -  -  -  - Get object closest to ray
            if (closest_object.intersects) {
              //-- It never hits anything anyway, so dont continue doing the
              // rest of
              //-- the bounces
              ray = RayLogic::calculate_new_ray_direction(
                  ray, closest_object.point_of_intersection,
                  closest_object.normal, closest_object.colour);
            } else {
              // TODO: Handle this case of ray never hitting anything
            }
            // -  -  -  -  - Update ray with new direction
            // -  -  -  -  - Store accumulated colour
            ray_colour.combine_colour_as_average(closest_object.colour);
          }
          // -  -  -  - Add colour to pixel total
          colours_for_pixel.push_back(ray_colour.get_total_colour());
        }
        // -  -  - Get the average pixel colour and store in pixel buffer
        auto pixel_colour = Colours::get_average_of_colours(colours_for_pixel);
        auto current_pixel_start_index =
            pixel_buffer_indexs[thread_id].first + (num_pixels_done * 4);

        pixel_buffer[current_pixel_start_index] =
            255 * static_cast<std::uint8_t>(pixel_colour[3]);
        pixel_buffer[current_pixel_start_index + 1] =
            255 * static_cast<std::uint8_t>(pixel_colour[4]);
        pixel_buffer[current_pixel_start_index + 2] =
            255 * static_cast<std::uint8_t>(pixel_colour[5]);
        pixel_buffer[current_pixel_start_index + 3] = 255;

        num_pixels_done++;
      }
    }

    std::cout << thread_id << ": Finished Ray Tracing" << std::endl;

    return true;
  };

  // Create the threads
  for (auto thread_id : thread_iterator) {
    threads.emplace_back(render_call, thread_id);
  }
  // Wait for threads to join
  // - Warning: thread 2 wont be checked until thread 1 is done, etc
  for (auto &thread : threads) {
    thread.join();
  }

  // Convert pixel_buffer into data for the texture

  // Texture -> Image -> Render
  pixel_map->update(pixel_buffer.data());
}
