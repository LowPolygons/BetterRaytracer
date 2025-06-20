#ifndef TIMER_TIMER_HH

#define TIMER_TIMER_HH

#include <chrono>
#include <iostream>

namespace TimerData {

template <typename T>
  requires std::is_arithmetic_v<T>
auto log_context(const std::string &context_name, const std::string &time_unit,
                 T duration) -> void;

template <typename T>
concept TimeUnit = requires(T unit) {
  typename T::rep;
  typename T::period;
  requires std::is_same_v<
      T, std::chrono::duration<typename T::rep, typename T::period>>;
};

template <typename T, TimeUnit tu>
  requires std::is_arithmetic_v<T>
class Timer {
public:
  Timer() { start = std::chrono::steady_clock::now(); };

  auto stop_clock() -> void { end = std::chrono::steady_clock::now(); };
  // Gets the times in little endian order
  auto get_time_difference() -> T;

private:
  std::chrono::time_point<std::chrono::steady_clock> start;
  std::chrono::time_point<std::chrono::steady_clock> end;
};

}; // namespace TimerData

template <typename T, TimerData::TimeUnit tu>
  requires std::is_arithmetic_v<T>
auto TimerData::Timer<T, tu>::get_time_difference() -> T {
  auto difference = std::chrono::duration_cast<tu>(end - start);

  return difference.count();
}

template <typename T>
  requires std::is_arithmetic_v<T>
auto TimerData::log_context(const std::string &context_name,
                            const std::string &time_unit, T duration) -> void {
  std::cout << "System - " << context_name << " - duration: " << duration
            << time_unit << std::endl;
}

#endif
