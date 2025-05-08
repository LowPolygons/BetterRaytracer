#ifndef GEOMETRY_SHAPES_SPHERE_HH

#define GEOMETRY_SHAPES_SPHERE_HH

#include "shape.hh"

class Sphere : public Shape {
public:
  Sphere() {};

  auto check_intersection() const -> bool override;
};

#endif
