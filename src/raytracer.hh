#ifndef RAYTRACER_HH
#define RAYTRACER_HH

#include "scene/SceneConfig.hh"

using Scene::SceneConfig;
class Raytracer {
public:
  Raytracer() : scene_config(SceneConfig()) {}
  auto run_raytracer_app() -> bool;

private:
  SceneConfig scene_config;
};

#endif
