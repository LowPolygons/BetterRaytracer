#ifndef POD_VECTOR_HH

#define POD_VECTOR_HH

#include <array>
#include <cstddef>
#include <utility>

namespace pod {

template <std::size_t S, typename T> //
class vector {
public:
  constexpr vector() : count(0), max(S), local_data(std::array<T, S>{}) {};
  constexpr auto push_back(T data) -> bool;
  constexpr auto size() const -> const std::size_t &;
  constexpr auto operator[](std::size_t i) -> T &;
  constexpr auto begin() -> T * { return local_data.data(); };
  constexpr auto end() -> T * { return local_data.data() + count; };

private:
  std::size_t count;
  std::size_t max;
  std::array<T, S> local_data;
};

template <std::size_t S, typename T>
constexpr auto vector<S, T>::push_back(T data) -> bool {
  if (count >= max)
    return false;

  local_data[count] = data;

  count++;

  return true;
}

template <std::size_t S, typename T>
constexpr auto vector<S, T>::size() const -> const std::size_t & {
  return count;
}

template <std::size_t S, typename T>
constexpr auto vector<S, T>::operator[](std::size_t i) -> T & {
  return local_data[i];
}

}; // namespace pod

#endif
