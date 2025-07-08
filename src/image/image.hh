#ifndef SDL_SCREEN_OBJECT_CONTAINER
#define SDL_SCREEN_OBJECT_CONTAINER

#include <cstdint>

#include "../camera/camera.hh"
#include "scene_objects/scene_objects.hh"
#include <random>

namespace Image {

auto PopulateIndexArrays(
    std::size_t &num_threads, std::size_t &row_width, std::size_t &num_rows,
    std::vector<std::pair<std::size_t, std::size_t>> &pixel_direcs_indexs,
    std::vector<std::pair<std::size_t, std::size_t>> &pixel_buffer_indexs)
    -> void;

auto render(const std::size_t &width, const std::size_t &height,
            SceneObjects &objects, const std::size_t &num_threads,
            Camera &camera, const std::size_t &num_rays,
            const std::size_t &num_bounces, std::mt19937 &rand_gen,
            const std::size_t &stat_log_every, const float &contribution,
            const float &colour_gamma) -> std::vector<std::uint8_t>;

auto save_image(const std::string &file_name, const std::size_t &width,
                const std::size_t &height,
                const std::vector<std::uint8_t> &pixel_buffer) -> bool;

} // namespace Image

#endif
