#include "triangle.hh"
#include "shape.hh"
#include "vectors/vector_definitions.hh"

using Vectors::Vec;

auto Triangle::check_intersection() const -> bool { return false; }

auto Triangle::check_winding_order() -> void {
  /* - When calculating barycentric coordinates, it involves doing a division
       with two of the three components of the vector x 2

     - This function will change the two vectors is uses to ensure that
       at least one of them is parralel to noaxis, which is guaranteed in any
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
}
