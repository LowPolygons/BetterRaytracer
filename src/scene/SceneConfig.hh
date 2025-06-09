#ifndef PROGRAM_SCENE_CONFIG_HH
#define PROGRAM_SCENE_CONFIG_HH

#include "vectors/vector_definitions.hh"
#include "window/scene_objects/scene_objects.hh"
#include <cstdint>
#include <optional>
#include <string>

// TODO: Get rid of these alias' now

using Constant = std::size_t;
using Offset = Vec<3, double>;
using Title = std::string;
using Precise = float;
// If it isn't defined, it'll use the random_device
using RandomSeed = std::optional<std::uint32_t>;
using Function = void;
using Confirmation = bool;

enum RenderMode { SFML, NOGPU };
// Used essentialy as a config file
// Style is different to signify direct user interaction
struct SceneConfig {
  SceneConfig() { GetSceneSetup(); }
  // Window Configuration
  Title WindowTitle;
  RenderMode RenderOption;

  // Ray Configuration
  Constant NumThreads;
  Constant NumRays;
  Constant NumBounces;
  Precise ContributionPerBounce;

  // Camera Configuration
  Constant Width;
  Constant Height;
  Constant FieldOfView;
  Precise HorizontalRotation;
  Precise VerticalRotation;
  Precise CameraRotation;
  Offset CameraPosition;

  // Object Configuration
  SceneObjects SceneSetup;
  RandomSeed SceneSeed;

  // Stat Info
  Constant PrintPercentStatusEvery;

  Confirmation StoreResultToFile;
  Confirmation DisplayResultOnScreen;

  Function GetSceneSetup();
  Function DisplaySceneSetup();
};
#endif
