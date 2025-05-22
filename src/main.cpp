#include "SDL_video.h"
#include "window/screen.hh"
#include <SDL2/SDL.h>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <optional>

#include "geometry/triangle.hh"
#include "window/scene_objects/scene_objects.hh"

#include "camera/camera.hh"
#include "geometry/sphere.hh"

#include "colour/colour.hh"
#include "vectors/vector_definitions.hh"

#include <random>

using Vectors::PI;
using Window::Screen_SDL;
using Window::Screen_SFML;
/* TODO:
 * - where applicable go through classes and make getters be const &
 * - standardise I/O to be neater
 */

//(MAYBE DONT NEED TO STORE IN A MUTEX)
// Get a copy of the pixel map before running. Then, store in a mutex and at the
// end of all of the computation each thread will add their data to the new
// pixel map

auto main() -> int {
  // Parameters for the Screen
  auto constexpr POS_X = SDL_WINDOWPOS_CENTERED;
  auto constexpr POS_Y = SDL_WINDOWPOS_CENTERED;
  auto constexpr DIM_X = 1000;
  auto constexpr DIM_Y = 1000;
  auto constexpr TITLE = "Window";
  auto constexpr FLAGS = SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN;

  // The program wide rand_gen
  std::random_device randomised_seed;
  auto rand_gen = std::mt19937(randomised_seed());

  // auto sdl_screen = Screen(TITLE, POS_X, POS_Y, DIM_X, DIM_Y, FLAGS);
  //
  auto scene_objects = SceneObjects();

  scene_objects.add_sphere(Sphere({-400.0, -700.0, 600.0}, 300.0,
                                  {0.7, 0.7, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0}));

  scene_objects.add_cuboid(
      {-1000.0, -1000.0, -1000.0}, {1000.0, -1000.0, -1000.0},
      {1000.0, -1000.0, 1000.0}, {-1000.0, -1000.0, 1000.0},
      {-1000.0, 1000.0, -1000.0}, {1000.0, 1000.0, -1000.0},
      {1000.0, 1000.0, 1000.0}, {-1000.0, 1000.0, 1000.0},
      {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
      {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
      {0.2, 1.0, 0.8, 0.0, 0.0, 0.0, 0.0, 0.0},
      {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
      {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0, 1.0, 1.0, 0.8, 1.0, 0.0},
      {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
      {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
      {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0},
      {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0},
      {0.0, 0.3, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0},
      {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0});

  auto sfml_screen = Screen_SFML(TITLE, DIM_X, DIM_Y, scene_objects);

  // sdl_screen.init();
  sfml_screen.init(rand_gen);

  auto isRunning = true;

  // SDL_Event ev;
  sf::Event event;

  while (isRunning) {
    // isRunning = sdl_screen.update(ev);
    isRunning = sfml_screen.update(event);
  }
}
