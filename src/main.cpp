#include "image/image.hh"

#include <optional>

#include "geometry/triangle.hh"
#include "image/scene_objects/scene_objects.hh"

#include "camera/camera.hh"
#include "geometry/sphere.hh"

#include "colour/colour.hh"

#include <random>

#include "scene/SceneConfig.hh"

#include "main_routines.hh"

#include <thread>

auto constexpr ONE = std::size_t{1};
auto constexpr MAX = std::size_t{999999};

// TODO: in an attempt to clean up redundant code, modify readers classes
auto main() -> int {
  SceneConfig scene_setup;

  //==// Read in the ini files //==//
  MainMethods::set_scene_configuration(scene_setup);

  //==// Check the random seed was given //==//
  std::mt19937 rand_gen;

  if (scene_setup.SceneSeed) {
    rand_gen = std::mt19937(scene_setup.SceneSeed.value());
  } else {
    std::random_device seed;
    rand_gen = std::mt19937(seed());
  }

  //==// If no number of threads were specified, use the maximum able //==//
  if (!scene_setup.NumThreads)
    scene_setup.NumThreads = std::thread::hardware_concurrency();

  //==// Display the Scene Setup //==//
  scene_setup.DisplaySceneSetup();

  //==// Set up the Camera //==//
  auto camera = Camera(scene_setup.Width, scene_setup.AspectRatio,
                       scene_setup.FieldOfView, scene_setup.HorizontalRotation,
                       scene_setup.VerticalRotation, scene_setup.CameraRotation,
                       scene_setup.CameraPosition);
  camera.populate_pixel_directions();

  scene_setup.Height = camera.get_calculated_height();

  //==// Call the [BLOCKING] Render function //==//
  auto pixel_buffer = Image::render(
      scene_setup.Width, scene_setup.Height, scene_setup.SceneSetup,
      scene_setup.NumThreads, camera, scene_setup.NumRays,
      scene_setup.NumBounces, rand_gen, scene_setup.PrintPercentStatusEvery,
      scene_setup.ContributionPerBounce);

  //==// Attempt to save the scene //==//
  if (scene_setup.StoreResultToFile) {
    auto file_id = std::uniform_int_distribution<int>(ONE, MAX);
    auto output_name = std::string("scenes/OutputScene_" +
                                   std::to_string(file_id(rand_gen)) + ".bmp");

    Image::save_image(output_name, scene_setup.Width, scene_setup.Height,
                      pixel_buffer);
  }
}
