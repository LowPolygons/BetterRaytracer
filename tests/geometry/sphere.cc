#include "geometry/sphere.hh"
#include "vectors/vector_definitions.hh"

#include "vectors/vector_definitions.hh"
#include <cstddef>
#include <cstdlib>
#include <utility>

using Vectors::Line;
using Vectors::Vec;

auto main() -> int {
  // Two Accompanying Lines
  Line<3, double> def_intersects = std::make_pair(   //
      Vec<3, double>{-1000.0, 30.0, 50.0},           //
      Vec<3, double>{1.0, 0.0, 0.0}                  //
  );                                                 //
  Line<3, double> doesnt_intersect = std::make_pair( //
      Vec<3, double>{-1000.0, 0.0, 0.0},             //
      Vec<3, double>{0.0, 0.0, 1.0}                  //
  );

  // Colour (irrelevant)
  auto _c = Vec<8, float>({0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0});

  Sphere test_sphere = Sphere(Vec<3, double>{0.0, 40.0, 40.0}, 50.0, _c);

  auto def = test_sphere.check_intersection(def_intersects);
  auto dont = test_sphere.check_intersection(doesnt_intersect);

  if (def and !dont) {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
