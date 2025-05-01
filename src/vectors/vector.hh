#ifndef VECTOR_HEADER_DEFINING_OBJECTS

#define VECTOR_HEADER_DEFINING_OBJECTS

#include <algorithm>
#include <array>
#include <cstddef>

namespace Vectors {

// The Vec type is an alias
template <std::size_t vector_size, typename Tp> //
using Vec = std::array<Tp, vector_size>;

// The + Overloaded Operator
template <std::size_t vec1_size, std::size_t vec2_size, //
          typename V1, typename V2>
auto operator+(const Vec<vec1_size, V1> &a, //
               const Vec<vec2_size, V2> &b  //
               ) -> Vec<(vec1_size >= vec2_size ? vec1_size : vec2_size),
                        decltype(std::declval<V1>() + std::declval<V2>())> {

  auto sum = [&](auto v1, auto v2) { return v1 + v2; };

  auto _ret = Vec<std::max(vec1_size, vec2_size),
                  decltype(std::declval<V1>() + std::declval<V2>())>();

  // Copy contents of larger to return value, then iterate over smaller one
  if (vec1_size > vec2_size) {
    std::copy_n(a.begin(), a.size(), _ret.begin());
    std::transform(b.begin(), b.end(), a.begin(), _ret.begin(), sum);

  } else {

    std::copy_n(b.begin(), b.size(), _ret.begin());
    std::transform(a.begin(), a.end(), b.begin(), _ret.begin(), sum);
  }
  return _ret;
}

// The - Overloaded Operator
template <std::size_t vec1_size, std::size_t vec2_size, //
          typename V1, typename V2>
auto operator-(const Vec<vec1_size, V1> &a, //
               const Vec<vec2_size, V2> &b  //
               ) -> Vec<(vec1_size >= vec2_size ? vec1_size : vec2_size),
                        decltype(std::declval<V1>() - std::declval<V2>())> {

  auto _ret = Vec<std::max(vec1_size, vec2_size),
                  decltype(std::declval<V1>() - std::declval<V2>())>();

  // Copy contents of larger to return value, then iterate over smaller one
  if (vec1_size > vec2_size) {
    std::copy_n(a.begin(), a.size(), _ret.begin());
    std::transform(b.begin(), b.end(), a.begin(), _ret.begin(),
                   [&](auto v2, auto v1) { return v1 - v2; });

  } else {

    std::copy_n(b.begin(), b.size(), _ret.begin());
    std::transform(a.begin(), a.end(), b.begin(), _ret.begin(),
                   [&](auto v1, auto v2) { return v1 - v2; });
  }
  return _ret;
}

// The * Overloaded Operator
template <std::size_t vec1_size, std::size_t vec2_size, //
          typename V1, typename V2>
auto operator*(const Vec<vec1_size, V1> &a, //
               const Vec<vec2_size, V2> &b  //
               ) -> Vec<(vec1_size >= vec2_size ? vec1_size : vec2_size),
                        decltype(std::declval<V1>() * std::declval<V2>())> {

  auto mul = [&](auto v1, auto v2) { return v1 * v2; };

  auto _ret = Vec<std::max(vec1_size, vec2_size),
                  decltype(std::declval<V1>() * std::declval<V2>())>();

  // Copy contents of larger to return value, then iterate over smaller one
  if (vec1_size > vec2_size) {
    std::copy_n(a.begin(), a.size(), _ret.begin());
    std::transform(b.begin(), b.end(), a.begin(), _ret.begin(), mul);

  } else {

    std::copy_n(b.begin(), b.size(), _ret.begin());
    std::transform(a.begin(), a.end(), b.begin(), _ret.begin(), mul);
  }
  return _ret;
}

// The * Overloaded Operator
template <std::size_t vec1_size, std::size_t vec2_size, //
          typename V1, typename V2>
auto operator/(const Vec<vec1_size, V1> &a, //
               const Vec<vec2_size, V2> &b  //
               ) -> Vec<(vec1_size >= vec2_size ? vec1_size : vec2_size),
                        decltype(std::declval<V1>() / std::declval<V2>())> {

  auto _ret = Vec<std::max(vec1_size, vec2_size),
                  decltype(std::declval<V1>() / std::declval<V2>())>();

  if (vec1_size > vec2_size) {
    std::copy_n(a.begin(), a.size(), _ret.begin());
    std::transform(b.begin(), b.end(), a.begin(), _ret.begin(),
                   [&](auto v2, auto v1) { return v1 / v2; });

  } else {

    std::copy_n(b.begin(), b.size(), _ret.begin());
    std::transform(a.begin(), a.end(), b.begin(), _ret.begin(),
                   [&](auto v1, auto v2) { return v1 / v2; });
  }
  return _ret;
}

} // namespace Vectors

#endif
