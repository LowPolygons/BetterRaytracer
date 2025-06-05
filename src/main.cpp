#include "window/screen.hh"
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
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

#include <GL/gl.h>

using Window::render;
using Window::Screen_NOGPU;
using Window::Screen_SFML;

auto constexpr ONE = std::size_t{1};
auto constexpr MAX = std::size_t{999999};

/* TODO:
 * - standardise I/O to be neater
 * - Rethink SceneConfig so that you don't have to recompile when changing scene
 * - Post processing effects
 * - Different camera type (rather than pinhole, have all initial vectors be
 * parallel)
 * - Make an alternative option which does not use any rendering library
 */

auto main() -> int {

  SceneConfig scene_setup;
  std::mt19937 rand_gen;

  //==// Check the random seed was given //==//
  if (scene_setup.SceneSeed) {
    rand_gen = std::mt19937(scene_setup.SceneSeed.value());
  } else {
    std::random_device seed;
    rand_gen = std::mt19937(seed());
  }

  //==// Display the Scene Setup //==//
  scene_setup.DisplaySceneSetup();

  //==// Set up the Camera //==//
  auto camera =
      Camera(scene_setup.Width, scene_setup.Height, scene_setup.FieldOfView,
             scene_setup.HorizontalRotation, scene_setup.VerticalRotation,
             scene_setup.CameraRotation, scene_setup.CameraPosition);
  camera.populate_pixel_directions();

  //==// Call the [BLOCKING] Render function //==//
  auto pixel_buffer = Window::render(
      scene_setup.Width, scene_setup.Height, scene_setup.SceneSetup,
      scene_setup.NumThreads, camera, scene_setup.NumRays,
      scene_setup.NumBounces, rand_gen, scene_setup.PrintPercentStatusEvery,
      scene_setup.ContributionPerBounce);

  //==// Create the Screen object and call its Init //==//
  switch (scene_setup.RenderOption) {
  case RenderMode::NOGPU: {
    auto gpuless = Screen_NOGPU(scene_setup.Width, scene_setup.Height,
                                scene_setup.SceneSetup);

    if (scene_setup.StoreResultToFile) {
      auto file_id = std::uniform_int_distribution<int>(ONE, MAX);
      auto output_name = std::string(
          "scenes/OutputScene_" + std::to_string(file_id(rand_gen)) + ".bmp");

      gpuless.save_image(output_name, scene_setup.Width, scene_setup.Height,
                         pixel_buffer);
    }

    break;
  }
  case RenderMode::SFML: {
    /*
    //==// Create the Scene object and call its Init //==//
    auto sfml_screen = Screen_SFML(scene_setup.WindowTitle, scene_setup.Width,
                                   scene_setup.Height, scene_setup.SceneSetup);

    //==// Prepare a File Name for the created image and save it
    if (scene_setup.StoreResultToFile) {
      auto file_id = std::uniform_int_distribution<int>(ONE, MAX);
      auto output_name = std::string(
          "scenes/OutputScene_" + std::to_string(file_id(rand_gen)) + ".png");
      if (sfml_screen.save_image(pixel_buffer, output_name)) {
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
    */
    break;
  }
  }
}
