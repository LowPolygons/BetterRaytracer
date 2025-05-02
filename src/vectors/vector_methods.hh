#ifndef VECTORS_VECTOR_METHODS_HPP

#define VECTORS_VECTOR_METHODS_HPP

#include "vector_definitions.hh"
#include <cstddef>

namespace Vectors {
// Method for getting the vector distance between two vectors
template <std::size_t Vs, typename V1, typename V2>
auto constexpr vector_distance(Vec<Vs, V1> &vec1, Vec<Vs, V2> &vec2)
    -> Vec<Vs, decltype(std::declval<V1>() + std::declval<V2>())>;

// Method for getting the scalar distance between two vectors
template <std::size_t Vs, typename V1, typename V2>
auto constexpr scalar_distance(Vec<Vs, V1> &vec1, Vec<Vs, V2> &vec2)
    -> decltype(std::declval<V1>() + std::declval<V2>());

//
template <std::size_t Vs, typename V> //
auto constexpr magnitude(Vec<Vs, V> &vec) -> V;

// Method to normalise a vector
template <std::size_t Vs, typename V> //
auto constexpr normalise(Vec<Vs, V> &vec) -> void;

// To get an ABS vector
template <std::size_t Vs, typename V> //
auto constexpr as_abs(Vec<Vs, V> &vec) -> void;

// Dot Product
template <std::size_t Vs, typename V1, typename V2>
auto constexpr dot(Vec<Vs, V1> &vec1, Vec<Vs, V2> &vec2)
    -> decltype(std::declval<V1>() + std::declval<V2>());

// Cross Product
template <typename V1, typename V2>
auto constexpr cross(Vec<3, V1> &vec1, Vec<3, V2> &vec2)
    -> Vec<3, decltype(std::declval<V1>() + std::declval<V2>())>;
} // namespace Vectors

#endif
