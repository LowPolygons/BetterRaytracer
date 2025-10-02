#ifndef GEOMETRY_SHAPES_METHODS

#define GEOMETRY_SHAPES_METHODS

#include "colour/colour.hh"
#include "vectors/vector_definitions.hh"

using Colours::BasicColour;
using Vectors::Line;

struct IntersectionReturnData {
  bool intersects = false;
  double lambda = -1.0;
  Vec<3, double> normal = {0.0, 0.0, 0.0};
  Vec<3, double> point_of_intersection = {0.0, 0.0, 0.0};
  BasicColour colour = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
};

// class Shape {
// public:
//   virtual auto check_intersection(Line<3, double> ray) const
//       -> IntersectionReturnData = 0;
//   virtual ~Shape() = default;
// };
//
#endif
