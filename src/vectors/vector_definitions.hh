#ifndef VECTORS_VECTOR_DEFINITION_HPP

#define VECTORS_VECTOR_DEFINITION_HPP

#include <array>
#include <cstddef>

// These were made for two purposes:
// - Intro to templating in C++
// - An easy-to-use 'library' for the vector maths used

namespace Vectors {

template <typename V1, typename V2>
  requires(std::is_arithmetic_v<V1>, std::is_arithmetic_v<V2>)
using JointType = decltype(std::declval<V1>() + std::declval<V2>());

template <std::size_t vector_size, typename Tp> //
  requires std::is_arithmetic_v<Tp>
using Vec = std::array<Tp, vector_size>;

template <std::size_t vector_size, typename Tp> //
  requires std::is_arithmetic_v<Tp>
using Line = std::pair<Vec<vector_size, Tp>, Vec<vector_size, Tp>>;

// Plane is defined by giving three points which the plane contains
template <typename Tp> //
  requires std::is_arithmetic_v<Tp>
using Plane = std::pair<Vec<3, Tp>, Tp>;

const auto PI = 3.14159265358979323;
} // namespace Vectors

#endif
