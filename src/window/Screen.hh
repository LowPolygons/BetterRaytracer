#ifndef SDL_SCREEN_OBJECT_CONTAINER

#define SDL_SCREEN_OBJECT_CONTAINER

#include <SDL2/SDL.h>
#include <cstdint>

#include "SDL_video.h"
#include "WindowData.hh"
#include <memory>

using Window::WindowData;

namespace Window {
// A class which will manage the sdl functions

class Screen {
public:
  Screen(std::string title,                //
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
  WindowData window_data;

  std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> sdl_window;
  std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> sdl_surface;
};

} // namespace Window

#endif
