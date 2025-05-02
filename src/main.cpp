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

  auto b = Vec<3, double>({10.5, 56.1, 122.73});
  auto a = Vec<8, int>({5, 4, 8, 1, 2, 3, 4, 5});
  auto c = a + b;
  auto d = a - b;
  auto e = a * b;
  auto f = a / b;

  for (auto val : c) {
    std::cout << "[" << val << ", " << typeid(val).name() << "], ";
  }
  std::cout << std::endl;

  for (auto val : d) {
    std::cout << "[" << val << ", " << typeid(val).name() << "], ";
  }
  std::cout << std::endl;

  for (auto val : e) {
    std::cout << "[" << val << ", " << typeid(val).name() << "], ";
  }
  std::cout << std::endl;

  for (auto val : f) {
    std::cout << "[" << val << ", " << typeid(val).name() << "], ";
  }
  std::cout << std::endl;

  auto isRunning = true;
  SDL_Event ev;

  while (isRunning) {
    isRunning = sdl_screen.update(ev);
  }
}
