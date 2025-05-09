#ifndef GEOMETRY_SHAPES_TRIANGLE_HH

#define GEOMETRY_SHAPES_TRIANGLE_HH

#include "shape.hh"
#include "vectors/vector_definitions.hh"

using Vectors::Vec;

class Triangle : public Shape {
public:
  Triangle() {};

  auto check_intersection() const -> bool override;

  // A function to reshuffle the order of each Position vector to ensure correct
  // winding order
  auto check_winding_order() -> void;

private:
  std::array<Vec<3, double>, 3> corners;
  // Two vectors with a common point
  std::array<Vec<3, double>, 2> vectors;

  // Colour: R, G, B, Emission: R, G, B, Strength, Specular: %
  //         0, 1, 2,           3, 4, 5         6,           7
  Vec<8, float> colour_properties;
};

#endif
