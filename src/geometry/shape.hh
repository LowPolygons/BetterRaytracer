#ifndef GEOMETRY_SHAPES_METHODS

#define GEOMETRY_SHAPES_METHODS

#include "vectors/vector_definitions.hh"

using Vectors::Line;

class Shape {
public:
  virtual auto check_intersection(Line<3, double> ray) const -> bool = 0;
  virtual ~Shape() = default;
};

#endif
