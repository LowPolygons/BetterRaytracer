#include "vectors/vector_overloads.hh"
#include "vectors/vector_definitions.hh"

#include <numeric>
#include <random>
#include <ranges>

// Test to see if the overload operators have their logic correct
using Vectors::Vec;
using Vectors::operator*;
using Vectors::operator+;
using Vectors::operator-;
using Vectors::operator/;

auto main() -> int {
  auto constexpr NUMTESTS = 100;
  auto constexpr MINVAL = -9999;
  auto constexpr MAXVAL = 9999;

  auto rand_gen = std::mt19937();
  auto distribution = std::uniform_int_distribution<>(MINVAL, MAXVAL);

  auto range_iterator = std::views::iota(0, NUMTESTS);

  for (auto counter [[maybe_unused]] : range_iterator) {
    int num1 = distribution(rand_gen);
    int num2 = distribution(rand_gen);

    if (num2 == 0)
      continue;

    auto primitive_results = std::array<int, 4>(
        {num1 + num2, num1 - num2, num1 * num2, num1 / num2});

    auto vec1 = Vec<1, int>({num1});
    auto vec2 = Vec<1, int>({num2});
    auto structure_results = std::array<Vec<1, int>, 4>(
        {vec1 + vec2, vec1 - vec2, vec1 * vec2, vec1 / vec2});

    int num_passed = std::transform_reduce(
        primitive_results.begin(), primitive_results.end(),
        structure_results.begin(), 0, std::plus<int>{},
        [](auto prim, auto struc) {
          if (prim == struc[0]) {
            return 1;
          }
          return 0;
        });

    if (num_passed != 4) {
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
