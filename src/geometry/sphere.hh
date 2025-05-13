#ifndef GEOMETRY_SHAPES_SPHERE_HH

#define GEOMETRY_SHAPES_SPHERE_HH

#include "shape.hh"
#include "vectors/vector_definitions.hh"

using Vectors::Vec;

class Sphere : public Shape {
public:
  Sphere(Vec<3, double> ctr, //
         double rds,         //
         Vec<8, float> clr   //
  ) {
    centre = ctr;
    radius = rds;
    colour_properties = clr;
  };

  auto check_intersection(Line<3, double> ray) const -> bool override;

private:
  Vec<3, double> centre;
  double radius;
  // Colour: R, G, B, Emission: R, G, B, Strength, Specular: %
  // Index:  0, 1, 2,           3, 4, 5         6,           7
  Vec<8, float> colour_properties;
};

#endif
