#include "geometry/shape.hh"
#include "geometry/sphere.hh"
#include "geometry/triangle.hh"
#include "vectors/vector_definitions.hh"

#include <variant>

#include "geometry_variant.hh"

using Vectors::Line;

auto GeometryVisitor::call_check_intersection(const GeometryVariant &menu,
                                              Line<3, double> ray)
    -> IntersectionReturnData {
  return std::visit(
      [&](const auto &visited_geometry) {
        return visited_geometry.check_intersection(ray);
      },
      menu);
}
