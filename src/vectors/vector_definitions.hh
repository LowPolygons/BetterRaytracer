#ifndef VECTORS_VECTOR_DEFINITION_HPP

#define VECTORS_VECTOR_DEFINITION_HPP

#include <array>
#include <cstddef>
#include <variant>

// In this definition of Namespace, I define the Vec type as an alias
namespace Vectors {

template <std::size_t vector_size, typename Tp> //
using Vec = std::array<Tp, vector_size>;

template <std::size_t vector_size, typename Tp>
using Line = std::pair<Vec<vector_size, Tp>, Vec<vector_size, Tp>>;

// Normal + dval or 3 Points based
template <typename Tp>
using Plane =
    std::variant<std::pair<Vec<3, Tp>, Tp>, std::array<Vec<3, Tp>, 3>>;
} // namespace Vectors

#endif
