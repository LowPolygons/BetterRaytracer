#include "SDL_video.h"
#include "window/Screen.hh"
#include <SDL2/SDL.h>
#include <iostream>

#include "geometry/triangle.hh"

#include "camera/camera.hh"

#include "vectors/vector_definitions.hh"

using Vectors::PI;
using Window::Screen;

/* TODO:
 * - Add a class/alias to store all of the object members, consider doing some
 *   overloads
 * - Add a Square class to reduce the misery of defining squares
 * - Add a multithreading manager
 * - make it actually raytrace
 */

//(MAYBE DONT NEED TO STORE IN A MUTEX)
// Get a copy of the pixel map before running. Then, store in a mutex and at the
// end of all of the computation each thread will add their data to the new
// pixel map

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
