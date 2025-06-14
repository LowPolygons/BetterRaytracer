#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Writer {

//==// Converting a 16 and 32 bit number into a set of 8 bits in little endians
auto little_endian_32_to_8(const std::uint32_t &value)
    -> std::vector<std::uint8_t>;
auto little_endian_16_to_8(const std::uint16_t &value)
    -> std::vector<std::uint8_t>;

auto write_bmp(const std::string &file_name,
               const std::vector<std::uint8_t> &pixel_buffer,
               const std::size_t &width, const std::size_t &height) -> bool;
} // namespace Writer
