#ifndef GEOMETRY_GEOMETRY_VARIANT_HH

#define GEOMETRY_GEOMETRY_VARIANT_HH

#include "geometry/shape.hh"
#include "geometry/sphere.hh"
#include "geometry/triangle.hh"
#include "vectors/vector_definitions.hh"

#include <variant>

using Vectors::Line;

using GeometryVariant = std::variant<Sphere, Triangle>;

namespace GeometryVisitor {
auto call_check_intersection(const GeometryVariant &menu, Line<3, double> ray)
    -> IntersectionReturnData;
};

class Geometry {
public:
  Geometry(GeometryVariant geometry) : geometry(geometry) {}

  auto shape() -> GeometryVariant & { return geometry; }

private:
  GeometryVariant geometry;
};

#endif
