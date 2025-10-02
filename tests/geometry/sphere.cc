#include "geometry/sphere.hh"
#include "colour/colour.hh"
#include "vectors/vector_definitions.hh"

#include <cstddef>
#include <cstdlib>
#include <utility>

using Colours::BasicColour;
using Vectors::Line;
using Vectors::Vec;

auto main() -> int {
  // // Two Accompanying Lines
  // auto def_intersects = Line{
  //     //
  //     Vec<3, double>{-1000.0, 30.0, 50.0}, //
  //     Vec<3, double>{1.0, 0.0, 0.0}        //
  // }; //
  // auto doesnt_intersect = Line{
  //     //
  //     Vec<3, double>{-1000.0, 0.0, 0.0}, //
  //     Vec<3, double>{0.0, 0.0, 1.0}      //
  // };
  //
  // // Colour (irrelevant)
  // auto _c = BasicColour({0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0});
  //
  // Sphere test_sphere = Sphere(Vec<3, double>{0.0, 40.0, 40.0}, 50.0, _c);
  //
  // auto def = test_sphere.check_intersection(def_intersects).intersects;
  // auto dont = test_sphere.check_intersection(doesnt_intersect).intersects;
  //
  // if (def and !dont) {
  //   return EXIT_SUCCESS;
  // }
  // return EXIT_FAILURE;
}
