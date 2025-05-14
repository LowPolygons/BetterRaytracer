#include "geometry/triangle.hh"
#include "vectors/vector_definitions.hh"
#include "vectors/vector_methods.hh"
#include <cstddef>
#include <utility>

using Vectors::areVectorsEqual;
using Vectors::Line;
using Vectors::Plane;
using Vectors::Vec;

auto main() -> int {
  // Two tests, one possibly problematic and one regular for each method
  auto problematic_corners = Vec<3, Vec<3, double>>({{
      {0.0, 0.0, -100.0}, //
      {0.0, 0.0, 100.0},  //
      {0.0, 90.0, 100.0}  //
  }});                    //
  auto okay_corners = Vec<3, Vec<3, double>>({{
      {-100.0, -100.0, -100.0}, //
      {100.0, 100.0, 100.0},    //
      {90.0, -100.0, 90.0}      //
  }});                          //

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

  Triangle problem_t = Triangle(problematic_corners[0], problematic_corners[1],
                                problematic_corners[2], _c);
  Triangle okay_t =
      Triangle(okay_corners[0], okay_corners[1], okay_corners[2], _c);

  auto problem_t_vs = problem_t.get_vectors();
  auto okay_t_vs = okay_t.get_vectors();

  auto problem_def = problem_t.check_intersection(def_intersects);
  auto problem_defnot = problem_t.check_intersection(doesnt_intersect);

  auto okay_def = okay_t.check_intersection(def_intersects);
  auto okay_defnot = okay_t.check_intersection(doesnt_intersect);

  if (areVectorsEqual(problem_t_vs[0], Vec<3, double>{0.0, 90.0, 200.0}) and //
      areVectorsEqual(okay_t_vs[0], Vec<3, double>{200.0, 200.0, 200.0}) and //
      problem_def and okay_def and !problem_defnot and !okay_defnot          //
  ) {
    return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;
}
