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

#include "SceneConfig.hh"

using Vectors::PI;
using Window::Screen_SDL;
using Window::Screen_SFML;

auto constexpr ONE = std::size_t{1};
auto constexpr MAX = std::size_t{999999};

/* TODO:
 * - where applicable go through classes and make getters be const &
 * - standardise I/O to be neater
 * -
 * - Make an assets directory which then stores 'object' templates as general
 */

auto main() -> int {
  SceneConfig scene_setup;
  std::mt19937 rand_gen;

  //==// Check the random seed was given //==/
  if (scene_setup.SceneSeed) {
    rand_gen = std::mt19937(scene_setup.SceneSeed.value());
  } else {
    std::random_device seed;
    rand_gen = std::mt19937(seed());
  }

  //==// Create the Scene object and call its Init //==//
  auto sfml_screen = Screen_SFML(scene_setup.WindowTitle, scene_setup.Width,
                                 scene_setup.Height, scene_setup.SceneSetup);
  sfml_screen.init();

  //==// Set up the Camera  //==//
  auto camera =
      Camera(scene_setup.Width, scene_setup.Height, scene_setup.FieldOfView,
             scene_setup.HorizontalRotation, scene_setup.VerticalRotation,
             scene_setup.CameraRotation, scene_setup.CameraPosition);
  camera.populate_pixel_directions();

  //==// Call the [BLOCKING] Render function //==//
  auto image =
      sfml_screen.render(scene_setup.NumThreads, camera, scene_setup.NumRays,
                         scene_setup.NumBounces, rand_gen);

  //==// Prepare a File Name for the created image and save it
  auto file_id = std::uniform_int_distribution<int>(ONE, MAX);
  auto output_name = std::string("scenes/OutputScene_" +
                                 std::to_string(file_id(rand_gen)) + ".png");

  if (image.saveToFile(output_name)) {
    std::cout << "Saved Scene to file: " << output_name << std::endl;
  }

  //==// Code to Allow Runtime viewing of the Simulation //==//
  auto isRunning = true;

  // SDL_Event ev;
  sf::Event event;

  while (isRunning) {
    // isRunning = sdl_screen.update(ev);
    isRunning = sfml_screen.update(event);
  }
}
