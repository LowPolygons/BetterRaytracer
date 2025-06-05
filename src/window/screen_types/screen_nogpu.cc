#include "window/WindowData.hh"
#include "window/screen.hh"
#include <fstream>
#include <ranges>

// Credit to Sean Barrett - https://github.com/nothings/stb
#include "external/stb/stb_image_write.h"

using Window::Screen_NOGPU;

auto Screen_NOGPU::save_image(const std::string &file_name,
                              const std::size_t &width,
                              const std::size_t &height,
                              const std::vector<std::uint8_t> &pixel_buffer)
    -> bool {

  auto constexpr channels = 4;

  if (stbi_write_bmp(file_name.c_str(), static_cast<int>(width),
                     static_cast<int>(height), channels, pixel_buffer.data())) {
    std::cout << "File saved successfully to " << file_name << std::endl;

    return true;
  }
  std::cout << "There was an error trying to save the file" << std::endl;

  return false;
}
