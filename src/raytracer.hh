#ifndef RAYTRACER_HH
#define RAYTRACER_HH

#include "scene/SceneConfig.hh"

enum RaytracerAppStatus { SUCCESS, CONFIG_READER_ERROR, IMAGE_SAVE_ERROR };

using Scene::SceneConfig;
class Raytracer {
public:
  Raytracer() : scene_config(SceneConfig()) {}
  auto run_raytracer_app() -> RaytracerAppStatus;

private:
  SceneConfig scene_config;
};

#endif
