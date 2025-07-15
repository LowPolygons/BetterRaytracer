#include "rasteriser.hh"
#include "SDL_error.h"
#include "SDL_pixels.h"
#include "image.hh"
#include "raytracer.hh"
#include <SDL2/SDL.h>
#include <expected>
#include <format>
#include <iostream>
#include <ranges>

auto constexpr PREVIEW_WIDTH = 800;

auto Rasteriser::initialise_sdl_window()
    -> std::expected<SDL_Window *, std::string> {
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    return std::unexpected<std::string>(
        std::format("Error initialising SDL: {}", SDL_GetError()));

  auto window =
      SDL_CreateWindow(std::format("{} Preview", window_data.title).c_str(),
                       window_data.posx, window_data.posy, window_data.width,
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

auto Rasteriser::run_rasteriser_app() -> bool {
  //==// If number of threads wasn't specified it uses the maximum //==//
  std::mt19937 rand_gen;

  // For a preview, it has a fixed width
  scene_config.Width = PREVIEW_WIDTH;
  scene_config.Height = scene_config.Width / scene_config.AspectRatio;

  // Update window data with newly read data
  window_data.width = scene_config.Width;
  window_data.height = scene_config.Height;
  window_data.title = scene_config.FileName;

  // Do all the initialising SDL Jargon
  auto maybe_sdl_window = initialise_sdl_window();

  if (!maybe_sdl_window) {
    std::cerr << maybe_sdl_window.error() << std::endl;
    return false;
  }

  auto sdl_window = maybe_sdl_window.value();
  auto sdl_surface = SDL_GetWindowSurface(sdl_window);

  auto running = true;
  auto event = SDL_Event{};

  while (running) {
    while (SDL_PollEvent(&event) != 0) {
      switch (event.type) {
      case SDL_QUIT: {
        running = false;
        break;
      };
      // TODO: Maybe move this elsewhere, a bit messy. also change it so that
      // WASD moves in the dir of the camera
      // MAKE IT SO THAT IF THEY PRESS ENTER, IT UPDATES THE SCENE CONFIG THAT
      // THE RAYTRACER USES
      case SDL_KEYDOWN: {
        switch (event.key.keysym.sym) {
        case SDLK_w:
          scene_config.CameraPosition[2] += 0.1;
          break;
        case SDLK_s:
          scene_config.CameraPosition[2] -= 0.1;
          break;
        case SDLK_a:
          scene_config.CameraPosition[0] -= 0.1;
          break;
        case SDLK_d:
          scene_config.CameraPosition[0] += 0.1;
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
        }
      };
      }
    }
    auto camera = Camera(
        scene_config.Width, scene_config.AspectRatio, scene_config.FieldOfView,
        scene_config.HorizontalRotation, scene_config.VerticalRotation,
        scene_config.CameraRotation, scene_config.CameraPosition);
    camera.populate_pixel_directions();

    auto pixel_buffer = Image::render(
        scene_config.Width, scene_config.Height, scene_config.SceneSetup,
        scene_config.NumThreads, camera, 1, 1, rand_gen, 101,
        scene_config.ContributionPerBounce, scene_config.ColourGamma, true);
    pixel_buffer_onto_surface(sdl_surface, pixel_buffer);

    SDL_UpdateWindowSurface(sdl_window);
  }

  SDL_DestroyWindow(sdl_window);
  SDL_Quit();

  return true;
}
