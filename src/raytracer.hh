#ifndef RAYTRACER_HH
#define RAYTRACER_HH

#include "scene/SceneConfig.hh"

enum RaytracerAppStatus { SUCCESS, IMAGE_SAVE_ERROR };

using Scene::SceneConfig;
class Raytracer {
public:
  Raytracer(SceneConfig scene_config) : scene_config(scene_config) {}
  auto run_raytracer_app() -> RaytracerAppStatus;

private:
  SceneConfig scene_config;
};

#endif
