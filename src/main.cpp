#include "rasteriser.hh"
#include "raytracer.hh"
#include "scene/SceneConfig.hh"
#include <iostream>

auto main() -> int {
  // Read the scene config in for the program
  SceneConfig program_scene_config;
  auto config_reading_success = Scene::set_scene_config(program_scene_config);

  if (!config_reading_success) {
    std::cerr << "Session terminated due to config reader errors" << std::endl;
    return EXIT_FAILURE;
  }

  // Run the rasterised version of the program if they chose to preview
  auto rasteriser = Rasteriser(program_scene_config);
  const auto preview_status [[maybe_unused]] = rasteriser.run_rasteriser_app();

  if (!preview_status) {
    std::cerr << "Sessian terminated due to an error in trying to preview"
              << std::endl;
    return EXIT_FAILURE;
  }

  // Run the actual raytracer
  auto raytracer = Raytracer(program_scene_config);
  const auto success_status = raytracer.run_raytracer_app();

  switch (success_status) {
  case RaytracerAppStatus::IMAGE_SAVE_ERROR: {
    std::cerr << "Session terminated due to image saving errors" << std::endl;
    return EXIT_FAILURE;
  }
  case RaytracerAppStatus::SUCCESS:
    break;
  }
}
