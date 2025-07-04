#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "bmp_writer/bmp.hh"
#include "image/image.hh"

auto Image::save_image(const std::string &file_name, const std::size_t &width,
                       const std::size_t &height,
                       const std::vector<std::uint8_t> &pixel_buffer) -> bool {

  if (Writer::write_bmp(file_name, pixel_buffer, width, height)) {
    std::cout << "File saved successfully to " << file_name << std::endl;

    return true;
  }
  std::cout << "There was an error trying to save the file" << std::endl;

  return false;
}
