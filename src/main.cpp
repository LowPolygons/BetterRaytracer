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

#include "scene/SceneConfig.hh"

#include "readers/object_config/ColourReader.hh"
#include "readers/scene_config/ConfigReader.hh"

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
 */

auto main() -> int {
  SceneConfig scene_setup;

  auto config_file_lines = ConfigReader::validate_config("scene_config.ini");

  if (!config_file_lines.has_value()) {
    std::cout << "Could not find scene_config.ini" << std::endl;
    return 1;
  }

  auto cleaned_up_lines =
      ConfigReader::clean_up_lines(config_file_lines.value());

  if (!ConfigReader::interpret_lines(scene_setup, cleaned_up_lines)) {
    std::cout << "Some values were invalid or missing in scene_config.ini"
              << std::endl;
    return 1;
  }

  auto colour_file_lines = ColourReader::validate_config("colour_data.ini");

  if (!colour_file_lines.has_value()) {
    return 1;
  }

  auto colours_cleaned =
      ColourReader::clean_up_lines(colour_file_lines.value());

  auto colours = std::unordered_map<std::string, BasicColour>{};
  if (!ColourReader::interpret_lines(colours, colours_cleaned)) {
    std::cout << "Some values were invalid in colour.ini" << std::endl;
    return 1;
  }

  for (auto pair : colours) {
    std::cout << pair.first << " " << pair.second[0] << std::endl;
  }
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
    //==// Create the Scene object and call its Init //==//
    auto sfml_screen = Screen_SFML(scene_setup.WindowTitle, scene_setup.Width,
                                   scene_setup.Height, scene_setup.SceneSetup);

    sfml_screen.init_window();
    sfml_screen.init_texture();
    sfml_screen.handle_pixel_data(pixel_buffer);

    //==// Prepare a File Name for the created image and save it
    if (scene_setup.StoreResultToFile) {
      auto file_id = std::uniform_int_distribution<int>(ONE, MAX);
      auto output_name = std::string(
          "scenes/OutputScene_" + std::to_string(file_id(rand_gen)) + ".png");

      auto file_saving_success = sfml_screen.save_image(output_name);

      if (!file_saving_success)
        std::cout << "There was a problem saving the file through SFML"
                  << std::endl;
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
    break;
  }
  }
}
