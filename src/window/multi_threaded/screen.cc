#include "screen.hh"
#include <algorithm>
#include <cstdint>
#include <ranges>
#include <thread>

using Window::Screen_SFML;

// Less ugly than an inline 4
auto constexpr FOUR = 4;

// TODO: consider implementing std::promise and future to work along side
// threads so that they can finish in any order
auto Screen_SFML::render(std::size_t num_threads) -> void {
  // Every 4 indexes represets a pixels RGBA channels
  std::vector<std::uint8_t> pixel_buffer(window_data.d_x * window_data.d_y *
                                         FOUR);

  // Incase the scene window is tiny
  auto num_threads_needed =
      std::min(num_threads, static_cast<std::size_t>(window_data.d_y));

  auto thread_iterator = std::views::iota(std::size_t{0}, num_threads_needed);

  // Initialise a number of threads
  std::vector<std::thread> threads;
  // Passed into the thread, lets them know where to access and where to place
  std::vector<std::pair<std::size_t, std::size_t>> start_end_indexes;

  // Anonymous function for each thread to call
  auto render_call = [&](std::size_t thread_id) {
    // First, it needs to know which rows it will use
  };

  // Create the threads
  for (auto thread_id : thread_iterator) {
    threads.emplace_back(render_call, thread_id);
  }
  // Wait for threads to join
  // - Warning: thread 2 wont be checked until thread 1 is done, etc
  for (auto &thread : threads) {
    thread.join();
  }
  // Convert pixel_buffer into data for the texture
  // Texture -> Image -> Render
}
