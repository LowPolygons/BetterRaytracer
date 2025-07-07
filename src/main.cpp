#include "raytracer.hh"

auto main() -> int {
  auto raytracer = Raytracer();
  const auto success = raytracer.run_raytracer_app();

  if (!success)
    return EXIT_FAILURE;
}
