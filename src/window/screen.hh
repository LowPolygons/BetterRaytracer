#ifndef SDL_SCREEN_OBJECT_CONTAINER

#define SDL_SCREEN_OBJECT_CONTAINER

#include <SDL2/SDL.h>
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <memory>

#include "../camera/camera.hh"
#include "WindowData.hh"
#include "scene_objects/scene_objects.hh"
#include <SDL2/SDL_video.h>
#include <random>

using Window::WindowData_SDL;
using Window::WindowData_SFML;

/* Container for the various options for Window Rendering
 */

// TODO: quite a bit of redundant data here, clean up
namespace Window {

auto PopulateIndexArrays(
    std::size_t &num_threads, std::size_t &row_width, std::size_t &num_rows,
    std::vector<std::pair<std::size_t, std::size_t>> &pixel_direcs_indexs,
    std::vector<std::pair<std::size_t, std::size_t>> &pixel_buffer_indexs)
    -> void;

auto render(std::size_t width, std::size_t height, SceneObjects objects,
            std::size_t num_threads, Camera &camera, std::size_t num_rays,
            std::size_t num_bounces, std::mt19937 &rand_gen,
            std::size_t stat_log_every, float contribution)
    -> std::vector<std::uint8_t>;

class Screen_SDL {
public:
  Screen_SDL(std::string title,                //
             std::size_t p_x, std::size_t p_y, // Position X and Y
             std::size_t d_x, std::size_t d_y, // Dimensions X and Y
             std::uint32_t flags)              //
      : sdl_window(nullptr, SDL_DestroyWindow),
        sdl_surface(nullptr, SDL_FreeSurface) {
    window_data.title = title;
    window_data.p_x = p_x;
    window_data.p_y = p_y;
    window_data.d_x = d_x;
    window_data.d_y = d_y;
    window_data.flags = flags;
  }

  // Use to initialise unique ptrs stored in class
  auto init() -> bool;

  auto handle_pixel_data(std::vector<std::uint8_t> pixel_buffer) -> void;

  // The raytracer body
  auto update(SDL_Event &ev) -> bool;

private:
  // Container for constants to do with the window itself
  WindowData_SDL window_data;

  std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> sdl_window;
  std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> sdl_surface;
};

class Screen_SFML {
public:
  Screen_SFML(std::string title, std::uint32_t d_x, std::uint32_t d_y,
              SceneObjects _objects)
      : window_data({.title = title, .d_x = d_x, .d_y = d_y}),
        screen(sf::VideoMode({window_data.d_x, window_data.d_y}),
               window_data.title),
        pixel_map(), objects(_objects) {}

  auto init_window() -> bool;
  auto init_texture() -> bool;

  auto handle_pixel_data(std::vector<std::uint8_t> pixel_buffer) -> void;

  auto save_image(std::vector<std::uint8_t> pixel_buffer, std::string file_name)
      -> bool;

  auto update(sf::Event &ev) -> bool;

private:
  WindowData_SFML window_data;

  sf::RenderWindow screen;
  sf::Texture pixel_map;

  SceneObjects objects;
};

class Screen_NOGPU {
public:
  Screen_NOGPU(std::size_t d_x, std::size_t d_y, SceneObjects _objects)
      : width(d_x), height(d_y), objects(_objects) {}

  auto save_image(const std::string &file_name, const std::size_t &width,
                  const std::size_t &height,
                  const std::vector<std::uint8_t> &pixel_buffer) -> bool;

private:
  std::size_t width;
  std::size_t height;

  SceneObjects objects;

  std::vector<std::uint8_t> bmp_data;
  std::vector<std::uint8_t> pixel_data;
};

} // namespace Window

#endif
