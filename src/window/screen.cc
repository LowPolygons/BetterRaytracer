#include <SDL2/SDL.h>
#include <cstdint>
#include <iostream>
#include <memory>

#include "WindowData.hh"
#include "camera/camera.hh"
#include "screen.hh"

using Window::Screen_SDL;
using Window::Screen_SFML;

auto Screen_SDL::init() -> bool {
  // Initialises the handler windows, surface and graphics handling
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Error Initialising SDL Video, Error: " << SDL_GetError()
              << std::endl;
    return false;
  }

  sdl_window.reset(SDL_CreateWindow(window_data.title.c_str(),        //
                                    window_data.p_x, window_data.p_y, //
                                    window_data.d_x, window_data.d_y, //
                                    window_data.flags                 //
                                    ));

  if (sdl_window.get() == NULL) {
    std::cout << "Error Creating the Window, Error: " << SDL_GetError()
              << std::endl;
    return false;
  }
  // Do an initial window surface update
  SDL_UpdateWindowSurface(sdl_window.get());

  return true;
}

auto Screen_SDL::update(SDL_Event &ev) -> bool {
  while (SDL_PollEvent(&ev) != 0) {
    switch (ev.type) {
    case SDL_QUIT:
      return false;
    }
  }

  return true;
}

auto Screen_SFML::init(std::mt19937 &rand_gen) -> bool {
  screen = std::make_unique<sf::RenderWindow>(
      sf::VideoMode({window_data.d_x, window_data.d_y}), window_data.title);
  screen->display();

  std::cout << "got here lol" << std::endl;
  pixel_map->create(window_data.d_x, window_data.d_y);
  std::cout << "got here lol" << std::endl;
  auto camera = Camera(window_data.d_x, window_data.d_y, 300, 0.0, 0.0, 0.0,
                       {0.0, 0.0, 0.0});
  camera.populate_pixel_directions();
  // Call the raytracer method here
  render(16, camera, 200, 8, rand_gen);

  // Random int for scene name
  auto int_suffix = std::uniform_int_distribution<int>(1, 999999);

  auto output_name = std::string("scenes/OutputScene_" +
                                 std::to_string(int_suffix(rand_gen)) + ".png");
  if (((*pixel_map).copyToImage()).saveToFile(output_name)) {
    std::cout << "Saved to file: " << output_name << std::endl;
  }

  return true;
}

auto Screen_SFML::update(sf::Event &ev) -> bool {
  auto scene = sf::Sprite(*pixel_map);

  screen->clear();
  screen->draw(scene);
  screen->display();

  while (screen->pollEvent(ev)) {
    if (ev.type == sf::Event::Closed) {
      return false;
    }
  }

  return true;
}
