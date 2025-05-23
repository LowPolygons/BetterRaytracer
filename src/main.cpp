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

using Window::Screen_SFML;

auto constexpr ONE = std::size_t{1};
auto constexpr MAX = std::size_t{999999};

/* TODO:
 * - standardise I/O to be neater
 * - Rethink SceneConfig so that you don't have to recompile when changing scene
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

  //==// Display the Scene Setup //==//
  scene_setup.DisplaySceneSetup();

  //==// Set up the Camera  //==//
  auto camera =
      Camera(scene_setup.Width, scene_setup.Height, scene_setup.FieldOfView,
             scene_setup.HorizontalRotation, scene_setup.VerticalRotation,
             scene_setup.CameraRotation, scene_setup.CameraPosition);
  camera.populate_pixel_directions();

  //==// Call the [BLOCKING] Render function //==//
  sfml_screen.init_texture();

  auto image = sfml_screen.render(
      scene_setup.NumThreads, camera, scene_setup.NumRays,
      scene_setup.NumBounces, rand_gen, scene_setup.PrintPercentStatusEvery);

  //==// Prepare a File Name for the created image and save it
  if (scene_setup.StoreResultToFile) {
    auto file_id = std::uniform_int_distribution<int>(ONE, MAX);
    auto output_name = std::string("scenes/OutputScene_" +
                                   std::to_string(file_id(rand_gen)) + ".png");
    if (image.saveToFile(output_name)) {
      std::cout << "Saved Scene to file: " << output_name << std::endl;
    }
  }
  //==// Code to Allow Runtime viewing of the Simulation //==//
  if (scene_setup.DisplayResultOnScreen) {
    sfml_screen.init_window();

    auto isRunning = true;

    sf::Event event;

    while (isRunning) {
      isRunning = sfml_screen.update(event);
    }
  }
}
