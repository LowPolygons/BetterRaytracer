#include "bmp.hh"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

auto Writer::little_endian_32_to_8(const std::uint32_t &value)
    -> std::vector<std::uint8_t> {
  auto returnValue = std::vector<std::uint8_t>{};

  returnValue.push_back(static_cast<std::uint8_t>((value) & 0xFF));
  returnValue.push_back(static_cast<std::uint8_t>((value >> 8) & 0xFF));
  returnValue.push_back(static_cast<std::uint8_t>((value >> 16) & 0xFF));
  returnValue.push_back(static_cast<std::uint8_t>((value >> 24) & 0xFF));

  return returnValue;
}
auto Writer::little_endian_16_to_8(const std::uint16_t &value)
    -> std::vector<std::uint8_t> {
  auto returnValue = std::vector<std::uint8_t>{};

  returnValue.push_back(static_cast<std::uint8_t>((value) & 0xFF));
  returnValue.push_back(static_cast<std::uint8_t>((value >> 8) & 0xFF));

  return returnValue;
}

auto Writer::write_bmp(const std::string &file_name,
                       const std::vector<std::uint8_t> &pixel_buffer,
                       const std::size_t &width, const std::size_t &height)
    -> bool {
  auto file_bytes = std::vector<std::uint8_t>{};

  //==// Shorthands for adding 16 and 32 bit numbers to an 8 bit array
  auto add_32_to_array = [&](auto value32) {
    auto vec = little_endian_32_to_8(value32);

    for (auto v : vec) {
      file_bytes.push_back(v);
    }
  };
  auto add_16_to_array = [&](auto value16) {
    auto vec = little_endian_16_to_8(value16);

    for (auto v : vec) {
      file_bytes.push_back(v);
    }
  };

  //==// BM Header //==//
  file_bytes.push_back(0x42);                             // B
  file_bytes.push_back(0x4D);                             // M
  add_32_to_array(std::uint32_t{14} + std::uint32_t{40} + // File size
                  static_cast<std::uint32_t>(pixel_buffer.size()));
  add_32_to_array(std::uint32_t{0}); // The 4 bytes of reserved
  add_32_to_array(std::uint32_t{14} + std::uint32_t{40}); // Data offset

  //==// DIP Header //==//
  add_32_to_array(std::uint32_t{40}); // DIP header size
  add_32_to_array(static_cast<std::uint32_t>(
      static_cast<std::int32_t>(width))); // Image width (pixels)
  add_32_to_array(static_cast<std::uint32_t>(
      static_cast<std::int32_t>(-height))); // Image Height (pixels)
  add_16_to_array(std::uint16_t{1});        // Colour Planes
  add_16_to_array(std::uint16_t{32}); // Bits per Pixel: 3x8 BGR, 1x8 Padding
  add_32_to_array(std::uint32_t{0});  // Compression
  add_32_to_array(std::uint32_t{0});  // Raw bimap data size: No compression: 0
  add_32_to_array(std::uint32_t{0});  // Print resolution horiz
  add_32_to_array(std::uint32_t{0});  // Print resolution vert
  add_32_to_array(std::uint32_t{0});  // Pallete info (num colours)
  add_32_to_array(std::uint32_t{0});  // Pallete info (important colours)

  //==// Pixel data in BGR(P) Format //==//
  auto pixel_iterator = std::views::iota(std::size_t{0}, width * height);

  for (auto pixel : pixel_iterator) {
    // R G B A -> B G R A
    // always (pixel*4) + 0, 1, 2, 3
    file_bytes.push_back(pixel_buffer[(pixel * 4) + 2]);
    file_bytes.push_back(pixel_buffer[(pixel * 4) + 1]);
    file_bytes.push_back(pixel_buffer[(pixel * 4) + 0]);
    file_bytes.push_back(pixel_buffer[(pixel * 4) + 3]);
  }

  //==// Open the file in binary-write and save //==//
  auto file = std::ofstream(file_name, std::ios::binary);
  if (!file)
    return false;

  // Interpret the data in memory as a list of chars, not a cast
  file.write(reinterpret_cast<const char *>(file_bytes.data()),
             file_bytes.size());
  file.close();

  return true;
}
