#ifndef VECTORS_VECTOR_DEFINITION_HPP

#define VECTORS_VECTOR_DEFINITION_HPP

#include <array>
#include <cstddef>

// In this definition of Namespace, I define the Vec type as an alias
namespace Vectors {

template <typename V1, typename V2>
using JointType = decltype(std::declval<V1>() + std::declval<V2>());
template <std::size_t vector_size, typename Tp> //
using Vec = std::array<Tp, vector_size>;

template <std::size_t vector_size, typename Tp>
using Line = std::pair<Vec<vector_size, Tp>, Vec<vector_size, Tp>>;

// Normal + dval or 3 Points based
template <typename Tp> //
using Plane = std::pair<Vec<3, Tp>, Tp>;

const auto PI = 3.14159265358979323;
} // namespace Vectors

#endif
