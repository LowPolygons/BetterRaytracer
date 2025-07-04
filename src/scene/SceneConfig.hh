#ifndef PROGRAM_SCENE_CONFIG_HH
#define PROGRAM_SCENE_CONFIG_HH

#include "image/scene_objects/scene_objects.hh"
#include <cstdint>
#include <optional>
#include <string>

auto constexpr FILE_NAME_DEFAULT = "SCENECONFIG_INDICATES_FILENAME_RANDOMISED";

// Used essentialy as a config file
// Style is different to signify direct user interaction
struct SceneConfig {
  SceneConfig() {}
  // Window Configuration
  std::string WindowTitle;

  // Ray Configuration
  std::size_t NumThreads;
  std::size_t NumRays;
  std::size_t NumBounces;
  float ContributionPerBounce;

  // Camera Configuration
  std::size_t Width;
  std::size_t Height;
  std::size_t FieldOfView;

  float AspectRatio;
  float HorizontalRotation;
  float VerticalRotation;
  float CameraRotation;
  Vec<3, double> CameraPosition;

  // Object Configuration
  SceneObjects SceneSetup;
  std::optional<std::uint32_t> SceneSeed;

  // Stat Info
  std::size_t PrintPercentStatusEvery;

  bool StoreResultToFile;
  std::string FileName = FILE_NAME_DEFAULT;

  bool DisplayResultOnScreen;

  void DisplaySceneSetup();
};
#endif
