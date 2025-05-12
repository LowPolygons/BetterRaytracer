#include "triangle.hh"
#include "shape.hh"
#include "vectors/vector_definitions.hh"
#include "vectors/vector_overloads.hh"
#include <algorithm>
#include <cstddef>
#include <numeric>

using Vectors::Vec;
using Vectors::operator-;

auto const CUTOFF = 1e-09;

auto Triangle::get_valid_vectors() -> void {
  /* - When calculating barycentric coordinates, it involves doing a division
       with two of the three components of the vector x 2

     - This function will change the two vectors is uses to ensure that
       at least one of them is parallel to noaxis, which is guaranteed in any
       triangle.

     - This ensures that it will have at least 2 non zero components
       By ensuring that at least one of the twovectors meets this requirementm
       it guarantees that there is somecombination of two vectors
       such that the denominator:

     -> vOne.b*vTwo.a - vOne.a*vTwo.b

       CANNOT be zero.

     - There is one exception and that is if all points lie on a line, but in
       that case it wouldn't render anyway
  */
  auto AB = std::make_pair(corners.at(1) - corners.at(0), int{0});
  auto AC = std::make_pair(corners.at(2) - corners.at(0), int{0});
  auto CB = std::make_pair(corners.at(2) - corners.at(1), int{0});

  // Check how many coefficients equal to zero there are in 2/3 vectors
  AB.second = std::transform_reduce(AB.first.begin(), AB.first.end(), int{0}, //
                                    std::plus<int>{},                         //
                                    [&](auto coef) {                          //
                                      if (std::abs(coef) < CUTOFF) {          //
                                        return 1;                             //
                                      }
                                      return 0;
                                    });
  AC.second = std::transform_reduce(AC.first.begin(), AC.first.end(), int{0}, //
                                    std::plus<int>{},                         //
                                    [&](auto coef) {                          //
                                      if (std::abs(coef) < CUTOFF) {          //
                                        return 1;                             //
                                      }
                                      return 0;
                                    });

  // Check how many zero coefs there are
  if (AB.second < 2 || AC.second < 2) {
    vectors.at(0) = AB.first;
    vectors.at(1) = AC.first;
  } else {
    // If AB or AC both match this, it means with certainty that the remaining
    // vector has no zero component
    vectors.at(0) = CB.first;
    vectors.at(1) = AC.first;
  }
}

auto Triangle::check_intersection() const -> bool { return false; }
