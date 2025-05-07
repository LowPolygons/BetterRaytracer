#include "vectors/vector_definitions.hh"
#include "vectors/vector_methods.hh"
#include "vectors/vector_overloads.hh"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <numeric>

using Vectors::Vec;

// Warning: Ugly Test
auto main() -> int {
  // Cross: Non Zero
  auto cross_non_one = Vec<3, double>({1.0, 0.0, 0.0});
  auto cross_non_two =
      Vec<3, double>({0.0, 1.0, 0.0}); // Cross product is 0, 0, 1
  // Cross: Zero
  auto cross_zero_one = Vec<3, double>({2.0, 2.0, 2.0});
  auto cross_zero_two =
      Vec<3, double>({4.0, 4.0, 4.0}); // Cross Product is 0, 0, 0

  // Dot: Non Zero
  auto dot_non_one = Vec<3, double>({1.0, 2.0, 3.0});
  auto dot_non_two = Vec<3, double>({4.0, 5.0, 6.0}); // Dot Product is 32

  // Dot: Zero
  auto dot_zero_one = Vec<3, double>({1.0, 0.0, 0.0});
  auto dot_zero_two = Vec<3, double>({0.0, 1.0, 0.0}); // Dot product is 0

  // Results
  auto dot_non = Vectors::dot(dot_non_one, dot_non_two);
  auto dot_zero = Vectors::dot(dot_zero_one, dot_zero_two);

  auto cross_non = Vectors::cross(cross_non_one, cross_non_two);
  auto cross_zero = Vectors::cross(cross_zero_one, cross_zero_two);

  if (std::abs(dot_non - 32.0) < 1e-09 &&                    //
      std::abs(dot_zero) < 1e-09 &&                          //
      std::abs(Vectors::magnitude(cross_non) - 1) < 1e-09 && //
      std::abs(Vectors::magnitude(cross_zero)) < 1e-09       //
  ) {
    return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;
}
