#ifndef VECTORS_VECTOR_OVERLOADS_HPP

#define VECTORS_VECTOR_OVERLOADS_HPP

#include <algorithm>
#include <cstddef>

#include "vector_definitions.hh"

// A function used to neaten the overloading functions visually
template <typename Rt, typename A, typename B, typename Anon>
auto perform_overloaded_op(A a, B b, Rt &rt, Anon fnc) -> void {
  std::copy_n(a.begin(), a.size(), rt.begin());
  std::transform(b.begin(), b.end(), a.begin(), rt.begin(), fnc);
}

// In this definition of Namespace, I define the
namespace Vectors {

// An alias for the return types of all the overloaded operators
template <std::size_t v1s, //
          std::size_t v2s, //
          typename V1,     //
          typename V2>     //
using OverloadReturnType =
    Vec<(v1s >= v2s ? v1s : v2s),
        decltype(std::declval<V1>() + std::declval<V2>())>;

// The + Overloaded Operator
template <std::size_t v1s, std::size_t v2s, typename V1, typename V2>
auto operator+(const Vec<v1s, V1> &a, const Vec<v2s, V2> &b)
    -> OverloadReturnType<v1s, v2s, V1, V2> {

  auto sum = [&](auto v1, auto v2) { return v1 + v2; };

  auto _ret = OverloadReturnType<v1s, v2s, V1, V2>();

  if (v1s > v2s) {
    perform_overloaded_op(a, b, _ret, sum);
  } else {
    perform_overloaded_op(b, a, _ret, sum);
  }
  return _ret;
}

// The - Overloaded Operator
template <std::size_t v1s, std::size_t v2s, typename V1, typename V2>
auto operator-(const Vec<v1s, V1> &a, const Vec<v2s, V2> &b)
    -> OverloadReturnType<v1s, v2s, V1, V2> {

  auto _ret = OverloadReturnType<v1s, v2s, V1, V2>();

  if (v1s > v2s) {
    perform_overloaded_op(a, b, _ret,
                          [&](auto v2, auto v1) { return v1 - v2; });
  } else {
    perform_overloaded_op(b, a, _ret,
                          [&](auto v1, auto v2) { return v1 - v2; });
  }
  return _ret;
}

// The * Overloaded Operator
template <std::size_t v1s, std::size_t v2s, typename V1, typename V2>
auto operator*(const Vec<v1s, V1> &a, const Vec<v2s, V2> &b)
    -> OverloadReturnType<v1s, v2s, V1, V2> {

  auto mul = [&](auto v1, auto v2) { return v1 * v2; };

  auto _ret = OverloadReturnType<v1s, v2s, V1, V2>();

  if (v1s > v2s) {
    perform_overloaded_op(a, b, _ret, mul);
  } else {
    perform_overloaded_op(b, a, _ret, mul);
  }
  return _ret;
}

// The * Overloaded Operator
template <std::size_t v1s, std::size_t v2s, typename V1, typename V2>
auto operator/(const Vec<v1s, V1> &a, const Vec<v2s, V2> &b)
    -> OverloadReturnType<v1s, v2s, V1, V2> {

  auto _ret = OverloadReturnType<v1s, v2s, V1, V2>();

  if (v1s > v2s) {
    perform_overloaded_op(a, b, _ret,
                          [&](auto v2, auto v1) { return v1 / v2; });
  } else {
    perform_overloaded_op(b, a, _ret,
                          [&](auto v1, auto v2) { return v1 / v2; });
  }
  return _ret;
}
} // namespace Vectors

#endif
