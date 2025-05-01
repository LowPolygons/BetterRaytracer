#ifndef WINDOWDATA_FOR_SDL_SCREEN
#define WINDOWDATA_FOR_SDL_SCREEN

#include <SDL2/SDL.h>
#include <cstddef>
#include <cstdint>
#include <iostream>

namespace Window {

struct WindowData {
  std::string title;
  std::size_t p_x = SDL_WINDOWPOS_UNDEFINED; // Pos x, y
  std::size_t p_y = SDL_WINDOWPOS_UNDEFINED;
  std::size_t d_x; // Dimensions x, y
  std::size_t d_y;
  std::uint32_t flags;
};

} // namespace Window

#endif
