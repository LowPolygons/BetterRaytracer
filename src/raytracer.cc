
#include "raytracer.hh"

#include "camera/camera.hh"
#include "colour/colour.hh"
#include "geometry/sphere.hh"
#include "geometry/triangle.hh"
#include "image/image.hh"
#include "scene/SceneConfig.hh"
#include "scene/scene_objects/scene_objects.hh"
#include "timer/timer.hh"

#include <chrono>
#include <optional>
#include <random>

// Makes the code a little more pleasant to look at
auto constexpr ONE = std::size_t{1};
auto constexpr MAX = std::size_t{999999};
// Used by the timers to determine if it should display in seconds or minutes
auto constexpr SECONDS_TO_MINUTES_CUTOFF = double{300};

auto Raytracer::run_raytracer_app() -> RaytracerAppStatus {
  //===// Initialise a program wide timer //===//
  auto program_timer_minutes = TimerData::Timer<double, std::chrono::minutes>();
  auto program_timer_seconds = TimerData::Timer<double, std::chrono::seconds>();

  //==// Check the random seed was given //==//
  std::mt19937 rand_gen;

  if (scene_config.SceneSeed) {
    rand_gen = std::mt19937(scene_config.SceneSeed.value());
  } else {
    std::random_device seed;
    rand_gen = std::mt19937(seed());
  }

  //==// Display the Scene Setup //==//
  scene_config.DisplaySceneSetup();

  //==// Set up the Camera //==//
  auto camera = Camera(
      scene_config.Width, scene_config.AspectRatio, scene_config.FieldOfView,
      scene_config.HorizontalRotation, scene_config.VerticalRotation,
      scene_config.CameraRotation, scene_config.CameraPosition);
  camera.populate_pixel_directions();

  scene_config.Height = camera.get_calculated_height();

  //==// Call the [BLOCKING] Render function //==//
  auto render_timer_minutes = TimerData::Timer<double, std::chrono::minutes>();
  auto render_timer_seconds = TimerData::Timer<double, std::chrono::seconds>();

  auto pixel_buffer = Image::render(
      scene_config.Width, scene_config.Height, scene_config.SceneSetup,
      scene_config.NumThreads, camera, scene_config.NumRays,
      scene_config.NumBounces, rand_gen, scene_config.PrintPercentStatusEvery,
      scene_config.ContributionPerBounce, scene_config.ColourGamma, false);

  render_timer_minutes.stop_clock();
  render_timer_seconds.stop_clock();
  //==// Attempt to save the scene //==//
  auto saver_timer = TimerData::Timer<double, std::chrono::milliseconds>();

  if (scene_config.StoreResultToFile) {
    auto file_id = std::uniform_int_distribution<int>(ONE, MAX);

    auto output_name =
        scene_config.FileName != FILE_NAME_DEFAULT
            ? scene_config.FileName
            : std::string("OutputScene_" + std::to_string(file_id(rand_gen)));
    output_name = std::string(output_name + ".bmp");

    auto image_save_success = Image::save_image(
        output_name, scene_config.Width, scene_config.Height, pixel_buffer);

    if (!image_save_success)
      return RaytracerAppStatus::IMAGE_SAVE_ERROR;
  }

  saver_timer.stop_clock();
  program_timer_minutes.stop_clock();
  program_timer_seconds.stop_clock();

  //===// Log the Timer results //==//
  // clang-format off
  std::cout << std::endl;
  // If the program time more than SECONDS_TO_MINUTES_CUTOFF, print time in mins
  if (render_timer_seconds.get_time_difference() < SECONDS_TO_MINUTES_CUTOFF) {
    TimerData::log_context("Ray Simulations", "s", render_timer_seconds.get_time_difference());
  } else {
    TimerData::log_context("Ray Simulations", "min", render_timer_minutes.get_time_difference());
  }
  TimerData::log_context("Writing BMP File", "ms", saver_timer.get_time_difference());
  // If the program time more than SECONDS_TO_MINUTES_CUTOFF, print time in mins
  if (program_timer_seconds.get_time_difference() < SECONDS_TO_MINUTES_CUTOFF) {
    TimerData::log_context("Program Duration", "s", program_timer_seconds.get_time_difference());
  } else {
    TimerData::log_context("Program Duration", "min", program_timer_minutes.get_time_difference());
  }
  // clang-format on
  return RaytracerAppStatus::SUCCESS;
}
