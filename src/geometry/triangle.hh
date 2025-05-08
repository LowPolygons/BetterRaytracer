#ifndef GEOMETRY_SHAPES_TRIANGLE_HH

#define GEOMETRY_SHAPES_TRIANGLE_HH

#include "shape.hh"

class Triangle : public Shape {
public:
  Triangle() {};

  auto check_intersection() const -> bool override;

  // A function to reshuffle the order of each Position vector to ensure correct
  // winding order
  auto check_winding_order() -> void;
};

#endif
