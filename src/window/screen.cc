#include <SDL2/SDL.h>
#include <cstdint>
#include <iostream>
#include <memory>

#include "WindowData.hh"
#include "camera/camera.hh"
#include "screen.hh"

using Window::Screen_SDL;

auto Screen_SDL::init() -> bool {
  // Initialises the handler windows, surface and graphics handling
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Error Initialising SDL Video, Error: " << SDL_GetError()
              << std::endl;
    return false;
  }

  sdl_window.reset(SDL_CreateWindow(window_data.title.c_str(),        //
                                    window_data.p_x, window_data.p_y, //
                                    window_data.d_x, window_data.d_y, //
                                    window_data.flags                 //
                                    ));

  if (sdl_window.get() == NULL) {
    std::cout << "Error Creating the Window, Error: " << SDL_GetError()
              << std::endl;
    return false;
  }
  // Do an initial window surface update
  SDL_UpdateWindowSurface(sdl_window.get());

  return true;
}

auto Screen_SDL::update(SDL_Event &ev) -> bool {
  while (SDL_PollEvent(&ev) != 0) {
    switch (ev.type) {
    case SDL_QUIT:
      return false;
    }
  }

  return true;
}
