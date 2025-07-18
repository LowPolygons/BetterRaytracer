#ifndef RASTERISER_HH
#define RASTERISER_HH

#include "vectors/vector_definitions.hh"
#ifdef SDL2_DEPENDENCY_FOUND

#include "raytracer.hh"
#include <SDL2/SDL.h>
#include <expected>
#include <string>

struct WindowData {
  std::string title;
  int posx = SDL_WINDOWPOS_UNDEFINED; // Could be SDL_WINDOWPOS_CENTERED
  int posy = SDL_WINDOWPOS_UNDEFINED;
  int width;
  int height;
  unsigned int flags = SDL_WINDOW_SHOWN;
};

// TO BE USED AS A PREVIEWER
class Rasteriser {
public:
  Rasteriser(SceneConfig scene_config)
      : scene_config(scene_config), window_data(WindowData()) {}
  auto run_rasteriser_app()
      -> std::expected<const std::pair<bool, SceneConfig>, std::string>;

private:
  auto initialise_sdl_window() -> std::expected<SDL_Window *, std::string>;
  auto pixel_buffer_onto_surface(SDL_Surface *sdl_surface,
                                 const std::vector<std::uint8_t> &pixel_buffer)
      -> void;
  auto get_direction_considering_rotation(const float &yaw, const float &pitch,
                                          const float &tilt,
                                          const SDL_Keycode &key_press)
      -> std::optional<Vec<3, double>>;

private:
  SceneConfig scene_config;
  WindowData window_data;
};

#endif

#endif
