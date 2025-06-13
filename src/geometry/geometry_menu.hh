#ifndef GEOMETRY_GEOMETRY_VARIANT_HH

#define GEOMETRY_GEOMETRY_VARIANT_HH

#include "geometry/shape.hh"
#include "geometry/sphere.hh"
#include "geometry/triangle.hh"
#include "vectors/vector_definitions.hh"

#include <variant>

using Vectors::Line;

using GeometryMenu = std::variant<Sphere, Triangle>;

class GeometryDirector {
public:
  static auto check_intersection(const GeometryMenu &menu, Line<3, double> ray)
      -> IntersectionReturnData {
    return std::visit(
        [&](const auto &visited_geometry) {
          return visited_geometry.check_intersection(ray);
        },
        menu);
  }
};

class Geometry {
public:
  Geometry(GeometryMenu geometry) : m_geometry(geometry) {}

  auto shape() -> GeometryMenu & { return m_geometry; }

private:
  GeometryMenu m_geometry;
};

#endif
