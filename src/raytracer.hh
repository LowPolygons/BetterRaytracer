#ifndef RAYTRACER_HH
#define RAYTRACER_HH

#include "scene/SceneConfig.hh"

class Raytracer {
public:
  Raytracer() : scene_config(SceneConfig()) {}
  auto run_raytracer_app() -> bool;

private:
  auto set_scene_config() -> bool;

private:
  SceneConfig scene_config;
};

#endif
