#include <SDL2/SDL.h>
#include <iostream>

#include "SDL_video.h"

#include "vectors/vector_definitions.hh"
#include "vectors/vector_overloads.hh"

#include "window/Screen.hh"
#include <typeinfo>

using Vectors::Vec;
using Vectors::operator+;
using Vectors::operator-;
using Vectors::operator*;
using Vectors::operator/;

using Window::Screen;

auto main() -> int { //
  auto constexpr POS_X = SDL_WINDOWPOS_CENTERED;
  auto constexpr POS_Y = SDL_WINDOWPOS_CENTERED;
  auto constexpr DIM_X = 1000;
  auto constexpr DIM_Y = 1000;
  auto constexpr TITLE = "Window";
  auto constexpr FLAGS = SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN;

  auto sdl_screen = Screen(TITLE, POS_X, POS_Y, DIM_X, DIM_Y, FLAGS);

  sdl_screen.init();

  std::cout << "Window Init Complete" << std::endl;

  auto isRunning = true;
  SDL_Event ev;

  while (isRunning) {
    isRunning = sdl_screen.update(ev);
  }
}
