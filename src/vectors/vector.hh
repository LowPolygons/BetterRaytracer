#ifndef VECTOR_HEADER_DEFINING_OBJECTS

#define VECTOR_HEADER_DEFINING_OBJECTS

#include <SDL2/SDL.h>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>

namespace vectors {

template <std::size_t vector_size, typename Tp> //
struct Vec {
  Vec() : values({0}) {}
  Vec(std::array<Tp, vector_size> vals) : values(vals) {};

  std::array<Tp, vector_size> values;

  auto Normalise() -> void;
};

// declval lets you treat it as an object of type V1 without being a thing

} // namespace vectors

using vectors::Vec;

template <std::size_t vec1_size, std::size_t vec2_size, typename V1,
          typename V2>
auto operator+(const Vec<vec1_size, V1> &a, const Vec<vec2_size, V2> &b)
    -> Vec<(vec1_size >= vec2_size ? vec1_size : vec2_size),
           decltype(std::declval<V1>() + std::declval<V2>())> {

  auto sum = [&](auto v1, auto v2) { return v1 + v2; };

  if (vec1_size > vec2_size) {
    auto _ret =
        Vec<vec1_size, decltype(std::declval<V1>() + std::declval<V2>())>();

    // std::transform(a.begin())
  }

  // return std::transform()
}

#endif
