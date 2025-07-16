#include "raytracer.hh"
#include "scene/SceneConfig.hh"
#include <iostream>

#ifdef SDL2_DEPENDENCY_FOUND
#include "rasteriser.hh"
#include <SDL2/SDL.h>
#endif

auto main() -> int {
  /*
   *
   * Read the scene config in for the program
   *
   */
  auto program_scene_config = SceneConfig();
  auto config_reading_success = Scene::set_scene_config(program_scene_config);

  if (!config_reading_success) {
    std::cerr << "Session terminated due to config reader errors" << std::endl;
    return EXIT_FAILURE;
  }

  /*
   *
   * Run the rasterised version of the program if they chose to preview and if
   * SDL2 was found
   *
   */
#ifdef SDL2_DEPENDENCY_FOUND
  if (program_scene_config.PreviewEnabled) {
    auto rasteriser = Rasteriser(program_scene_config);
    const auto maybe_updated_scene_config = rasteriser.run_rasteriser_app();

    if (!maybe_updated_scene_config) {
      std::cerr << "Sessian terminated due to an error in trying to preview: "
                << maybe_updated_scene_config.error() << std::endl;
      return EXIT_FAILURE;
    }

    // If the user pressed enter on their preview scene, update the scene config
    // to be the one they returned
    if (maybe_updated_scene_config.value().first)
      program_scene_config = maybe_updated_scene_config.value().second;
  }
#endif
  /*
   *
   * Run the actual raytracer
   *
   */
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
