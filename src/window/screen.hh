#ifndef SDL_SCREEN_OBJECT_CONTAINER

#define SDL_SCREEN_OBJECT_CONTAINER

#include <SDL2/SDL.h>
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <memory>

#include "SDL_video.h"
#include "WindowData.hh"
#include "camera/camera.hh"
#include "scene_objects/scene_objects.hh"
#include <random>

using Window::WindowData_SDL;
using Window::WindowData_SFML;

namespace Window {
// A class which will manage the sdl functions
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

  auto update(sf::Event &ev) -> bool;

  // Multi-threaded method implemented in seperate directory
  auto render(std::size_t num_threads, Camera &camera, std::size_t num_rays,
              std::size_t num_bounces, std::mt19937 &rand_gen,
              std::size_t stat_log_every, float contribution) -> sf::Image;

private:
  WindowData_SFML window_data;

  sf::RenderWindow screen;
  sf::Texture pixel_map;

  SceneObjects objects;
};

class NoGpu_Screen {
public:
  NoGpu_Screen(std::string title, std::size_t d_x, std::size_t d_y,
               SceneObjects _objects) {}

  auto render(std::size_t num_threads, Camera &camera, std::size_t num_rays,
              std::size_t num_bounces, std::mt19937 &rand_gen,
              std::size_t stat_log_every, float contribution) -> void;

private:
  SceneObjects objects;

  std::vector<std::uint8_t> pixel_data;
};

} // namespace Window

#endif
