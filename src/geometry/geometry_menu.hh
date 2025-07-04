#ifndef GEOMETRY_GEOMETRY_VARIANT_HH

#define GEOMETRY_GEOMETRY_VARIANT_HH

#include "geometry/shape.hh"
#include "geometry/sphere.hh"
#include "geometry/triangle.hh"
#include "vectors/vector_definitions.hh"

#include <variant>

using Vectors::Line;

using GeometryMenu = std::variant<Sphere, Triangle>;

namespace GeometryDirector {
auto call_check_intersection(const GeometryMenu &menu, Line<3, double> ray)
    -> IntersectionReturnData;
};

class Geometry {
public:
  Geometry(GeometryMenu geometry) : geometry(geometry) {}

  auto shape() -> GeometryMenu & { return geometry; }

private:
  GeometryMenu geometry;
};

#endif
