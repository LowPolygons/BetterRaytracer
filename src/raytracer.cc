
#include "raytracer.hh"

#include "camera/camera.hh"
#include "colour/colour.hh"
#include "geometry/sphere.hh"
#include "geometry/triangle.hh"
#include "image/image.hh"
#include "image/scene_objects/scene_objects.hh"
#include "readers/object_config/ColourReader.hh"
#include "readers/object_config/ObjectConfig.hh"
#include "readers/scene_config/ConfigReader.hh"
#include "scene/SceneConfig.hh"
#include "timer/timer.hh"

#include <chrono>
#include <optional>
#include <random>
#include <thread>

// Makes the code a little more pleasant to look at
auto constexpr ONE = std::size_t{1};
auto constexpr MAX = std::size_t{999999};
// Used by the timers to determine if it should display in seconds or minutes
auto constexpr SECONDS_TO_MINUTES_CUTOFF = double{300};

auto Raytracer::set_scene_config() -> bool {
  //==// Read the ini files to set the configuration //==//
  auto config_file_lines = ConfigReader::validate_config("scene_config.ini");
  auto colour_file_lines = ColourReader::validate_config("colour_data.ini");
  auto objects_file_lines =
      ObjectConfigReader::validate_config("object_config.ini");

  auto files_read_successfully = true;

  // Confirm they all exist
  if (!config_file_lines.has_value()) {
    std::cout << "Could not find scene_config.ini" << std::endl;
    files_read_successfully = false;
  }
  if (!colour_file_lines.has_value()) {
    std::cout << "Could not find colour_config.ini" << std::endl;
    files_read_successfully = false;
  }
  if (!objects_file_lines.has_value()) {
    std::cout << "Could not find object_config.ini" << std::endl;
    files_read_successfully = false;
  }

  if (!files_read_successfully)
    return false;

  auto cleaned_up_lines =
      ConfigReader::clean_up_lines(config_file_lines.value());
  auto colours_cleaned =
      ColourReader::clean_up_lines(colour_file_lines.value());
  auto cleaned_objects =
      ObjectConfigReader::clean_up_lines(objects_file_lines.value());

  auto colours = std::unordered_map<std::string, BasicColour>{};

  // Confirm all values were aquired
  if (!ConfigReader::interpret_lines(scene_config, cleaned_up_lines)) {
    std::cout << "Some values were invalid or missing in scene_config.ini"
              << std::endl;
    files_read_successfully = false;
  }
  if (!ColourReader::interpret_lines(colours, colours_cleaned)) {
    std::cout << "Some values were invalid in colour.ini" << std::endl;
    files_read_successfully = false;
  }
  if (!ObjectConfigReader::interpret_lines(scene_config, cleaned_objects,
                                           colours)) {
    std::cout << "Something went wrong interpreting object config" << std::endl;
    files_read_successfully = false;
  }

  if (!files_read_successfully)
    return false;

  return true;
}

auto Raytracer::run_raytracer_app() -> bool {
  //===// Initialise a program wide timer //===//
  auto program_timer_minutes = TimerData::Timer<double, std::chrono::minutes>();
  auto program_timer_seconds = TimerData::Timer<double, std::chrono::seconds>();

  //==// Read in the ini files //==//
  auto config_read_success = set_scene_config();
  if (!config_read_success)
    return false;

  //==// Check the random seed was given //==//
  std::mt19937 rand_gen;

  if (scene_config.SceneSeed) {
    rand_gen = std::mt19937(scene_config.SceneSeed.value());
  } else {
    std::random_device seed;
    rand_gen = std::mt19937(seed());
  }

  //==// If number of threads wasn't specified it uses the maximum //==//
  if (scene_config.NumThreads == 0)
    scene_config.NumThreads = std::thread::hardware_concurrency();

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
      scene_config.ContributionPerBounce);

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

    Image::save_image(output_name, scene_config.Width, scene_config.Height,
                      pixel_buffer);
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
  return true;
}
