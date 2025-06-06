#include "window/WindowData.hh"
#include "window/screen.hh"

using Window::Screen_SFML;

auto Screen_SFML::init_window() -> bool {
  screen.setActive(true);
  screen.display();

  return true;
}

auto Screen_SFML::init_texture() -> bool {
  pixel_map.create(window_data.d_x, window_data.d_y);

  return true;
}

auto Screen_SFML::update(sf::Event &ev) -> bool {
  auto scene = sf::Sprite(pixel_map);

  screen.clear();
  screen.draw(scene);
  screen.display();

  while (screen.pollEvent(ev)) {
    if (ev.type == sf::Event::Closed) {
      return false;
    }
  }

  return true;
}
auto Screen_SFML::handle_pixel_data(std::vector<std::uint8_t> pixel_buffer)
    -> void {
  pixel_map.update(pixel_buffer.data());
}

auto Screen_SFML::save_image(const std::string &file_name) -> bool {
  auto image_of_pixel_map = pixel_map.copyToImage();

  if (image_of_pixel_map.saveToFile(file_name)) {
    std::cout << "Successfully saved file to " << file_name << std::endl;

    return true;
  }

  return false;
}
