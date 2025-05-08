#ifndef GEOMETRY_SHAPES_METHODS

#define GEOMETRY_SHAPES_METHODS

class Shape {
public:
  virtual auto check_intersection() const -> bool;
};

#endif
