#ifdef SDL2_DEPENDENCY_FOUND

#include "rasteriser.hh"
#include "SDL_error.h"
#include "SDL_pixels.h"
#include "image.hh"
#include "raytracer.hh"
#include <SDL2/SDL.h>
#include <expected>
#include <format>
#include <ranges>
#include <utility>

auto constexpr PREVIEW_WIDTH = 800;

using Vectors::operator+;

auto Rasteriser::initialise_sdl_window()
    -> std::expected<SDL_Window *, std::string> {
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    return std::unexpected<std::string>(
        std::format("Error initialising SDL: {}", SDL_GetError()));

  auto window = SDL_CreateWindow(window_data.title.c_str(), window_data.posx,
                                 window_data.posy, window_data.width,
                                 window_data.height, window_data.flags);
  if (!window)
    return std::unexpected<std::string>(
        std::format("Error creating window object: {}", SDL_GetError()));

  return window;
}

auto Rasteriser::pixel_buffer_onto_surface(
    SDL_Surface *sdl_surface, const std::vector<std::uint8_t> &pixel_buffer)
    -> void {
  SDL_LockSurface(sdl_surface);

  auto surface_width = sdl_surface->w;
  auto surface_pixels = static_cast<std::uint32_t *>(sdl_surface->pixels);
  auto surface_padding = sdl_surface->pitch / sizeof(std::uint32_t);

  auto pixel_iterator =
      std::views::iota(std::size_t{0}, pixel_buffer.size() / 4);
  for (auto pixel : pixel_iterator) {
    // (P)ixel (I)ndex O(ffset)
    auto pio = pixel * 4;

    auto pixel_y = pixel / surface_width;
    auto pixel_x = pixel % surface_width;

    auto sdl_pixel = SDL_MapRGB(sdl_surface->format, pixel_buffer[pio + 0],
                                pixel_buffer[pio + 1], pixel_buffer[pio + 2]);

    surface_pixels[(pixel_y * surface_padding) + pixel_x] = sdl_pixel;
  }

  SDL_UnlockSurface(sdl_surface);
}

auto Rasteriser::get_direction_considering_rotation(
    const float &yaw, const float &pitch, const float &roll,
    const SDL_Keycode &key_press) -> std::optional<Vec<3, double>> {

  // Lambda to rotate a vector - assumes the vec is a unit vector
  auto rotate_vector = [&](auto &vec) -> Vec<3, double> {
    // Rotating the directions
    auto rolled_vec = Vec<3, double>{
        vec[0] * std::cos(roll) - vec[1] * std::sin(roll),
        vec[0] * std::sin(roll) + vec[1] * std::cos(roll), vec[2]};
    auto pitched_vec = Vec<3, double>{
        rolled_vec[0],
        rolled_vec[1] * std::cos(pitch) - rolled_vec[2] * std::sin(pitch),
        rolled_vec[1] * std::sin(pitch) + rolled_vec[2] * std::cos(pitch)};
    auto yawed_vec = Vec<3, double>{
        pitched_vec[0] * std::cos(yaw) + pitched_vec[2] * std::sin(yaw),
        pitched_vec[1],
        pitched_vec[2] * std::cos(yaw) - pitched_vec[0] * std::sin(yaw),
    };
    return yawed_vec;
  };

  auto vec_to_rotate = Vec<3, double>{};

  switch (key_press) {
  case SDLK_w:
    vec_to_rotate = {0.0, 0.0, 0.5};
    break;
  case SDLK_s:
    vec_to_rotate = {0.0, 0.0, -0.5};
    break;
  case SDLK_a:
    vec_to_rotate = {-0.5, 0.0, 0.0};
    break;
  case SDLK_d:
    vec_to_rotate = {0.5, 0.0, 0.0};
    break;
  default:
    return std::nullopt;
  }

  auto rotated_vec = rotate_vector(vec_to_rotate);
  return rotated_vec;
}

auto Rasteriser::run_rasteriser_app()
    -> std::expected<const std::pair<bool, SceneConfig>, std::string> {
  //==// If number of threads wasn't specified it uses the maximum //==//
  std::mt19937 rand_gen;

  // For a preview, it has a fixed width
  auto preview_height = PREVIEW_WIDTH / scene_config.AspectRatio;

  // Update window data with newly read data
  window_data.width = PREVIEW_WIDTH;
  window_data.height = preview_height;
  window_data.title = "Scene Preview";

  // Do all the initialising SDL Jargon
  auto maybe_sdl_window = initialise_sdl_window();

  if (!maybe_sdl_window)
    return std::unexpected<std::string>(maybe_sdl_window.error());

  auto sdl_window = maybe_sdl_window.value();
  auto sdl_surface = SDL_GetWindowSurface(sdl_window);

  auto running = true;
  auto pressed_enter = false;
  auto event = SDL_Event{};

  while (running) {
    while (SDL_PollEvent(&event) != 0) {
      switch (event.type) {
      case SDL_QUIT: {
        running = false;
        break;
      };
      case SDL_KEYDOWN: {
        switch (event.key.keysym.sym) {
        case SDLK_w:
          scene_config.CameraPosition = scene_config.CameraPosition +
                                        get_direction_considering_rotation(
                                            scene_config.HorizontalRotation,
                                            scene_config.VerticalRotation,
                                            scene_config.CameraRotation, SDLK_w)
                                            .value();
          break;
        case SDLK_s:
          scene_config.CameraPosition = scene_config.CameraPosition +
                                        get_direction_considering_rotation(
                                            scene_config.HorizontalRotation,
                                            scene_config.VerticalRotation,
                                            scene_config.CameraRotation, SDLK_s)
                                            .value();
          break;
        case SDLK_a:
          scene_config.CameraPosition = scene_config.CameraPosition +
                                        get_direction_considering_rotation(
                                            scene_config.HorizontalRotation,
                                            scene_config.VerticalRotation,
                                            scene_config.CameraRotation, SDLK_a)
                                            .value();
          break;
        case SDLK_d:
          scene_config.CameraPosition = scene_config.CameraPosition +
                                        get_direction_considering_rotation(
                                            scene_config.HorizontalRotation,
                                            scene_config.VerticalRotation,
                                            scene_config.CameraRotation, SDLK_d)
                                            .value();
          break;
        case SDLK_PAGEUP:
          scene_config.CameraPosition[1] -= 0.1;
          break;
        case SDLK_PAGEDOWN:
          scene_config.CameraPosition[1] += 0.1;
          break;
        case SDLK_UP:
          scene_config.VerticalRotation += 0.1;
          break;
        case SDLK_DOWN:
          scene_config.VerticalRotation -= 0.1;
          break;
        case SDLK_LEFT:
          scene_config.HorizontalRotation -= 0.1;
          break;
        case SDLK_RIGHT:
          scene_config.HorizontalRotation += 0.1;
          break;
        case SDLK_LEFTBRACKET:
          scene_config.CameraRotation -= 0.1;
          break;
        case SDLK_RIGHTBRACKET:
          scene_config.CameraRotation += 0.1;
          break;
        case SDLK_RETURN:
          pressed_enter = true;
          running = false;
          break;
        case SDLK_BACKSPACE:
          running = false;
          break;
        }
      };
      }
    }
    auto camera = Camera(
        PREVIEW_WIDTH, scene_config.AspectRatio, scene_config.FieldOfView,
        scene_config.HorizontalRotation, scene_config.VerticalRotation,
        scene_config.CameraRotation, scene_config.CameraPosition);
    camera.populate_pixel_directions();

    auto pixel_buffer = Image::render(
        PREVIEW_WIDTH, preview_height, scene_config.SceneSetup,
        scene_config.NumThreads, camera, 1, 1, rand_gen, 0,
        scene_config.ContributionPerBounce, scene_config.ColourGamma, true);
    pixel_buffer_onto_surface(sdl_surface, pixel_buffer);

    SDL_UpdateWindowSurface(sdl_window);
  }

  SDL_DestroyWindow(sdl_window);
  SDL_Quit();

  return std::make_pair(pressed_enter, scene_config);
}

#endif
