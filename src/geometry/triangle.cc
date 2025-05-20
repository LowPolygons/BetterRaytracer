#include "triangle.hh"
#include "shape.hh"
#include "vector_methods.hh"
#include "vectors/vector_advanced.hh"
#include "vectors/vector_definitions.hh"
#include "vectors/vector_overloads.hh"
#include <algorithm>
#include <cstddef>
#include <numeric>

using Vectors::Line;
using Vectors::Plane;
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

  // You only need to search two of the vectors as the rest of duplicates or can
  // be inferred
  auto AB = std::make_pair(corners[1] - corners[0], int{0});
  auto AC = std::make_pair(corners[2] - corners[0], int{0});

  auto CA = std::make_pair(corners[0] - corners[2], int{0});
  auto CB = std::make_pair(corners[1] - corners[2], int{0});

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
  // There are 3 cases for the origin: A, A or C
  index_of_vec_origin = 0;

  // Check how many zero coefs there are
  if (AB.second < 2) {
    vectors[0] = AB.first;
    vectors[1] = AC.first;
  } else if (AC.second < 2) {
    vectors[0] = AC.first;
    vectors[1] = AB.first;
  } else {
    // If it gets here, it means with certainty that the remaining
    // vector has no zero component
    index_of_vec_origin = 2;
    vectors[0] = CB.first;
    vectors[1] = CA.first;
  }

  // Choosing the two coefs used when solving simulatenous EQs for barycentrics

  // Checking for parallelism with axis
  if (std::abs(vectors[0][0]) < CUTOFF and //
      std::abs(vectors[1][0]) < CUTOFF) {
    // The triangle is parallel to the X axis so use Y and Z
    indexs_for_baryc = {1, 2};
  } else if (std::abs(vectors[0][1]) < CUTOFF and
             std::abs(vectors[1][1]) < CUTOFF) {
    // The triangle is parallel to the Y axis so use X and Z
    indexs_for_baryc = {0, 2};
  } else {
    // Either parallel to Z axis, or parallel to none
    indexs_for_baryc = {0, 1};
  }
}

// TODO: Add a check to the triangle check_intersection method to confirm that
// the dot between the normal and the incoming ray is obtuse (MAY NOT BE
// NECESSARY)
auto Triangle::check_intersection(Line<3, double> ray) const -> bool {
  // Return constants needed:
  //  - Lambda value
  //  - POI,
  //  - Success status
  // TODO: remove this comment once implemented

  // Check if the line intersects the plane
  auto point_of_intersection =
      Vectors::line_intersects_plane(ray, triangle_plane);

  if (!point_of_intersection)
    return false;

  auto p_of_i =
      point_of_intersection.value().second; // .first is the lambda value

  // Get the Lambda and Mu vals when converting the POI to barycentrics
  // Shorter alias for indexes
  auto i1 = indexs_for_baryc[0];
  auto i2 = indexs_for_baryc[1];
  auto oi = index_of_vec_origin; // origin index

  /*
     Derive from solving the simulatenous equation:
      o + λa + μb = c
     Where o = Local Origin, a and b are the vectors, c is the p.o.i
     Solving for lambda and mu
  */

  auto lambda =
      (p_of_i[i2] * vectors[0][i1] - p_of_i[i1] * vectors[0][i2] -
       corners[oi][i2] * vectors[0][i1] + corners[oi][i1] * vectors[0][i2]) /
      (vectors[1][i2] * vectors[0][i1] - vectors[1][i1] * vectors[0][i2]);

  auto mu =
      (p_of_i[i1] - corners[oi][i1] - lambda * vectors[1][i1]) / vectors[0][i1];

  if (((0 - CUTOFF) < lambda and lambda < (1 + CUTOFF)) and
      ((0 - CUTOFF) < mu and mu < (1 + CUTOFF))) {
    // Lies at least inside the quadrilateral
    if (lambda + mu < (1 + CUTOFF)) {
      // The ray has collided with the triangle
      return true;
    }
  }
  return false;
}
