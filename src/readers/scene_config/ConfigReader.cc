#include "ConfigReader.hh"
#include "readers/file_reader.hh"
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

using FileReader::generalised_cast;

auto ConfigReader::validate_config(const std::string &file_path)
    -> std::optional<std::vector<std::string>> {
  return FileReader::read_file_lines(file_path);
}

auto ConfigReader::clean_up_lines(std::vector<std::string> &lines)
    -> std::unordered_map<std::string, std::string> {
  auto constexpr invalid_char = '[';

  // Headers removed
  auto only_desired_lines =
      FileReader::filter_desired_lines(lines, invalid_char);
  // Get the K:V Pairs
  auto key_value_pairs =
      FileReader::split_lines_across_equals(only_desired_lines);

  return key_value_pairs;
}

auto ConfigReader::interpret_lines(
    SceneConfig &scene_config,
    std::unordered_map<std::string, std::string> lines) -> bool {
  // Checks if an item exists in a map
  auto confirm_entity_exists = [&](const auto key) {
    if (lines.find(key) == lines.end())
      return false;

    return true;
  };

  // Screen
  auto exists_Width = confirm_entity_exists("Width");
  auto exists_AspectRatio = confirm_entity_exists("AspectRatio");

  // Simulation
  auto exists_NumThreads = confirm_entity_exists("NumThreads");
  auto exists_NumRays = confirm_entity_exists("NumRays");
  auto exists_NumBounces = confirm_entity_exists("NumBounces");
  auto exists_Contribution = confirm_entity_exists("ContributionPerBounce");

  // Camera
  auto exists_CameraFOV = confirm_entity_exists("FieldOfView");
  auto exists_HorizRot = confirm_entity_exists("HorizontalRotation");
  auto exists_VertRot = confirm_entity_exists("VerticalRotation");
  auto exists_CamRot = confirm_entity_exists("CameraRotation");
  auto exists_CameraX = confirm_entity_exists("CameraOffset_X");
  auto exists_CameraY = confirm_entity_exists("CameraOffset_Y");
  auto exists_CameraZ = confirm_entity_exists("CameraOffset_Z");
  auto exists_ColourGamma = confirm_entity_exists("ColourGamma");

  // Logging
  auto exists_PrintPerc = confirm_entity_exists("PrintPercentStatusEvery");
  auto exists_RandSeed = confirm_entity_exists("RandomSeed");
  // Misc
  auto exists_StoreResult = confirm_entity_exists("StoreResultToFile");
  auto exists_FileName = confirm_entity_exists("FileName");
  auto exists_PreviewEnabled = confirm_entity_exists("PreviewEnabled");

  // They must all exist
  if (exists_Width &&        //
      exists_AspectRatio &&  //
      exists_NumRays &&      //
      exists_NumBounces &&   //
      exists_Contribution && //
      exists_CameraFOV &&    //
      exists_HorizRot &&     //
      exists_VertRot &&      //
      exists_CamRot &&       //
      exists_CameraX &&      //
      exists_CameraY &&      //
      exists_CameraZ &&      //
      exists_PrintPerc &&    //
      exists_StoreResult &&  //
      exists_PreviewEnabled) {
    // Attempt the casting
    auto maybe_width = generalised_cast<int>(lines["Width"]);
    auto maybe_aspect = generalised_cast<double>(lines["AspectRatio"]);
    auto maybe_rays = generalised_cast<int>(lines["NumRays"]);
    auto maybe_bounces = generalised_cast<int>(lines["NumBounces"]);
    auto maybe_contrib =
        generalised_cast<float>(lines["ContributionPerBounce"]);
    auto maybe_camfov = generalised_cast<int>(lines["FieldOfView"]);
    auto maybe_horiz = generalised_cast<double>(lines["HorizontalRotation"]);
    auto maybe_vert = generalised_cast<double>(lines["VerticalRotation"]);
    auto maybe_camrot = generalised_cast<double>(lines["CameraRotation"]);
    auto maybe_camx = generalised_cast<double>(lines["CameraOffset_X"]);
    auto maybe_camy = generalised_cast<double>(lines["CameraOffset_Y"]);
    auto maybe_camz = generalised_cast<double>(lines["CameraOffset_Z"]);
    auto maybe_print = generalised_cast<int>(lines["PrintPercentStatusEvery"]);
    auto maybe_store = generalised_cast<bool>(lines["StoreResultToFile"]);
    auto maybe_preview = generalised_cast<bool>(lines["PreviewEnabled"]);

    // If they all have correctly castable values
    if (                             //
        maybe_width.has_value() &&   //
        maybe_aspect.has_value() &&  //
        maybe_rays.has_value() &&    //
        maybe_bounces.has_value() && //
        maybe_contrib.has_value() && //
        maybe_camfov.has_value() &&  //
        maybe_horiz.has_value() &&   //
        maybe_vert.has_value() &&    //
        maybe_camrot.has_value() &&  //
        maybe_camx.has_value() &&    //
        maybe_camy.has_value() &&    //
        maybe_camz.has_value() &&    //
        maybe_print.has_value() &&   //
        maybe_store.has_value() &&   //
        maybe_preview.has_value()) {
      scene_config.Width = maybe_width.value();
      scene_config.AspectRatio = maybe_aspect.value();
      scene_config.NumRays = maybe_rays.value();
      scene_config.NumBounces = maybe_bounces.value();
      scene_config.ContributionPerBounce = maybe_contrib.value();
      scene_config.FieldOfView = maybe_camfov.value();
      scene_config.HorizontalRotation = maybe_horiz.value();
      scene_config.VerticalRotation = maybe_vert.value();
      scene_config.CameraRotation = maybe_camrot.value();
      scene_config.CameraPosition = {
          maybe_camx.value(), //
          maybe_camy.value(), //
          maybe_camz.value()  //
      };
      scene_config.PrintPercentStatusEvery = maybe_print.value();
      scene_config.StoreResultToFile = maybe_store.value();
      scene_config.PreviewEnabled = maybe_preview.value();

      // Optional
      if (exists_RandSeed) {
        auto maybe_randseed = generalised_cast<int>(lines["RandomSeed"]);

        if (maybe_randseed.has_value())
          scene_config.SceneSeed = maybe_randseed.value();
      }
      if (exists_NumThreads) {
        auto maybe_threads = generalised_cast<int>(lines["NumThreads"]);

        if (maybe_threads.has_value())
          scene_config.NumThreads = maybe_threads.value();
      }
      if (exists_FileName) {
        scene_config.FileName = lines["FileName"];
      }
      if (exists_ColourGamma) {
        auto maybe_colourgamma = generalised_cast<float>(lines["ColourGamma"]);
        if (maybe_colourgamma.has_value())
          scene_config.ColourGamma = maybe_colourgamma.value();
      }

    } else {
      return false;
    }
  } else {
    return false;
  }

  return true;
}
