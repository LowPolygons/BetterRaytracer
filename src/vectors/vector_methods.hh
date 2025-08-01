#ifndef VECTORS_VECTOR_METHODS_HPP

#define VECTORS_VECTOR_METHODS_HPP

#include "vector_definitions.hh"
#include "vector_overloads.hh"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <numeric>

namespace Vectors {
// Method for getting the vector distance between two vectors
template <std::size_t Vs, typename V1, typename V2>
  requires(std::is_arithmetic_v<V1>, std::is_arithmetic_v<V2>)
auto constexpr vector_distance(Vec<Vs, V1> &vec1, Vec<Vs, V2> &vec2)
    -> Vec<Vs, decltype(std::declval<V1>() + std::declval<V2>())> {
  return vec2 - vec1;
}

template <std::size_t S, typename T>
  requires std::is_arithmetic_v<T>
auto constexpr areVectorsEqual(const Vec<S, T> &v1, const Vec<S, T> &v2)
    -> bool {
  std::size_t num_matching_coefs = std::transform_reduce(
      v1.begin(), v1.end(), v2.begin(), 0, std::plus{}, [&](auto c1, auto c2) {
        if (std::abs(c1 - c2) < 1e-09) {
          return 1;
        }
        return 0;
      });

  if (num_matching_coefs == S) {
    return true;
  }
  return false;
}

//
template <std::size_t Vs, typename V, typename S> //
  requires(std::is_arithmetic_v<V>, std::is_arithmetic_v<S>)
auto constexpr scale(const Vec<Vs, V> &vec, S scale_factor)
    -> Vec<Vs, decltype(std::declval<V>() + std::declval<S>())> {

  auto new_vec = vec;

  std::transform(vec.begin(), vec.end(), new_vec.begin(),
                 [&](auto coef) { return coef * scale_factor; });

  return new_vec;
}

template <std::size_t Vs, typename V> //
  requires std::is_arithmetic_v<V>
auto constexpr magnitude(const Vec<Vs, V> &vec) -> V {
  return std::sqrt(                     //
      std::transform_reduce(            //
          vec.begin(), vec.end(), V{0}, //
          std::plus<V>{},               //
          [&](auto coef) {              //
            return coef * coef;
          } //
          )); //
}

template <std::size_t Vs, typename V> //
  requires std::is_arithmetic_v<V>
auto constexpr magnitude_squared(const Vec<Vs, V> &vec) -> V {
  return std::transform_reduce(     //
      vec.begin(), vec.end(), V{0}, //
      std::plus<V>{},               //
      [&](auto coef) {              //
        return coef * coef;
      } //
  ); //
}

// Method for getting the scalar distance between two vectors
template <std::size_t Vs, typename V1, typename V2>
  requires(std::is_arithmetic_v<V1>, std::is_arithmetic_v<V2>)
auto constexpr scalar_distance(Vec<Vs, V1> &vec1, Vec<Vs, V2> &vec2)
    -> decltype(std::declval<V1>() + std::declval<V2>()) {

  return magnitude(vector_distance(vec1, vec2)); //
}

// Method to normalise a vector
template <std::size_t Vs, typename V> //
  requires std::is_arithmetic_v<V>
auto constexpr normalise(Vec<Vs, V> &vec) -> void {
  auto mag = magnitude(vec);

  std::transform(vec.begin(), vec.end(), vec.begin(),
                 [&](auto val) { return val / mag; });
}

// To get an ABS vector
template <std::size_t Vs, typename V> //
  requires std::is_arithmetic_v<V>
auto constexpr as_abs(Vec<Vs, V> &vec) -> void {
  std::transform(vec.begin(), vec.end(), vec.begin(),
                 [&](auto val) { return std::abs(val); });
}

// Dot product
template <std::size_t Vs, typename V1, typename V2>
  requires(std::is_arithmetic_v<V1>, std::is_arithmetic_v<V2>)
auto constexpr dot(const Vec<Vs, V1> &vec1, const Vec<Vs, V2> &vec2)
    -> JointType<V1, V2> {
  return std::transform_reduce(vec1.begin(), vec1.end(), vec2.begin(),
                               JointType<V1, V2>{0},
                               std::plus<JointType<V1, V2>>{},
                               [&](auto v1, auto v2) { return v1 * v2; });
}

// Cross Product
template <typename V1, typename V2>
  requires(std::is_arithmetic_v<V1>, std::is_arithmetic_v<V2>)
auto constexpr cross(const Vec<3, V1> &vec1, const Vec<3, V2> &vec2)
    -> Vec<3, decltype(std::declval<V1>() + std::declval<V2>())> {
  return Vec<3, decltype(std::declval<V1>() + std::declval<V2>())>({
      (vec1.at(1) * vec2.at(2) - vec1.at(2) * vec2.at(1)),
      -1 * (vec1.at(0) * vec2.at(2) - vec1.at(2) * vec2.at(0)),
      (vec1.at(0) * vec2.at(1) - vec1.at(1) * vec2.at(0)),
  });
};

} // namespace Vectors

#endif
