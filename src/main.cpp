#include "raytracer.hh"
#include <iostream>

auto main() -> int {
  auto raytracer = Raytracer();
  const auto success_status = raytracer.run_raytracer_app();

  switch (success_status) {
  case RaytracerAppStatus::CONFIG_READER_ERROR: {
    std::cerr << "Session terminated due to config reader errors" << std::endl;
    return EXIT_FAILURE;
  }
  case RaytracerAppStatus::IMAGE_SAVE_ERROR: {
    std::cerr << "Session terminated due to image saving errors" << std::endl;
    return EXIT_FAILURE;
  }
  case RaytracerAppStatus::SUCCESS:
    break;
  }
}
