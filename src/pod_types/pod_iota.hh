#ifndef POD_IOTA_HH

#define POD_IOTA_HH

#include <cstddef>
#include <utility>
namespace pod {

template <std::size_t B, std::size_t E>
  requires(E > B)
class iota {
public:
  iota()
      : start(B), end_ni(E), length(E - B),
        data(std::array<std::size_t, (E - B)>{}) {
    for (auto i = 0; i < (end_ni - start); i++)
      data[i] = i + start;
  }

  constexpr auto begin() -> std::size_t * { return data.data(); }
  constexpr auto end() -> std::size_t * { return data.data() + length; }

private:
  std::size_t start;
  std::size_t end_ni;
  std::size_t length;
  std::array<std::size_t, (E - B)> data;
};
} // namespace pod

#endif
