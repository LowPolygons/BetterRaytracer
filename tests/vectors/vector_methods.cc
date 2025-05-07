#include "vectors/vector_methods.hh"
#include "vectors/vector_definitions.hh"

#include <cmath>
#include <cstddef>

using Vectors::magnitude;
using Vectors::normalise;
using Vectors::scale;
using Vectors::Vec;

// Warning: Ugly Test
auto main() -> int {

  std::size_t constexpr scale_factor = 4;

  // Vec 2 Variables
  auto vec2_magnitude = Vec<2, int>({3, 4});
  auto vec2_normalise = Vec<2, double>({3.0, 4.0});
  auto vec2_scale = Vec<2, double>({3.0, 4.0});

  // Vec 3 Variables
  auto vec3_magnitude = Vec<3, int>({3, 4, 0});
  auto vec3_normalise = Vec<3, double>({3.0, 4.0, 0.0});
  auto vec3_scale = Vec<3, double>({3.0, 4.0, 0.0});

  auto vec2_mag_result = magnitude(vec2_magnitude);
  normalise(vec2_normalise);
  auto vec2_scale_result = scale(vec2_scale, scale_factor);

  auto vec3_mag_result = magnitude(vec3_magnitude);
  normalise(vec3_normalise);
  auto vec3_scale_result = scale(vec3_scale, scale_factor);

  if (vec2_mag_result == 5 &&                              //
      std::abs(magnitude(vec2_normalise) - 1.0) < 1e-09 && //
      magnitude(vec2_scale_result) == 5 * scale_factor &&  //
      vec3_mag_result == 5 &&                              //
      std::abs(magnitude(vec3_normalise) - 1.0) < 1e-09 && //
      magnitude(vec3_scale_result) == 5 * scale_factor     //
  ) {
    return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;
}
